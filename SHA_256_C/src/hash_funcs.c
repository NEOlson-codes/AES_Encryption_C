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


void fill_current_block(uint64_t total_blocks, uint64_t current_blk_num,
		uint32_t* current_blk_ptr, uint32_t num_pad_words, uint32_t pad_2_blocks){

	// The number of message words to fill into the last block
	uint32_t msg_words;
	// What to do on the last block
	if((pad_2_blocks == 0) && (current_blk_num == total_blocks - 1)){
		msg_words = 16 - num_pad_words;
		uint64_t msg_word_num = current_blk_num * 16;
		for(uint32_t i = 0; i < msg_words; i++){
			current_blk_prt[i] =
		}
	}
	else if((pad_2_blocks == 1) && (current_blk_num == total_blocks - 2)){


	}
	else if ((pad_2_blocks == 1) && (current_blk_num == total_blocks - 1)){


	}




}


void fill_last_2_blocks(){

}




void create_msg_schedule(uint32_t* msg_schedule_array, uint32_t* msg){

	// Create an alias for brevity
	uint32_t* w = msg_schedule_array;

	uint32_t cnt = 0;
	// Fill the first 16 slots of the msg schedule with the message
	while(cnt < 16){
	w[cnt] = msg[cnt];
	cnt++;
	}

	while(cnt < 63){
	w[cnt] = sigma1(w[cnt - 2]) + w[cnt - 7] + sigma0(w[cnt - 15]) + w[cnt - 16];
	cnt++;
	}
}





	// Iterates over the words in the msg block and constructs the words from the message
	// bytes and the padding.
	if((current_block_num != total_blocks - 1) && (current_block_num != total_blocks - 2)){
		for(uint32_t word_num; word_num < 16; word_num++){
			msg_block[word_num] =
		}

	}






	}



		if (message_padding.num_bytes_padding == 0){
			uint32_t no_padding = 1;
		}



	// Append the last bytes of the message

	// The padding is only going to come in the last or last two blocks
	if((current_block_num == total_blocks - 1) || (current_block_num == total_blocks - 2)){
		if (message_padding.num_bytes_padding > 64)


	}


}




void update_msg_schedule(uint32_t* msg_schedule){

	// Create the 64 word message schedule
	// The first 16 words come from the message block
	for(uint32_t i = 0; i < 16; i++){

	}



}


*/
