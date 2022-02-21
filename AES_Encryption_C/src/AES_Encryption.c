/*
 ============================================================================
 Name        : AES_256.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : AES 256-bit encryption module. Hardware independent.
 ============================================================================
 */

#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "AES_256.h"

// See details in AES_256.h
void encrypt_block_128(char* data_128_bits, uint32_t cipher_key){

	char state[4][4];
	// Convert data block into state block format
    for(int byte = 0; byte < 16; byte++){

    	// Column is byte / 4 (truncates due to int division)
    	// Row is byte % 4
    	state[byte % 4][byte / 4] = data_128_bits[byte];
    }



	// Copy input data into state

	// Add round key


}

int* decrypt_block_128(){



}


uint32_t* generate_key_schedule(uint32_t* cipher_key){

	/*
	 * The 4 words from the schedule key are XOR'ed with the state matrix
	 * once at the beginning of the cipher process and during each of the
	 * 14 rounds.
	 */
	uint32_t key_schedule_len = 4 * (14 + 1);

    static uint32_t key_schedule[key_schedule_len];

    uint32_t word = 0;
	// The 8 words of the cipher are the first 8 words of the schedule key
    while(word < 8){
		key_schedule[word] = cipher_key[word];
		word++;
	}

    // word # is 8.
	while(word < key_schedule_len){
		uint32_t temp = key_schedule[word - 1];
		uint32_t temp2 = 0;

		// Every 8th word has a special operation
		if(word % 8 == 0){
			// The word has following transformation:
			// [b0, b1, b2, b3] -> [b1, b2, b3, b0]
			// Note: transformations are in-line for efficiency
			temp2 = temp;
			temp2 <<= 24; // Isolate byte 0
			temp >>= 8; // Get rid of byte 0, number is unsigned so MSBs should be 0
			temp ^= temp2; //XOR to put original byte 0 at the end of the word




		}


	}



}





int* cipher_round(int){
	// if(not_last)
		// 1 Apply S-box substitutions
		// 2 Shift rows of state by different offsets
		// 3 Mix the data within each column
		// 4 Adding a round key to the state
	// if(last)
		// Normal round without mixing the columns
}



// Create S-Box

//

