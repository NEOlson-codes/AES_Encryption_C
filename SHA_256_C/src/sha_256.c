/*
 ============================================================================
 Name        : sha_256.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 24, 2022
 Description : SHA-256 hashing module. Hardware independent.
 Note 1      : This module requires that message lengths be a multiple of 32 bits
 Note 2      : At max, there could be 2^55 blocks of 512 bits. That's why any value
               related to the message length uses a uint64_t.
 Note 3      : This module is meant to be used with a hardware or platform specific
               wrapper function.
 ============================================================================
 */

#include "sha_256.h"
#define EXIT_SUCCESS  0


uint32_t use_sha_256(uint32_t* msg, uint64_t msg_len_words, uint32_t* output_loc){

	uint32_t error_code = error_handler(msg, msg_len_words, output_loc);
	if(error_code != 0) return error_code;

	uint32_t* pad_array;

	uint32_t zero_pad_len = MSG_BLOCK_LEN - (msg_len_words % MSG_BLOCK_LEN);

	// Pad must be at least 3 words, or it wraps to incorporate another block
	if (zero_pad_len < 3){
		zero_pad_len = zero_pad_len + MSG_BLOCK_LEN;
	}

	pad_array = pad_msg(zero_pad_len, msg_len_words);


	// Initial hash values defined in NIST FIPS 180-4 (SHA standard)
	uint32_t hash_vals[NUM_TEMP_HASHES] = {

		0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
	};

	// Temporary variables with NIST naming convention
	uint32_t a = hash_vals[0];
	uint32_t b = hash_vals[1];
	uint32_t c = hash_vals[2];
	uint32_t d = hash_vals[3];
	uint32_t e = hash_vals[4];
	uint32_t f = hash_vals[5];
	uint32_t g = hash_vals[6];
	uint32_t h = hash_vals[7];
	uint32_t t1;
	uint32_t t2;

	uint64_t total_len_words = msg_len_words + zero_pad_len;

	uint64_t total_blocks = total_len_words / MSG_BLOCK_LEN;

	uint32_t msg_schedule_array[MSG_SCHED_LEN];

	uint32_t msg_block[MSG_BLOCK_LEN] = {0};

	uint64_t zero_pad_start_blk;
	(zero_pad_len <= MSG_BLOCK_LEN) ? (zero_pad_start_blk = total_blocks - 1) : (zero_pad_start_blk = total_blocks - 2);


	// Begin the rounds of compression, 1 iteration for each 512 bit message block
	for(uint64_t blk_num = 0; blk_num < total_blocks; blk_num++){

		// Create the message block.
		if (blk_num >= zero_pad_start_blk){
			if (blk_num == zero_pad_start_blk){
				// This block will be the end of the message and the first words (perhaps only words) of the pad
				fill_blk_msg_and_pad(msg, blk_num, msg_len_words, msg_block, pad_array);
			}
			else{
				// This is only used if the zero pad takes up more than a full 16-word block
				fill_msg_blk_w_pad(pad_array, zero_pad_len, msg_block);
			}
		}
		// Fill the 512-bit block only with message bits (this will be what happens most of the time)
		else{
			fill_msg_blk_no_pad(msg, blk_num, msg_block);
		}


		create_msg_schedule(msg_schedule_array, msg_block);

		// Operations on working variables as defined by NIST FIPS 180-4 (SHA standard)
		for (uint32_t i = 0; i < MSG_SCHED_LEN; i++){

			t1 = add_w_mod( add_w_mod( add_w_mod( add_w_mod( h, sum1(e)) , ch_xor(e, f, g)), k_vals[i]),
					msg_schedule_array[i]);

			t2 = add_w_mod(sum0(a), maj_xor(a,b,c));
			h = g;
			g = f;
			f = e;

			e = add_w_mod(d, t1);
			d = c;
			c = b;
			b = a;
			a = add_w_mod(t1, t2);
		}

		hash_vals[0] = add_w_mod(a, hash_vals[0]);
		// Reset working variables for the next loop iteration (next msg block)
		a = hash_vals[0];

		hash_vals[1] = add_w_mod(b, hash_vals[1]);
		b = hash_vals[1];

		hash_vals[2] = add_w_mod(c, hash_vals[2]);
		c = hash_vals[2];

		hash_vals[3] = add_w_mod(d, hash_vals[3]);
		d= hash_vals[3];

		hash_vals[4] = add_w_mod(e, hash_vals[4]);
		e = hash_vals[4];

		hash_vals[5] = add_w_mod(f, hash_vals[5]);
		f = hash_vals[5];

		hash_vals[6] = add_w_mod(g, hash_vals[6]);
		g = hash_vals[6];

		hash_vals[7] = add_w_mod(h, hash_vals[7]);
		h = hash_vals[7];
	}


for (uint32_t i = 0; i < NUM_TEMP_HASHES; i++){

	output_loc[i] = hash_vals[i];
}


	return EXIT_SUCCESS;
}



uint32_t error_handler(uint32_t* msg, uint64_t msg_len_words, uint32_t* output_loc){

	if(msg == NULL) return MSG_ARRAY_NULL_PTR_ERR;

	if(msg_len_words == 0) return MSG_LEN_ZERO_ERR;

	if(output_loc == NULL) return OUTPUT_LOC_NULL_PTR_ERR;

	return 0;
}


