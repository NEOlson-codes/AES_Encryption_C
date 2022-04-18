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
 Note 1      : Readers of the code will note that all of the operations on the
               state vector are byte-indexed (as opposed to word indexed).
               There is a very specific reason for this. All the operations in
               this algorithm are byte operations. Words would need to be
               decomposed in order to operate on the bytes, which would be
               very inefficient.
 ============================================================================
 */

#ifndef PRE_CIPHER_UTILS_H_
#define PRE_CIPHER_UTILS_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>
#include "stddef.h"
#include "s_box.h"
#include "cipher_utils.h"
#include "aes_encryption.h"

typedef enum key_dependent_params{Nk_, Nr_} key_params;

/*
 * Purpose : Defines parameters related to the cipher key length. Example: there
 *           will be a different number of transformation rounds based on the
 *           cipher key length.
 * Inputs  : Cipher key length, desired parameter
 * Outputs : Variable Nk or variable Nr
 * Notes   : Nk is defined as the number of 32-bit words in the cipher key. This
 *           information is primarily used in creating the key schedule. Nr is the
 *           number of rounds of "scrambling" that occurs to the state vector.
 */
uint8_t set_algo_params(uint32_t cipher_key_len, key_params param);


/*
 * Purpose : Creates the key schedule used on the state matrix during every
 *           transformation round.
 * Inputs  : Cipher key
 * Outputs : Returns a pointer to the key_schedule array. key_schedule is
 *           declared as static so the memory remains allocated for the
 *           duration of the program.
 */
uint8_t* generate_key_schedule(uint8_t* cipher_key, const uint8_t Nr, const uint8_t Nk);


#ifdef __cplusplus
}
#endif

#endif /* PRE_CIPHER_UTILS_H_ */
