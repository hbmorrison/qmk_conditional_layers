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

// Indicates that the conditional layer is not being used.

#define CL_NO_MASK (0)

// Works out the layer bit associated with a given layer. A layer mask can be
// built up using logical-or.
// e.g. LAYER_BIT(LAYER_BASE) | LAYER_BIT(LAYER_SYMBOL)

#define LAYER_BIT(code) (1 << ((code) & 0x7f))

// Function declarations.

void add_conditional_layers(void);
void set_conditional_layer_mask(layer_state_t, layer_state_t);
