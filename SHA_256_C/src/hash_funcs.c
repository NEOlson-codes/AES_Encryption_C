/*
 ============================================================================
 Name        : hash_funcs.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 24, 2022
 Description : SHA-256 utility functions
 ============================================================================
 */

#include "pre_hash_funcs.h"
#include "math_funcs.h"


void fill_msg_blk_no_pad(uint32_t* msg, uint64_t blk_num, uint32_t* block_to_fill){

	// Find the index of the first word of the current message block
	uint64_t start_word_idx = blk_num * 16;

	// Fill the message block
	for (uint32_t i = 0; i < 16; i++){
		block_to_fill[i] = msg[start_word_idx + i];
	}
}


void fill_msg_blk_w_pad(uint32_t* pad_array, uint32_t pad_words_total, uint32_t* block_to_fill){

	// Fill the message block with the last 16 entries
	for (int32_t i = 15; i >= 0; i--){
		block_to_fill[i] = pad_array[(pad_words_total - 1) - i];
	}
}


void fill_blk_msg_and_pad(uint32_t* msg, uint64_t blk_num, uint64_t msg_len_words, uint32_t* block_to_fill, uint32_t* pad_array){

	uint32_t num_msg_words_in_blk = msg_len_words - (blk_num * 16);

	// Find the index of the first word of the current message block
	uint64_t start_word_idx = blk_num * 16;

	for(uint32_t i = 0; i < 16; i++){
		if(i < num_msg_words_in_blk){
			block_to_fill[i] = msg[start_word_idx + i];
		}
		else{
			block_to_fill[i] = pad_array[i - num_msg_words_in_blk];
		}
	}
}


void create_msg_schedule(uint32_t* msg_schedule_array, uint32_t* msg_block){

	// Create an alias for brevity
	uint32_t* w = msg_schedule_array;

	uint32_t cnt = 0;
	// Fill the first 16 slots of the msg schedule with the message
	while(cnt < 16){
	w[cnt] = msg_block[cnt];
	cnt++;
	}

	while(cnt < 64){
	w[cnt] = add_w_mod( add_w_mod( add_w_mod(sigma1(w[cnt - 2]), w[cnt - 7]), sigma0(w[cnt - 15])),
			w[cnt - 16]);
	cnt++;
	}
}




