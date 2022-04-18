/*
 ============================================================================
 Name        : math_funcs.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 24, 2022
 Description : Mathematical functions primarily performing bitwise operations
               and bit shifting.
 ============================================================================
 */


#ifndef MATH_FUNCS_H_
#define MATH_FUNCS_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include <stdint.h>
#include <math.h>

uint32_t add_w_mod(uint32_t num1, uint32_t num2);

uint32_t ch_xor(uint32_t x, uint32_t y, uint32_t z);

uint32_t maj_xor(uint32_t x, uint32_t y, uint32_t z);

uint32_t sum0(uint32_t x);

uint32_t sigma0(uint32_t x);

uint32_t sum1(uint32_t x);

uint32_t sigma1(uint32_t x);

uint32_t rot_l(uint32_t x, uint32_t shift_n);

uint32_t rot_r(uint32_t x, uint32_t shift_n);


#ifdef __cplusplus
}
#endif

#endif /* MATH_FUNCS_H_ */
