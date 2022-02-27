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

#include <stdint.h>
#include "pre_hash_funcs.h"
#include "math_funcs.h"


uint32_t use_sha_256(uint32_t* msg, uint64_t msg_len_words);

uint32_t error_handler();



#endif /* SHA_256_H_ */
