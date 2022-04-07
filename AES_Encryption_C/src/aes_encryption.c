/*
 ============================================================================
 Name        : aes_encryption.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : AES encryption/decryption module. Hardware independent. Can use 128,
               192, or 256 bit keys. The module is run with use_aes(), which contains
               all functionality.
 Note 1      : To run on embedded hardware, this module will require a wrapper function
               to provide data and encryption parameters to the module.
 Note 2      : This module takes the liberty of deleting (overwriting) the cipher key
               after encryption/decryption is completed. If this did not occur, there
               would be a local copy of the cipher key existing in memory (which would
               compromise security of the encrypted data).
 ============================================================================
 */


#include "aes_encryption.h"

aes_out use_aes(uint8_t* data_16_bytes, cipher_len cipher_key_len, uint8_t* cipher_key, aes_op_flag aes_op){

	aes_out output_code = {
		.termination_code = 0,
		.msg = "Encrypt/decrypt successful"
	};

	check_invalid_input(data_16_bytes, cipher_key, &output_code);

	if (output_code.termination_code != 0){
		return output_code;
	}

	// Set parameters related to cipher key length (e.g. number of scrambling rounds).
	params cp;

	switch (cipher_key_len){
		case (128):
			cp.Nk = 4;
			cp.Nr = 10;
			break;
		case (192):
			cp.Nk = 6;
			cp.Nr = 12;
			break;
		default: // 256-bit key
			cp.Nk = 8;
			cp.Nr = 14;
	}

	uint8_t* round_keys = generate_key_schedule(cipher_key, cp.Nr, cp.Nk);

	if(aes_op == encrypt){
		encrypt_16_bytes(data_16_bytes, round_keys, aes_op, cp.Nr);
	}
	else if(aes_op == decrypt){
		decrypt_16_bytes(data_16_bytes, round_keys, aes_op, cp.Nr);
	}

	// Erase the cipher key (as mentioned above)
	for(uint8_t i = 0; i < (cp.Nk * BYTES_IN_WORD); i++){
		cipher_key[i] = 0;
	}

	return output_code;
}


void encrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, aes_op_flag aes_op, const uint8_t Nr){

	// Note: Encrypted data will overwrite original message
	uint8_t* state = data_16_bytes;

	uint8_t round_num = 0;

	add_round_key(state, round_keys, &round_num, aes_op);

	while(round_num < Nr){

		for(uint8_t i = 0; i < BYTES_IN_STATE; i++){
			state[i] = apply_sbox(state[i]);
		}

		shift_rows(state);

		mix_col_words(state);

		add_round_key(state, round_keys, &round_num, aes_op);
	}

	// Final round does not include mix_col_words
	for(uint8_t i = 0; i < BYTES_IN_STATE; i++){
		state[i] = apply_sbox(state[i]);
	}
	shift_rows(state);
	add_round_key(state, round_keys, &round_num, aes_op);

}


void decrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, aes_op_flag aes_op, const uint8_t Nr){

	uint8_t* state = data_16_bytes;

	uint8_t round_num = Nr;

	add_round_key(state, round_keys, &round_num, aes_op);

	while(round_num > 0){

		inv_shift_rows(state);

		for(uint8_t i = 0; i < BYTES_IN_STATE; i++){
			state[i] = apply_inv_sbox(state[i]);
		}

		add_round_key(state, round_keys, &round_num, aes_op);

		inv_mix_col_words(state);

	}

	// The final round (# 0) doesn't include mix_col_words
	inv_shift_rows(state);
	for(uint8_t i = 0; i < BYTES_IN_STATE; i++){
		state[i] = apply_inv_sbox(state[i]);
	}

	add_round_key(state, round_keys, &round_num, aes_op);

}


void check_invalid_input(uint8_t* data_16_bytes, uint8_t* cipher_key, aes_out* aes_out_ptr){

	if(data_16_bytes == NULL){
		aes_out_ptr->termination_code = 1;
		strcpy(aes_out_ptr->msg, "Pointer to input data is NULL.");
	}

	if(cipher_key == NULL){
		aes_out_ptr->termination_code = 2;
		strcpy(aes_out_ptr->msg, "Pointer to cipher key is NULL.");
	}

}
