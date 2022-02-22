/*
 ============================================================================
 Name        : s_box.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : S-box (substitution box) is a look-up table used in transforms
 	           both during the rounds and to generate the key schedule. This
 	           header holds the s-box, a 2D array of byte values. Several
 	           functions related to the s-box are also declared.
 ============================================================================
 */

#ifndef S_BOX_H_
#define S_BOX_H_

#include <stdint.h>

extern uint8_t s_box[16][16];

/*
 * Purpose : Exchanges a byte's value with its corresponding value in the
 *   		 s-box.
 * Inputs  : 1 byte of data
 * Outputs : 1 byte from the s-box corresponding to input
 */
uint8_t apply_sbox(uint8_t byte);


#endif /* S_BOX_H_ */

