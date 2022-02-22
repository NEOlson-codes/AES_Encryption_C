/*
 ============================================================================
 Name        : cipher_utils.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 22, 2022
 Description : Contains functions used in the AES cipher algorithm
 ============================================================================
 */

#ifndef CIPHER_UTILS_H_
#define CIPHER_UTILS_H_

#include <stdint.h>
#include "pre_cipher_utils.h"

/*
 * Purpose : XOR's (finite fields "add") current round key to the state matrix
 * Inputs  : State matrix, key schedule, round number
 * Outputs : Transformed state matrix
 */
void add_round_key(uint8_t* state, uint8_t* round_key, uint8_t* round_num);


/*
 * Purpose : Shifts rows 2, 3, and 4 of the 2D (4x4) state matrix
 * Inputs  : State matrix
 * Outputs : Transformed (row shifted) state matrix
 */

// Concept of 2D state matrix. b# is the byte number in the 1D state matrix.
//      Col    0    1    2    3
// Row
//   0        b0   b4   b8   b12
//   1        b1   b5   b9   b13
//   2        b2   b6   b10  b14
//   3        b3   b7   b11  b15

void shift_rows(uint8_t* state);


/*
 * Purpose : Shifts rows 2, 3, and 4 of the 2D (4x4) state matrix
 * Inputs  : State matrix
 * Outputs : Transformed (row shifted) state matrix
 */
void mix_col_words(uint8_t* state);

#endif /* CIPHER_UTILS_H_ */
