/*
 ============================================================================
 Name        : aes_encryption.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : AES encryption module. Hardware independent. Can use 128,
               192, or 256 bit keys.
 ============================================================================
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "aes_encryption.h"


//  NOTE: All data blocks will be treated as bytes (rather than a mix
//  of bytes and words). This avoids having to constantly decompose
//  words to get the bytes for operations and then recreating the words.

// See details in AES_256.h
void encrypt_16_bytes(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key){

	// Set parameters related to cipher key length. (e.g. number of scrambling rounds)
	set_global_params(cipher_key_len);

	// Operations are occurring directly on the input data. This serves two purposes:
	// 1. There is no copying of the data required.
	// 2. This acts as an "erase" mechanism, making sure no trace of the original data exists
	uint8_t* state = data_16_bytes;

	uint8_t round_num = 0;

	uint8_t* round_keys = generate_key_schedule(cipher_key);

	// Input to add_round_key signaling that encryption is occurring
	uint8_t encrypt = 0;

	// Add the round key in round 0
	add_round_key(state, round_keys, &round_num, encrypt);

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
		add_round_key(state, round_keys, &round_num, encrypt);
	}

	// The final round (# Nr) doesn't include mix_col_words, but includes all other operations
	// from the while() above.
	for(uint8_t i = 0; i < 16; i++) state[i] = apply_sbox(state[i]);
	shift_rows(state);
	add_round_key(state, round_keys, &round_num, encrypt);

}


void decrypt_16_btyes(uint8_t* data_16_bytes, uint32_t cipher_key_len, uint8_t* cipher_key){

	// Set parameters related to cipher key length. (e.g. number of un-scrambling rounds)
	set_global_params(cipher_key_len);

	// The state pointed to here will likely be a copy of the encrypted data. Location is
	// determined by the hardware-specific wrapper module.
	uint8_t* state = data_16_bytes;

	uint8_t round_num = Nr; // Round starts at Nr and counts down

	// The key schedule for deciphering encrypted data is the same as the one for encryption
	// Decryption and encryption functions should be entirely independent because there may
	// be a power shutdown (where round keys are destroyed) between uses of the module.
	uint8_t* round_keys = generate_key_schedule(cipher_key);

	// Input to add_round_key signaling that decryption is occurring
	uint8_t decrypt = 1;

	// Starting in the exact opposite order of the encryption function, add round key Nr
	// to the state vector.
	add_round_key(state, round_keys, &round_num, decrypt);

	// Count down from Nr - 1 to 0
	while(round_num > 0){

		// This operation considers the 16 byte state array as a 2D array with 4 rows and
		// 4 columns. It performs shift operations on rows 2, 3, and 4.
		inv_shift_rows(state);

		// Apply the s-box to every byte in the state
		for(uint8_t i = 0; i < 16; i++) state[i] = apply_inv_sbox(state[i]);

		// Add round key and increment round_num
		add_round_key(state, round_keys, &round_num, decrypt);

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
	add_round_key(state, round_keys, &round_num, decrypt);

	//for(int i = 0; i < 16; i++) printf("0x%x ",state[i]);

}


