/*
 ============================================================================
 Name        : hash_funcs.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 24, 2022
 Description : SHA-256 utility functions
 ============================================================================
 */


#ifndef HASH_FUNCS_H_
#define HASH_FUNCS_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "sha_256.h"
#include "math_funcs.h"

void fill_msg_blk_no_pad(uint32_t* msg, uint64_t blk_num, uint32_t* block_to_fill);


void fill_msg_blk_w_pad(uint32_t* pad_array, uint32_t pad_words_total, uint32_t* block_to_fill);


void fill_blk_msg_and_pad(uint32_t* msg, uint64_t blk_num, uint64_t msg_len_words, uint32_t* block_to_fill, uint32_t* pad_array);


void create_msg_schedule(uint32_t* msg_schedule_array, uint32_t* msg);


#ifdef __cplusplus
}
#endif

#endif /* HASH_FUNCS_H_ */
