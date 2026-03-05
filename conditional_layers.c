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

#include "print.h"
#include "keymap.h"
#include "conditional_layers.h"

// Stores conditional layer masks that will be checked against the layer state
// as it changes over time.

static layer_state_t conditional_layer[MAX_LAYER] = {
  [0 ... MAX_LAYER - 1] = CL_NO_MASK
};

layer_state_t layer_state_set_conditional_layers(layer_state_t state) {

  // Go through every conditional layer.

  for (uint16_t layer = 0; layer < MAX_LAYER; ++layer) {
    layer_state_t mask = conditional_layer[layer];
    if (mask != CL_NO_MASK) {
      if ((mask & state) == mask)
        state |= LAYER_BIT(layer);
      else
        state &= ~(LAYER_BIT(layer));
    }
  }
  return state;
}

__attribute__((weak)) void conditional_layer_set() {
}

void keyboard_post_init_conditional_layers() {
  conditional_layer_set();
}

void conditional_layer_set_mask(layer_state_t layer, layer_state_t mask) {
  conditional_layer[layer] = mask;
}
