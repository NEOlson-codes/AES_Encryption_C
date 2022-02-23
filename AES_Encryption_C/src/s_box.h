/*
 ============================================================================
 Name        : s_box.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 20, 2022
 Description : S-box (substitution box) is a look-up table used in transforms
 	           both during the rounds and to generate the key schedule. This
 	           header holds function declarations related to the s-box.
 ============================================================================
 */

#ifndef S_BOX_H_
#define S_BOX_H_

#include <stdint.h>

extern uint8_t s_box[16][16];
extern uint8_t inv_s_box[16][16];

/*
 * Purpose : Exchanges a byte's value with its corresponding value in the
 *   		 s-box.
 * Inputs  : 1 byte of data
 * Outputs : 1 byte from the s-box corresponding to input
 */
uint8_t apply_sbox(uint8_t byte);


/*
 * Purpose : Performs the inverse substitution of the original s-box
 * Inputs  : 1 byte of s-box lookup values from the encryption process
 * Outputs : 1 byte that was originally transformed by the s-box
 */
uint8_t apply_inv_sbox(uint8_t byte);

#endif /* S_BOX_H_ */

