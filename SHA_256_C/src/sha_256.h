/*
 ============================================================================
 Name        : sha_256.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 24, 2022
 Description : SHA-256 hashing module. Hardware independent.

 ============================================================================
 */

#ifndef SHA_256_H_
#define SHA_256_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdlib.h>
#include <stdint.h>
#include "pre_hash_funcs.h"
#include "math_funcs.h"
#include "hash_funcs.h"

extern const uint32_t k_vals[];

#define MSG_BLOCK_LEN     16
#define NUM_TEMP_HASHES   8
#define MSG_SCHED_LEN     64
#define ZERO_PAD_MAX_LEN  18
#define BITS_IN_WORD      32

#define MSG_ARRAY_NULL_PTR_ERR   1
#define MSG_LEN_ZERO_ERR         2
#define OUTPUT_LOC_NULL_PTR_ERR  3

uint32_t use_sha_256(uint32_t* msg, uint64_t msg_len_words, uint32_t* output_loc);

uint32_t error_handler();


#ifdef __cplusplus
}
#endif

#endif /* SHA_256_H_ */
