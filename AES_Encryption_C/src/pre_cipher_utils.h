/*
 ============================================================================
 Name        : pre_cipher_utils.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 21, 2022
 Description : Contains several function declarations for functions defining
               global variables and performing other work that is set-up
               for the cipher process. This includes creating the key schedule
               and defining cipher key length-dependent parameters.
 ============================================================================
 */

#ifndef PRE_CIPHER_UTILS_H_
#define PRE_CIPHER_UTILS_H_

#include <stdint.h>
#include "stddef.h"

extern uint8_t Nb, Nr, Nk;

/*
 * Purpose : Defines parameters related to the cipher key length. Example: there
 *           will be a different number of transformation rounds based on the
 *           cipher key length.
 * Inputs  : Cipher key length
 * Outputs : Defines global variables Nk, Nb, and Nr (see description in function
 * 			 definition for each).
 */
void set_global_params(uint32_t cipher_key_len);


/*
 * Purpose : Creates the key schedule used on the state matrix during every
 *           transformation round.
 * Inputs  : Cipher key
 * Outputs : Returns a pointer to the key_schedule array. key_schedule is
 *           declared as static so the memory remains allocated for the
 *           duration of the program.
 */
uint8_t* generate_key_schedule(uint8_t* cipher_key);

uint8_t mult_by_x(uint8_t byte, uint8_t num_multiplications);


#endif /* PRE_CIPHER_UTILS_H_ */
