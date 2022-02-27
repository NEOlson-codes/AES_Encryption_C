/*
 ============================================================================
 Name        : math_funcs.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 24, 2022
 Description : Mathematical functions primarily performing bitwise operations
               and bit shifting.
 Note 1      : Naming conventions for the functions are very similar to what
               appears in the FIPS 180-4 standard (for SHA-2)
 ============================================================================
 */

#include "math_funcs.h"
// Naming conventions are as shown in the NIST standard document


uint32_t add_w_mod(uint32_t num1, uint32_t num2){
	// Addition may overflow. Set temp to >32 bits
	uint64_t temp;
	temp = num1 + num2;
	// If the result of the addition is > 2^32
	if(temp >> 32 == 1) temp -= 0x0000000100000000;
	temp = (uint32_t) temp;

	return temp;
}

uint32_t ch_xor(uint32_t x, uint32_t y, uint32_t z){
	uint32_t out = (x & y) ^ ((~x) & z);
	return out;
}


uint32_t maj_xor(uint32_t x, uint32_t y, uint32_t z){
	uint32_t out = (x & y) ^ (x & z) ^ (y & z);
	return out;
}


uint32_t sum0(uint32_t x){
	uint32_t out = rot_r(x, 2) ^ rot_r(x, 13) ^ rot_l(x, 10);
	return out;
}

uint32_t sigma0(uint32_t x){
	uint32_t out = rot_r(x, 7) ^ rot_l(x, 14) ^ (x >> 3);
	return out;
}

uint32_t sum1(uint32_t x){
	uint32_t out = rot_r(x, 6) ^ rot_r(x, 11) ^ rot_l(x, 7);
	return out;
}

uint32_t sigma1(uint32_t x){
	uint32_t out = rot_l(x, 15) ^ rot_l(x, 13) ^ (x >> 10);
	return out;
}


// Circular rotation left
uint32_t rot_l(uint32_t x, uint32_t shift_n){
	uint32_t temp = x << shift_n;
	x >>= 32 - shift_n;
	x |= temp;

	return x;
}

// Circular rotation right
uint32_t rot_r(uint32_t x, uint32_t shift_n){
	uint32_t temp = x >> shift_n;
	x <<= 32 - shift_n;
	x |= temp;

	return x;
}
