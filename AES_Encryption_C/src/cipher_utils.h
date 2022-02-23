/*
 ============================================================================
 Name        : cipher_utils.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 22, 2022
 Description : Contains functions used in the AES cipher algorithm (encryption)
               and functions used in its inversion (decryption)
 ============================================================================
 */

#ifndef CIPHER_UTILS_H_
#define CIPHER_UTILS_H_

#include <stdint.h>
#include "pre_cipher_utils.h"

/*
 * Purpose : XOR's (finite fields "add") current round key to the state matrix
 * Inputs  : State matrix, key schedule, round number, 1 bit specifying
 *           whether encryption or decryption is occurring.
 * Outputs : Transformed state matrix
 */
void add_round_key(uint8_t* state, uint8_t* round_key, uint8_t* round_num, uint8_t encrypt_or_decrypt);


/*
 * Purpose : Shifts rows 2, 3, and 4 of the 2D (4x4) state matrix right
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
 * Purpose : Perform finite fields math operations on the bytes in each column.
		     Simplified equations for each element given by NIST standard
		     document (FIPS 197).
 * Inputs  : State matrix
 * Outputs : Transformed state matrix (column bytes mixed)
 */
void mix_col_words(uint8_t* state);


/*
 * Purpose : Performs the inverse operation of "shift_rows()" above.
 *           Shifts rows 2, 3, and 4 of the 2D (4x4) state matrix left
 * Inputs  : State matrix
 * Outputs : Transformed (row shifted) state matrix
 */
void inv_shift_rows(uint8_t* state);


/*
 * Purpose : Performs the inverse operation of "mix_col_words()" above.
 *           Finite fields math operations are done on the bytes in each column.
 *	         Simplified equations for each element given by NIST standard
 *	         document (FIPS 197).
 * Inputs  : State matrix
 * Outputs : Transformed state matrix (column bytes mixed)
 */
void inv_mix_col_words(uint8_t* state);


/*
 * Purpose : Allows for a succinct multiplication of a finite field
 *           polynomial by x many times. {02} represents a multiply
 *           by x in finite fields notation. {04} is x^2, and so on. See the
 *           NIST standard (FIPS 197) for more information. This function is
 *           needed specifically for use in inv_mix_col_words().
 * Inputs  : Expansion polynomial in hex, byte to transform
 * Outputs : Transformed state matrix (column bytes mixed)
 */
uint8_t mult_by_x_expansion(uint8_t expansion_hex, uint8_t byte);


#endif /* CIPHER_UTILS_H_ */
