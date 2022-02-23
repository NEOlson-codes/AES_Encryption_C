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
#define  EXIT_SUCCESS  0


// Details for all functions can be found in aes_encryption.h

uint32_t use_aes(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key, uint32_t encrypt_or_decrypt){

	// A successful run of the algorithm will result in use_aes() returning zero. Otherwise,
	// it will prematurely return (not having done anything) with a non-zero error code.
	// Error checking is primarily for null pointers and invalid numeric inputs.
	uint32_t error_code = error_handler(data_16_bytes, cipher_key_len, cipher_key, encrypt_or_decrypt);

	// If there is a non-zero error code, terminate the main function prematurely and return the error.
	if (error_code) return error_code;

	// Set parameters related to cipher key length (e.g. number of scrambling rounds).
	// See full descriptions in pre_cipher_utils.h
	const uint8_t Nk = set_algo_params(cipher_key_len, Nk_);
	const uint8_t Nr = set_algo_params(cipher_key_len, Nr_);

	// Generate round keys, which are the same for both encryption and decryption.
	const uint8_t* round_keys = generate_key_schedule(cipher_key, Nr, Nk);

	// Call main encryption or decryption function based on encrypt_or_decrypt flag
	if(encrypt_or_decrypt == 0){
		encrypt_16_bytes(data_16_bytes, round_keys, encrypt_or_decrypt, Nr);
	}
	else{
		decrypt_16_bytes(data_16_bytes, round_keys, encrypt_or_decrypt, Nr);
	}

	// Erase the cipher key (as mentioned above)
	for(uint8_t i = 0; i < 16; i++) cipher_key[i] = 0;


	return EXIT_SUCCESS;
}


void encrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, uint32_t encrypt_or_decrypt, const uint8_t Nr){

	// Operations are occurring directly on the input data. This serves two purposes:
	// 1. No copying of the data is required (which could cause significant overhead if
	//    encrypting a significant amount of data).
	// 2. This acts as an "erase" mechanism, making sure no trace of the original data exists
	uint8_t* state = data_16_bytes;

	uint8_t round_num = 0;

	// Add the round key in round 0
	add_round_key(state, round_keys, &round_num, encrypt_or_decrypt);

	// Nr is the number of rounds. The last round is different and is handled separately
	while(round_num < Nr){
		// Apply the s-box to every byte in the state
		for(uint8_t i = 0; i < 16; i++) state[i] = apply_sbox(state[i]);

		// This operation considers the 16 byte state array as a 2D array with 4 rows and
		// 4 columns. It performs shift operations on rows 2, 3, and 4.
		shift_rows(state);

		// This function takes each column of the state vector and applies a different
		// finite field math formula (consisting of the other column elements) to it
		// as a transform.
		mix_col_words(state);

		// Add round key and increment round_num
		add_round_key(state, round_keys, &round_num, encrypt_or_decrypt);
	}

	// The final round (# Nr) doesn't include mix_col_words, but includes all other operations
	// from the while() above.
	for(uint8_t i = 0; i < 16; i++) state[i] = apply_sbox(state[i]);
	shift_rows(state);
	add_round_key(state, round_keys, &round_num, encrypt_or_decrypt);

}


void decrypt_16_bytes(uint8_t* data_16_bytes, uint8_t* round_keys, uint32_t encrypt_or_decrypt, const uint8_t Nr){

	// The state pointed to here will likely be a copy of the encrypted data (rather than
	// the copy stored in non-volatile memory). Location is determined by the
	// hardware-specific wrapper module.
	uint8_t* state = data_16_bytes;

	// Round starts at Nr and counts down
	uint8_t round_num = Nr;

	// Starting in the exact opposite order of the encryption function, add round key Nr
	// to the state vector.
	add_round_key(state, round_keys, &round_num, encrypt_or_decrypt);

	// Count down from Nr - 1 to 0
	while(round_num > 0){

		// This operation considers the 16 byte state array as a 2D array with 4 rows and
		// 4 columns. It performs shift operations on rows 2, 3, and 4.
		inv_shift_rows(state);

		// Apply the s-box to every byte in the state
		for(uint8_t i = 0; i < 16; i++) state[i] = apply_inv_sbox(state[i]);

		// Add round key and increment round_num
		add_round_key(state, round_keys, &round_num, encrypt_or_decrypt);

		// This function takes each column of the state vector and applies a different
		// finite field math formula (consisting of the other column elements) to
		// reverse the effects of the original (encryption) mix_col_words() function.
		inv_mix_col_words(state);

	}

	// The final round (# 0) doesn't include mix_col_words, but includes all other operations
	// from the while() above.
	inv_shift_rows(state);
	for(uint8_t i = 0; i < 16; i++) state[i] = apply_inv_sbox(state[i]);
	// Since round_num is unsigned, it will wrap when decremented at the end of add_round_key. However,
	// that doesn't matter since the variable isn't used again.
	add_round_key(state, round_keys, &round_num, encrypt_or_decrypt);

}


uint32_t error_handler(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key, uint32_t encrypt_or_decrypt){

	// Error code numbers go in order of the inputs
	// Error code '1': pointer to 16 byte input is a null pointer
	if(data_16_bytes == NULL) return 1;

	// Error code '2': cipher key has an invalid length (not 128, 192, 256)
	if((cipher_key_len != 128) && (cipher_key_len != 192) && (cipher_key_len != 256)) return 2;

	// Error code '3': pointer to cipher key is a null pointer
	if(cipher_key == NULL) return 3;

	// Error code '4': encrypt_or_decrypt does not specify '0' (encryption) or '1' (decryption)
	if((encrypt_or_decrypt != 0) && (encrypt_or_decrypt != 1)) return 4;

	// If no errors are generated, return zero
	return 0;
}
