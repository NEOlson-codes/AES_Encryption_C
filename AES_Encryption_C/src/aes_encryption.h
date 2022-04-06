/*
 ============================================================================
 Name        : aes_encryption.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : AES encryption/decryption module. Hardware independent.
               Can use 128, 192, or 256 bit keys.
 Note 1      : To run on embedded hardware, this module will require a wrapper function
               to provide data and encryption parameters to the module.
 Note 2      : This module takes the liberty of deleting (overwriting) the cipher key
               after encryption/decryption is completed. If this did not occur, there
               would be a local copy of the cipher key existing in memory (which would
               compromise security of the encrypted data).
 ============================================================================
 */

#ifndef AES_ENCRYPTION_H_
#define AES_ENCRYPTION_H_

#include <stdlib.h>
#include <stdint.h>
#include "s_box.h"
#include "pre_cipher_utils.h"
#include "cipher_utils.h"

#define AES_CONSTANTS

#define  EXIT_SUCCESS    0
#define  BYTES_IN_WORD   4
#define  BYTES_IN_STATE  16
#define  HALF_BYTE       4
#define  WORDS_IN_STATE  4
#define  COLS_IN_STATE   4
#define  ROWS_IN_STATE	 4


/*
 * Purpose : Wrapper function for encryption & decryption function calls. Includes error
 *           handling and initialization of parameters used in both encryption and decryption.
 * Inputs  : 1. Pointer to 16 bytes of data to encrypt/decrypt
 *           2. Length of the cipher key (should be 128, 192, or 256)
 *           3. Pointer to the cipher key
 *           4. Flag indicating encryption ('0') or decryption ('1')
 * Outputs : Message indicating success or a non-zero error code. Erases cipher key.
 */
uint32_t use_aes(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key, uint8_t is_decrypt);


/*
 * Purpose : Main encryption function
 * Inputs  : Takes 128 bit block of plaintext data to perform cipher on and a
 *           128, 192, or 256 bit cipher key.
 * Outputs : Function writes cipher output directly to the memory location
 *           pointed to by the parameter "data_16_bytes". That memory location
 *           is predetermined by a separate hardware-specific wrapper module.
 */
void encrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, uint8_t is_decrypt, const uint8_t Nr);


/*
 * Purpose : Main decryption function
 * Inputs  : Takes pointer to 128 bit block of encrypted data to decipher.
 *           Also takes 128, 192, or 256 bit cipher key.
 * Outputs : Function writes deciphered output directly to the memory location
 *           pointed to by the parameter "data_128_bits". That memory location
 *           is predetermined by a separate hardware-specific wrapper module.
 */
void decrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, uint8_t is_decrypt, const uint8_t Nr);


/*
 * Purpose : Catch invalid inputs to use_aes()
 * Inputs  : Pointers to the cipher key and data block, integer inputs for
 *           cipher key length and encrypt/decrypt flag
 * Outputs : If there is no error, function returns zero. If there is an error,
 *           the codes are as follows.
 *           '1' - data_16_bytes is a null pointer
 *           '2' - cipher_key_len is an invalid length (not 128, 192, or 256)
 *           '3' - cipher_key is a null pointer
 *           '4' - is_decrypt flag is not '0' (encrypt) or '1' (decrypt)
 */
uint8_t check_invalid_input(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key, uint8_t is_decrypt);



#endif /* AES_ENCRYPTION_H_ */
