/*
 ============================================================================
 Name        : pre_hash_funcs.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 24, 2022
 Description : Contains the pad generator function and the K constants used
               to set the temporary variables between compressions.
 ============================================================================
 */


#ifndef PRE_HASH_FUNCS_H_
#define PRE_HASH_FUNCS_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "sha_256.h"
#include <stdint.h>

uint32_t* pad_msg(uint64_t words_of_padding, uint64_t msg_len_words);


#ifdef __cplusplus
}
#endif

#endif /* PRE_HASH_FUNCS_H_ */
