/*
 ============================================================================
 Name        : AES_Encryption.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : AES encryption/decryption module. Hardware independent. Can
               use 128, 192, or 256 bit keys.
 ============================================================================
 */

#ifndef AES_256_H_
#define AES_256_H_

#include <stdint.h>
#include "s_box.h"
#include "pre_cipher_utils.h"
#include "cipher_utils.h"


//extern uint8_t Nb, Nr, Nk;

/*
 * Purpose : Main encryption function
 * Inputs  : Takes 128 bit block of plaintext data to perform cipher on.
 *           Also takes 256-bit cipher key.
 * Outputs : Function writes cipher output directly to the memory location
 *           pointed to by the parameter "data_128_bits".
 */
void encrypt_16_bytes(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key);


/*
 * Purpose : Main decryption function
 * Inputs  : Takes pointer to 128 bit block of encrypted data to decipher.
 *           Also takes 256-bit cipher key.
 * Outputs : Function writes cipher output directly to the memory location
 *           pointed to by the parameter "data_128_bits".
 */
//void decrypt_16_bytes();



#endif /* AES_256_H_ */
