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

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct aes_output_msg{
	uint8_t termination_code;
	char msg[100];
} aes_out;

typedef struct cipher_parameters{
	uint8_t Nk; // Number of 32-bit words
	uint8_t Nr; // Number of rounds
} params;

typedef enum{key_128 = 128, key_192 = 192, key_256 = 256} cipher_len;

typedef enum{encrypt, decrypt} aes_op_flag;

#include "s_box.h"
#include "pre_cipher_utils.h"
#include "cipher_utils.h"

#define  BYTES_IN_WORD   4
#define  BYTES_IN_STATE  16
#define  HALF_BYTE       4
#define  WORDS_IN_STATE  4
#define  COLS_IN_STATE   4
#define  ROWS_IN_STATE	 4


aes_out use_aes(uint8_t* data_16_bytes, cipher_len cipher_key_len, uint8_t* cipher_key, aes_op_flag aes_op);

void encrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, aes_op_flag aes_op, const uint8_t Nr);

void decrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, aes_op_flag aes_op, const uint8_t Nr);

// Checks inputs to use_aes() to prevent null pointers and invalid numerical inputs
void check_invalid_input(uint8_t* data_16_bytes, uint8_t* cipher_key, aes_out* aes_out_ptr);


#ifdef __cplusplus
}
#endif

#endif /* AES_ENCRYPTION_H_ */
