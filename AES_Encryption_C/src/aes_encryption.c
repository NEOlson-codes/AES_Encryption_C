/*
 ============================================================================
 Name        : aes_encryption.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : AES encryption/decryption module. Hardware independent. Can use 128,
               192, or 256 bit keys. The module is run with use_aes(), which contains
               all functionality. See aes_encryption.h for input/output details.
 Note 1      : To run on embedded hardware, this module will require a wrapper function
               to provide data and encryption parameters to the module.
 Note 2      : This module takes the liberty of deleting (overwriting) the cipher key
               after encryption/decryption is completed. If this did not occur, there
               would be a local copy of the cipher key existing in memory (which would
               compromise security of the encrypted data).
 ============================================================================
 */


#include "aes_encryption.h"

// Details for all functions can be found in aes_encryption.h

uint32_t use_aes(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key, uint8_t is_decrypt){


	uint8_t invalid_input = check_invalid_input(data_16_bytes, cipher_key_len, cipher_key, is_decrypt);

	if (invalid_input){
		return invalid_input;
	}

	// Set parameters related to cipher key length (e.g. number of scrambling rounds).
	const uint8_t Nk = set_algo_params(cipher_key_len, Nk_);
	const uint8_t Nr = set_algo_params(cipher_key_len, Nr_);

	uint8_t* round_keys = generate_key_schedule(cipher_key, Nr, Nk);

	if(is_decrypt == 0){
		encrypt_16_bytes(data_16_bytes, round_keys, is_decrypt, Nr);
	}
	else if(is_decrypt == 1){
		decrypt_16_bytes(data_16_bytes, round_keys, is_decrypt, Nr);
	}

	// Erase the cipher key (as mentioned above)
	for(uint8_t i = 0; i < (Nk * BYTES_IN_WORD); i++){
		cipher_key[i] = 0;
	}


	return EXIT_SUCCESS;
}


void encrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, uint8_t is_decrypt, const uint8_t Nr){

	// Note: Encrypted data will overwrite original message
	uint8_t* state = data_16_bytes;

	uint8_t round_num = 0;

	add_round_key(state, round_keys, &round_num, is_decrypt);

	while(round_num < Nr){

		for(uint8_t i = 0; i < BYTES_IN_STATE; i++) state[i] = apply_sbox(state[i]);

		shift_rows(state);

		mix_col_words(state);

		add_round_key(state, round_keys, &round_num, is_decrypt);
	}

	// Final round does not include mix_col_words
	for(uint8_t i = 0; i < BYTES_IN_STATE; i++){
		state[i] = apply_sbox(state[i]);
	}
	shift_rows(state);
	add_round_key(state, round_keys, &round_num, is_decrypt);

	for(int i = 0; i < 16; i++) printf("0x%x ",state[i]);

}


void decrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, uint8_t is_decrypt, const uint8_t Nr){

	uint8_t* state = data_16_bytes;

	uint8_t round_num = Nr;

	add_round_key(state, round_keys, &round_num, is_decrypt);

	while(round_num > 0){

		inv_shift_rows(state);

		for(uint8_t i = 0; i < BYTES_IN_STATE; i++){
			state[i] = apply_inv_sbox(state[i]);
		}

		add_round_key(state, round_keys, &round_num, is_decrypt);

		inv_mix_col_words(state);

	}

	// The final round (# 0) doesn't include mix_col_words
	inv_shift_rows(state);
	for(uint8_t i = 0; i < BYTES_IN_STATE; i++){
		state[i] = apply_inv_sbox(state[i]);
	}

	add_round_key(state, round_keys, &round_num, is_decrypt);

}


uint8_t check_invalid_input(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key, uint8_t is_decrypt){

	// Error code '1': pointer to 16 byte input is a null pointer
	if(data_16_bytes == NULL) return 1;

	// Error code '2': cipher key has an invalid length (not 128, 192, 256)
	if((cipher_key_len != 128) && (cipher_key_len != 192) && (cipher_key_len != 256)) return 2;

	// Error code '3': pointer to cipher key is a null pointer
	if(cipher_key == NULL) return 3;

	// Error code '4': is_decrypt does not specify '0' (encryption) or '1' (decryption)
	if((is_decrypt != 0) && (is_decrypt != 1)) return 4;

	// If input is valid, return zero
	return 0;
}
