// Copyright 2026 Hannah Blythe Morrison
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "conditional_layers.h"

// Array that stores conditional layers that will be processed as the layer
// state changes over time. By default all of the conditional layers in the
// array are initialised to be 'off', and will not affect the layer state.

// Thank you to to https://github.com/yeroca for sharing this elegant way of
// defining the size of an array-of-things in a static way that can also be
// modified by the keymap writer (by defining MAX_CONDITIONAL_LAYER in
// keymap.c).

static conditional_layer_t conditional_layer[MAX_CONDITIONAL_LAYER] = {
  [0 ... MAX_CONDITIONAL_LAYER - 1] = CONDITIONAL_LAYER(CL_CONDITIONAL_LAYER_OFF, CL_NO_LAYER)
}

// This is a placeholder function that should be defined in keymap.c and should
// contain a number of add_conditional_layer(CONDITIONAL_LAYER( ... )) function
// calls to get all of the keymap's conditional layers defined up front.

__attribute__ ((weak)) void conditional_layer_init () {
  return;
}

// Call the conditional_layer_init() function after the keyboard initialisation
// process has completed.

void keyboard_post_init_conditional_layer () {
  conditional_layer_init();
}

// Looks in the given layer state for any conditional layers that might apply. If one
// is found, the layer state is modified before being returned. If more than one
// is found, the layer comprising the highest combination of individual layers
// will be selected. This also allows the use of an arithmetic comparison, which
// is no coincidence.

layer_state_t layer_state_set_conditional_layer(layer_state_t state) {
  uint16_t      largest_index = 0;
  layer_state_t largest_mask  = 0;

  // Deactivate any currently active conditional layers whose masks no longer
  // match the current layer state. The first part of the if statement is
  // obvious. It excludes any conditional layers that are not active. The
  // second part of the if statement relies on the logical-AND operator.
  //
  // If a conditional layer is currently active, then its layer mask must have
  // matched the layer state, meaning that mask & state would have been at least
  // identical to the layer state. If mask & state was not identical to state
  // then state must have at least one more bit set to 1. Arithmetically, that
  // means when they conditional layer is active, mask & state will be AT LEAST
  // as large as mask.
  //
  // Now that the layer state has changed, if any of the layers that make up the
  // same conditional layer mask are now not active, then mask & state will contain
  // at least one zero that it never used to, which means that it MUST be
  // smaller arithmetically than mask. Note that state itself may well be much
  // larger, arithmetically speaking, since any number of layers not present in
  // the mask may also be active - but the layer mask ANDs those potentially set
  // bits out.

  for (uint16_t index = 0; index < MAX_CONDITIONAL_LAYER; ++index) {
    conditional_layer_t *cl = get_conditional_layer(index);
    if (cl->active && cl->mask > cl->mask & state) {
      cl->active = false;

      // We can remove the now inactive conditional layer by ANDing the current
      // layer state with the inverse of the layer (in other words, all 1s
      // except for the layer bit itself, which will be 0).

      state &= ~(cl->layer);
    }
  }

  // Now activate the conditional layer whose mask matches the current layer
  // state. There may be more than one conditional layer mask that matches so
  // this process involves finding the arithmetically largest conditional layer
  // mask that matches the current layer state.
  //
  // Potentially ANY conditional layer whose mask matches could be activated,
  // but right now the code selects the biggest one, which should correspond to
  // the conditional layer that contains the highest currently active layer in
  // its layer mask.

  for (uint16_t index = 0; index < MAX_CONDITIONAL_LAYER; ++index) {

    // Get the next conditional layer.

    conditional_layer_t *cl = get_conditional_layer(index);

    // If the cl mask matches the current layer state, save the mask and the
    // current index, but only if the mask is larger than any mask that has
    // already been found.

    if (cl && state & cl->mask) {
      if (cl->mask > largest_mask) {
        largest_mask  = cl->mask;
        largest_index = index;
      }
    }
  }

  // If a matching conditional layer was found, add the conditional layer bit to
  // the current layer state.

  if (largest_mask) {
    conditional_layer_t *largest_cl = get_conditional_layer(largest_index);
    state |= LAYER_BIT(largest_cl->layer);

    // Set the conditional layer to active.

    largest_cl->active = true;
  }

  // Return the potentially modified state.

  return state;
}

// Return a pointer to the conditional layer found at the given index.

conditional_layer_t* get_conditional_layer(uint16_t index) {
  return index < MAX_CONDITIONAL_LAYER ? &conditional_layer[index] : NULL;
}

// Add the given conditional layer to the list of conditional layers if there is
// room.

uint16_t add_conditional_layer(conditional_layer_t *cl) {
  for (uint16_t index = 0; index < MAX_CONDITIONAL_LAYER; ++index) {
    if (conditional_layer[index].mask == CL_CONDITIONAL_LAYER_OFF) {

      // Set the state of the conditional layer at the index to match the state
      // of the argument.

      conditional_layer[index].mask  = cl->mask;
      conditional_layer[index].layer = cl->layer;

      // Return the index of the conditional layer as a success code.

      return index;
    }
  }

  // If we reach here then no unused conditional layers has been found, so
  // return an abberant conditional layer index.

  return MAX_CONDITIONAL_LAYER + 1;
}

// Remove a conditional layer that matches the argument - although actually this
// function will remove all conditional layers that match the argument.

void remove_conditional_layer(conditional_layer_t *cl) {
  for (uint16_t index = 0; index < MAX_CONDITIONAL_LAYER; ++index) {
    if (conditional_layer[index].mask == cl->mask
        && conditional_layer[index].layer == cl->layer) {

      // Set the conditional layer at the index to the default state.

      conditional_layer[index].mask  = CL_CONDITIONAL_LAYER_OFF;
      conditional_layer[index].layer = CL_NO_LAYERS;
    }
}
