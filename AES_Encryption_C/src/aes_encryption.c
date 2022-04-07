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

aes_out use_aes(uint8_t* data_16_bytes, cipher_len cipher_key_len, uint8_t* cipher_key, aes_op_flag aes_op){

	aes_out output_code = {
		.termination_code = 0,
		.msg = "Encrypt/decrypt successful"
	};

	check_invalid_input(data_16_bytes, cipher_key_len, cipher_key, aes_op, &output_code);

	if (output_code.termination_code != 0){
		return output_code;
	}

	// Set parameters related to cipher key length (e.g. number of scrambling rounds).
	const uint8_t Nk = set_algo_params(cipher_key_len, Nk_);
	const uint8_t Nr = set_algo_params(cipher_key_len, Nr_);

	uint8_t* round_keys = generate_key_schedule(cipher_key, Nr, Nk);

	if(aes_op == encrypt){
		encrypt_16_bytes(data_16_bytes, round_keys, aes_op, Nr);
	}
	else if(aes_op == decrypt){
		decrypt_16_bytes(data_16_bytes, round_keys, aes_op, Nr);
	}

	// Erase the cipher key (as mentioned above)
	for(uint8_t i = 0; i < (Nk * BYTES_IN_WORD); i++){
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

		for(uint8_t i = 0; i < BYTES_IN_STATE; i++) state[i] = apply_sbox(state[i]);

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

	//for(int i = 0; i < 16; i++) printf("0x%x ",state[i]);

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


void check_invalid_input(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key, aes_op_flag aes_op, aes_out* aes_out_ptr){

	if(data_16_bytes == NULL){
		aes_out_ptr->termination_code = 1;
		strcpy(aes_out_ptr->msg, "Pointer to input data is NULL.");
	}

	if((cipher_key_len != 128) && (cipher_key_len != 192) && (cipher_key_len != 256)){
		aes_out_ptr->termination_code = 2;
		strcpy(aes_out_ptr->msg, "Cipher key has an invalid length (not 128, 192, 256).");
	}

	if(cipher_key == NULL){
		aes_out_ptr->termination_code = 3;
		strcpy(aes_out_ptr->msg, "Pointer to cipher key is NULL.");
	}

	if((aes_op != encrypt) && (aes_op != decrypt)){
		aes_out_ptr->termination_code = 4;
		strcpy(aes_out_ptr->msg, "Specified operation is neither decryption nor encryption.");
	}
}
