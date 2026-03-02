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

#pragma once

#include QMK_KEYBOARD_H

// Set the default maximum number of conditional layers.

#ifndef MAX_CONDITIONAL_LAYER
#define MAX_CONDITIONAL_LAYER MAX_LAYER
#endif

// Conditional layer.

typedef struct conditional_layer_t {
  layer_state_t mask;
  layer_state_t layer;
} conditional_layer_t;

// This module makes the assumption that the base layer layer mask, which is
// equivalent to 0, will not mean much since, almost always, the base layer will
// be active, and also because conditional layers will usually be associated
// with a layer-changing key press that introduces a higher layer. For this
// reason, the zero'th layer can be used as a 'noop' flag that indicates the
// conditional layer is not being used.

#define CL_CONDITIONAL_LAYER_OFF 0
#define CL_NO_LAYERS             (1 << 0)

// List of layer bits that can be OR'ed to create the layer mask. It is easy to
// write LAYER_ONE | LAYER_THREE but it might be less clear what those layers
// are for, so also look at LAYER_BIT() below as a more semantic alternative.

#define CL_LAYER_ONE             (1 << 1)
#define CL_LAYER_TWO             (1 << 2)
#define CL_LAYER_THREE           (1 << 3)
#define CL_LAYER_FOUR            (1 << 4)
#define CL_LAYER_FIVE            (1 << 5)
#define CL_LAYER_SIX             (1 << 6)
#define CL_LAYER_SEVEN           (1 << 7)
#define CL_LAYER_EIGHT           (1 << 8)
#define CL_LAYER_NINE            (1 << 9)
#define CL_LAYER_TEN             (1 << 10)
#define CL_LAYER_ELEVEN          (1 << 11)
#define CL_LAYER_TWELVE          (1 << 12)
#define CL_LAYER_THIRTEEN        (1 << 13)
#define CL_LAYER_FOURTEEN        (1 << 14)
#define CL_LAYER_FIFTEEN         (1 << 15)

// Layer masks can also be constructed by using LAYER_BIT() to work out the bit
// of the layer mask for a given layer. The layer mask can be built up by ORing
// together LAYER_BIT() statements.
//
// For example, LAYER_BIT(LAYER_NUM) | LAYER_BIT(LAYER_SYMBOL) defines a
// two-layer mask, where LAYER_NUM and LAYER_SYMBOL are layers defined in the
// layer enum in keymap.c.

#define LAYER_BIT(code) (1 << ((code) & 0x7f))

// Define a conditional layer.

#define CONDITIONAL_LAYER(mask, layer) { \
  .mask   = (mask),                      \
  .layer  = (layer),                     \
  .active = false                        \
}

// Function declarations.

conditional_layer_t* get_conditional_layer(uint16_t);
uint16_t add_conditional_layer(conditional_layer_t *);
void remove_conditional_layer(conditional_layer_t *);
