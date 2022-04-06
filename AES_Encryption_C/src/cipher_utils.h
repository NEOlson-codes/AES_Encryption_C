/*
 ============================================================================
 Name        : cipher_utils.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 22, 2022
 Description : Contains functions used in the AES cipher algorithm (encryption)
               and functions used in its inversion (decryption)
 Note 1      : Keeping the state as a 1D matrix rather than transforming it into
               a 2D (4x4) matrix is intentional. Copying the state into a 2D matrix
               would make the code in the functions below more readable, but it
               would also require many copy operations (at least 1x the entire
               input dataset). If this module were used to encrypt GB of data then
               copying the whole input dataset would make the module significantly
               slower.
 ============================================================================
 */

#ifndef CIPHER_UTILS_H_
#define CIPHER_UTILS_H_

#include <stdint.h>
#include "pre_cipher_utils.h"
#include "aes_encryption.h"


/*
 * Purpose : XOR's (finite fields "add") current round key to the state matrix
 * Inputs  : State matrix, key schedule, round number, 1 bit specifying
 *           whether encryption or decryption is occurring.
 * Outputs : Transformed state matrix
 */
void add_round_key(uint8_t* state, uint8_t* round_key, uint8_t* round_num, uint8_t is_decrypt);


/*
 * Purpose : Shifts rows 2, 3, and 4 of the 2D (4x4) state matrix right
 * Inputs  : State matrix
 * Outputs : Transformed (row shifted) state matrix
 */
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
 * Purpose : Multiplies a finite field polynomial by x. Multiplication of two large order
 *           polynomials can be broken into one of the polynomials being multiplied by
 *           x many times (with polynomial modulo if order =8). See NIST standard
 *           document (FIPS 197) for more details.
 * Inputs  : Finite field polynomial to multiply (represented by a binary byte) and the
 *           number of times it will be multiplied by x.
 * Outputs : Polynomial product of multiplication
 */
uint8_t mult_by_x(uint8_t byte, uint8_t num_multiplications);


/*
 * Purpose : Allows for a succinct multiplication of a finite field
 *           polynomial A by another polynomial B where (B % X != 0).
 *           {02} represents a multiply by x in finite fields notation.
 *           {03} is x + 1, and so on. See the NIST standard (FIPS 197) for more
 *           information. This function is needed specifically for use in inv_mix_col_words().
 * Inputs  : Expansion polynomial in hex, byte to transform
 * Outputs : Transformed state matrix (column bytes mixed)
 */
uint8_t mult_by_x_expansion(uint8_t expansion_hex, uint8_t byte);


#endif /* CIPHER_UTILS_H_ */
