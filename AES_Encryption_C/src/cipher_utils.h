/*
 ============================================================================
 Name        : cipher_utils.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 22, 2022
 Description : Contains the mathematical operations used in the AES cipher algorithm
 ============================================================================
 */



#ifndef CIPHER_UTILS_H_
#define CIPHER_UTILS_H_

#include "aes_encryption.h"

// XOR's (finite field "add") current round key to the state matrix
void add_round_key(uint8_t* state, uint8_t* round_key, uint8_t* round_num, aes_op_flag aes_operation);


// Shifts rows 2, 3, and 4 of the 2D (4x4) state matrix right
void shift_rows(uint8_t* state);


// Perform finite fields math operations on the bytes in each column.
// Simplified equations for each element given by NIST standard document (FIPS 197).
void mix_col_words(uint8_t* state);

void inv_shift_rows(uint8_t* state);

void inv_mix_col_words(uint8_t* state);


/*
 * Multiplies a finite field polynomial by x. Multiplication of two large order
 * polynomials can be broken into one of the polynomials being multiplied by
 * x many times (with polynomial modulo if order =8). See NIST standard
 * document (FIPS 197) for more details.
 */
uint8_t mult_by_x(uint8_t byte, uint8_t num_multiplications);


/*
 * Allows for a succinct multiplication of a finite field
 * polynomial A by another polynomial B where (B % X != 0).
 * {02} represents a multiply by x in finite fields notation.
 * {03} is x + 1, and so on. See the NIST standard (FIPS 197) for more
 * information. This function is needed specifically for use in inv_mix_col_words().
 */
uint8_t mult_by_x_expansion(uint8_t expansion_hex, uint8_t byte);


#endif /* CIPHER_UTILS_H_ */
