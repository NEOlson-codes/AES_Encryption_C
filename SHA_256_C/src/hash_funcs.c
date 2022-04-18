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


// Fill the message block entirely with words from the original message
void fill_msg_blk_no_pad(uint32_t* msg, uint64_t blk_num, uint32_t* block_to_fill){

	// Find the index of the first word of the current message block
	uint64_t start_word_idx = blk_num * MSG_BLOCK_LEN;

	// Fill the message block
	for (uint32_t i = 0; i < MSG_BLOCK_LEN; i++){
		block_to_fill[i] = msg[start_word_idx + i];
	}
}


// Fill the message block entirely with words from the end of the zero pad
void fill_msg_blk_w_pad(uint32_t* pad_array, uint32_t pad_words_total, uint32_t* block_to_fill){

	// Words from the pad used in the last block
	uint32_t start_idx = pad_words_total - MSG_BLOCK_LEN;

	// Fill the message block with the last 16 entries
	for (int32_t i = 0; i < MSG_BLOCK_LEN; i++){
		block_to_fill[i] = pad_array[start_idx + i];
	}
}


// Fill the message block with words from the end of the message and beginning
// of the zero pad
void fill_blk_msg_and_pad(uint32_t* msg, uint64_t blk_num, uint64_t msg_len_words, uint32_t* block_to_fill, uint32_t* pad_array){

	uint32_t num_msg_words_in_blk = msg_len_words - (blk_num * MSG_BLOCK_LEN);

	uint64_t start_word_idx = blk_num * MSG_BLOCK_LEN;

	for(uint32_t i = 0; i < MSG_BLOCK_LEN; i++){
		if(i < num_msg_words_in_blk){
			block_to_fill[i] = msg[start_word_idx + i];
		}
		else{
			block_to_fill[i] = pad_array[i - num_msg_words_in_blk];
		}
	}
}


void create_msg_schedule(uint32_t* msg_schedule_array, uint32_t* msg_block){

	uint32_t* sch = msg_schedule_array;

	uint32_t cnt = 0;
	// Fill the first 16 slots of the msg schedule with the message
	while(cnt < MSG_BLOCK_LEN){
		sch[cnt] = msg_block[cnt];
		cnt++;
	}

	while(cnt < MSG_SCHED_LEN){
		sch[cnt] = add_w_mod( add_w_mod( add_w_mod(sigma1(sch[cnt - 2]), sch[cnt - 7]),
				sigma0(sch[cnt - 15])), sch[cnt - 16]);
		cnt++;
	}
}




