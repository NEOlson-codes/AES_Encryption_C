/*
 ============================================================================
 Name        : AES_256.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : AES 256-bit encryption/decryption module. Hardware independent.
 ============================================================================
 */

#ifndef AES_256_H_
#define AES_256_H_

#include <stdint.h>

/*
 * Purpose : Main encryption function
 * Inputs  : Takes 128 bit block of plaintext data to perform cipher on.
 *           Also takes 256-bit cipher key.
 * Outputs : Function writes cipher output directly to the memory location
 *           pointed to by the parameter "data_128_bits".
 */
void encrypt_block_128(char*, uint32_t);


/*
 * Purpose : Main decryption function
 * Inputs  : Takes pointer to 128 bit block of encrypted data to decipher.
 *           Also takes 256-bit cipher key.
 * Outputs : Function writes cipher output directly to the memory location
 *           pointed to by the parameter "data_128_bits".
 */
void decrypt_block_128(char* data_128_bits, uint32_t cipher_key);


/*
 * Purpose : Provides the key schedule for the first operation of the add key
 *           function
 * Inputs  :
 * Outputs : A pointer to an array of Nb * (Nr + 1) 32-bit words (the key
 *           schedule). Nr is defined as the number of rounds (14 for 256
 *           bit AES). Nb is defined as
 */
uint32_t generate_key_schedule(uint32_t* cipher_key);









#endif /* AES_256_H_ */
