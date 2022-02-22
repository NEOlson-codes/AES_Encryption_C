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
/*
void encrypt_block_128(int8_t* data_128_bits, uint32_t cipher_key_len, uint8_t* cipher_key){




	int8_t state[4][4];
	// Convert data block into state block format
    for(int byte = 0; byte < 16; byte++){

    	// Column is byte / 4 (truncates due to int division)
    	// Row is byte % 4
    	state[byte % 4][byte / 4] = data_128_bits[byte];
    }



	// Copy input data into state

	// Add round key


}

void decrypt_block_128(){



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



