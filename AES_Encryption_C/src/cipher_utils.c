/*
 ============================================================================
 Name        : cipher_utils.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 22, 2022
 Description : Contains functions used in the AES cipher algorithm
 ============================================================================
 */

#include "cipher_utils.h"

void add_round_key(uint8_t* state, uint8_t* round_key, uint8_t* round_num){
	// XOR bytes of the state with bytes of the round_key
	for(uint8_t i = 0; i < 16; i++){
		state[i] ^= round_key[(16 * (*round_num)) + i];
	}

	// Every time a round key is added, the round number is incremented
	(*round_num)++;
}


void shift_rows(uint8_t* state){
	// Row 0 is untouched.

	uint8_t row = 1;
	uint8_t col = 0;
	// Since the state is stored as a 1D array of 16 bytes, it's necessary to keep
	// track of the conceptual "rows" and "columns". See cipher_utils.h for a
	// diagram of the state bytes and their order.
	// Shift row 1 by 1 byte to the left

	// Create a temporary byte to hold the first byte in row 1
	uint8_t temp_byte = state[col * 4 + row];
	// Rather than track col with the variable, it's clearer to use an offset from zero
	state[col * 4 + row] = state[(col + 1) * 4 + row];
	state[(col + 1) * 4 + row] = state[(col + 2) * 4 + row];
	state[(col + 2) * 4 + row] = state[(col + 3) * 4 + row];
	state[(col + 3) * 4 + row] = temp_byte;


	// Shift row 2 by 2 bytes to the left
	row = 2;
	// Store the first two bytes in row 2 in temporary variables.
	temp_byte = state[col * 4 + row];

	uint8_t temp_byte2 = state[(col + 1) * 4 + row];

	state[col * 4 + row] = state[(col + 2) * 4 + row];
	state[(col + 1) * 4 + row] = state[(col + 3) * 4 + row];
	state[(col + 2) * 4 + row] = temp_byte;
	state[(col + 3) * 4 + row] = temp_byte2;

	// Shift row 3 by 3 to the left
	row = 3;

	// In practice, shift 3 left is equivalent to shift 1 right
	temp_byte = state[(col + 3) * 4 + row];
	state[(col + 3) * 4 + row] = state[(col + 2) * 4 + row];
	state[(col + 2) * 4 + row] = state[(col + 1) * 4 + row];
	state[(col + 1) * 4 + row] = state[col * 4 + row];
	state[col * 4 + row] = temp_byte;

}


void mix_col_words(uint8_t* state){
	// Perform finite fields math operations on the bytes in each column.
	// Simplified equations for each element given by NIST standard document (FIPS 197)

	uint8_t r0, r1, r2, r3;

	// Like with shift_rows, the state matrix is conceptualized as a 4x4 2D matrix
	for(uint8_t col = 0; col < 4; col++){
		// Need temp variables for every element in the column
		r0 = state[(col * 4)];
		r1 = state[(col * 4) + 1];
		r2 = state[(col * 4) + 2];
		r3 = state[(col * 4) + 3];

		// All equations shown in comments involve finite fields operations
		// row_0_out = ({02} * row_0_in) + ({03} * row_1_in) + row_2_in + row_3_in
		state[(col * 4)] = mult_by_x(r0, 1) ^ (r1 ^ mult_by_x(r1, 1)) ^ r2 ^ r3;

		// row_1_out = row_0_in + ({02} * row_1_in) + ({03} * row_2_in) + row_3_in
		state[(col * 4) + 1] = r0 ^ mult_by_x(r1, 1) ^ (r2 ^ mult_by_x(r2, 1)) ^ r3;

		// row_2_out = row_0_in + row_1_in + ({02} * row_2_in) + ({03} * row_3_in)
		state[(col * 4) + 2] = r0 ^ r1 ^ mult_by_x(r2, 1) ^ (r3 ^ mult_by_x(r3, 1));

		// row_3_out = ({03} * row_0_in) + row_1_in + row_2_in + ({02} * row_3_in)
		state[(col * 4) + 3] = (r0 ^ mult_by_x(r0, 1)) ^ r1 ^ r2 ^ mult_by_x(r3, 1);
	}

}

