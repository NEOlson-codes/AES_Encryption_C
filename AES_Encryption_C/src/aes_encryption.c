/*
 ============================================================================
 Name        : AES_Encryption.c
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
#include "AES_256.h"


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

	// Add the round key in round 0
	add_round_key(state, round_keys, &round_num);

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
		add_round_key(state, round_keys, &round_num);
	}

	// The final round (# Nr) doesn't include mix_col_words, but includes all other operations
	// from the while() above.
	for(uint8_t i = 0; i < 16; i++) state[i] = apply_sbox(state[i]);
	shift_rows(state);
	add_round_key(state, round_keys, &round_num);

	// DIAGNOSTIC LINE. NOT IN FINAL RELEASE
	printf("End of round val: ");
	for(int i = 0; i < 16; i++) printf("0x%x ",state[i]);
	printf("\n");


}

/*
void decrypt_16_btyes(void){



}





uint32_t* cipher_round(uint32_t){
	// if(not_last)
		// 1 Apply S-box substitutions
		// 2 Shift rows of state by different offsets
		// 3 Mix the data within each column
		// 4 Adding a round key to the state
	// if(last)
		// Normal round without mixing the columns
}

*/



