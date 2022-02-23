/*
 ============================================================================
 Name        : cipher_utils.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 22, 2022
 Description : Contains functions used in the AES cipher algorithm (encryption)
               and functions used in its inversion (decryption)
 Note 1      : Keeping the state as a 1D matrix rather than transforming it into
               a 2D (4x4) matrix is intentional. Copying the state into a 2D matrix
               would make the code in the functions below more readable, but it
               would also require many copy operations (at least 1x the entire
               input dataset). If this module were used to encrypt GB of data then
               copying the whole input dataset would make the module significantly
               slower.
 ============================================================================
 */

#include "cipher_utils.h"


void add_round_key(uint8_t* state, uint8_t* round_key, uint8_t* round_num, uint8_t encrypt_or_decrypt){
	// XOR bytes of the state with bytes of the round_key
	for(uint8_t i = 0; i < 16; i++){
		state[i] ^= round_key[(16 * (*round_num)) + i];
	}

	// Every time a round key is added, the round number changes
	if (encrypt_or_decrypt == 0) (*round_num)++; // '0' for encryption
	else if (encrypt_or_decrypt == 1) (*round_num)--; // '1' for decryption
}


void shift_rows(uint8_t* state){
	// Row 0 is untouched.

	uint8_t row = 1;
	uint8_t col_0 = 0;
	// Since the state is stored as a 1D array of 16 bytes, it's necessary to keep
	// track of the conceptual "rows" and "columns". See cipher_utils.h for a
	// diagram of the state bytes and their order.
	// Shift row 1 by 1 byte to the left

	// Create a temporary byte to hold the first byte in row 1
	uint8_t temp_byte = state[col_0 + row];
	// Rather than track col with the variable, it's clearer to use an offset from zero.
	// Additionally, doing the multiplication (column * 4) is necessary because of the
	// byte indexing (column 1 row 0 is byte 4, etc.)
	state[col_0 + row] = state[(col_0 + 1) * 4 + row];
	state[(col_0 + 1) * 4 + row] = state[(col_0 + 2) * 4 + row];
	state[(col_0 + 2) * 4 + row] = state[(col_0 + 3) * 4 + row];
	state[(col_0 + 3) * 4 + row] = temp_byte;


	// Shift row 2 by 2 bytes to the left
	row = 2;
	// Store the first two bytes in row 2 in temporary variables.
	temp_byte = state[col_0 + row];

	uint8_t temp_byte2 = state[(col_0 + 1) * 4 + row];

	state[col_0 + row] = state[(col_0 + 2) * 4 + row];
	state[(col_0 + 1) * 4 + row] = state[(col_0 + 3) * 4 + row];
	state[(col_0 + 2) * 4 + row] = temp_byte;
	state[(col_0 + 3) * 4 + row] = temp_byte2;

	// Shift row 3 by 3 to the left
	row = 3;

	// In practice, shift 3 left is equivalent to shift 1 right
	temp_byte = state[(col_0 + 3) * 4 + row];
	state[(col_0 + 3) * 4 + row] = state[(col_0 + 2) * 4 + row];
	state[(col_0 + 2) * 4 + row] = state[(col_0 + 1) * 4 + row];
	state[(col_0 + 1) * 4 + row] = state[col_0 + row];
	state[col_0 + row] = temp_byte;

}


void mix_col_words(uint8_t* state){

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


void inv_shift_rows(uint8_t* state){
	// Row 0 is not to be modified.

	uint8_t row = 1;
	uint8_t col_0 = 0;
	// Since the state is stored as a 1D array of 16 bytes, it's necessary to keep
	// track of the conceptual "rows" and "columns". See cipher_utils.h for a
	// diagram of the state bytes and their order.
	// Shift row 1 by 1 byte to the right (to reverse the original operation)

	// Create a temporary byte to hold the fourth byte in row 1
	uint8_t temp_byte = state[(col_0 + 3) * 4 + row];
	// Rather than increment the col variable, it's clearer to use an offset from zero
	state[(col_0 + 3) * 4 + row] = state[(col_0 + 2) * 4 + row];
	state[(col_0 + 2) * 4 + row] = state[(col_0 + 1) * 4 + row];
	state[(col_0 + 1) * 4 + row] = state[col_0 + row];
	state[col_0 + row] = temp_byte;


	// Shift row 2 by 2 bytes to the right
	row = 2;
	// Store the first two bytes in row 2 in temporary variables.
	temp_byte = state[(col_0 + 2) * 4 + row];

	uint8_t temp_byte2 = state[(col_0 + 3) * 4 + row];

	state[(col_0 + 3) * 4 + row] = state[(col_0 + 1) * 4 + row];
	state[(col_0 + 2) * 4 + row] = state[col_0 * 4 + row];
	state[col_0 + row] = temp_byte;
	state[(col_0 + 1) * 4 + row] = temp_byte2;

	// Shift row 3 by 3 to the right
	row = 3;

	// In practice, shift 3 right is equivalent to shift 1 left
	temp_byte = state[col_0 + row];
	state[col_0 + row] = state[(col_0 + 1) * 4 + row];
	state[(col_0 + 1) * 4 + row] = state[(col_0 + 2) * 4 + row];
	state[(col_0 + 2) * 4 + row] = state[(col_0 + 3) * 4 + row];
	state[(col_0 + 3) * 4 + row] = temp_byte;
}


void inv_mix_col_words(uint8_t* state){

	uint8_t r0, r1, r2, r3;

	// Like with shift_rows, the state matrix is conceptualized as a 4x4 2D matrix
	for(uint8_t col = 0; col < 4; col++){
		// Need temp variables for every element in the column
		r0 = state[(col * 4)];
		r1 = state[(col * 4) + 1];
		r2 = state[(col * 4) + 2];
		r3 = state[(col * 4) + 3];

		// All equations shown in comments involve finite fields operations
		// row_0_out = ({0e} * row_0_in) + ({0b} * row_1_in) + ({0d} * row_2_in) + ({09} * row_3_in)
		state[(col * 4)] = mult_by_x_expansion(0x0e, r0) ^ mult_by_x_expansion(0x0b, r1) ^
				           mult_by_x_expansion(0x0d, r2) ^ mult_by_x_expansion(0x09, r3);

		// row_1_out = ({09} * row_0_in) + ({0e} * row_1_in) + ({0b} * row_2_in) + ({0d} * row_3_in)
		state[(col * 4) + 1] = mult_by_x_expansion(0x09, r0) ^ mult_by_x_expansion(0x0e, r1) ^
				               mult_by_x_expansion(0x0b, r2) ^ mult_by_x_expansion(0x0d, r3);

		// row_2_out = ({0d} * row_0_in) + ({09} * row_1_in) + ({0e} * row_2_in) + ({0b} * row_3_in)
		state[(col * 4) + 2] = mult_by_x_expansion(0x0d, r0) ^ mult_by_x_expansion(0x09, r1) ^
	               	           mult_by_x_expansion(0x0e, r2) ^ mult_by_x_expansion(0x0b, r3);

		// row_3_out = ({0b} * row_0_in) + ({0d} * row_1_in) + ({09} * row_2_in) + ({0e} * row_3_in)
		state[(col * 4) + 3] = mult_by_x_expansion(0x0b, r0) ^ mult_by_x_expansion(0x0d, r1) ^
    	           	   	       mult_by_x_expansion(0x09, r2) ^ mult_by_x_expansion(0x0e, r3);
	}

}


uint8_t mult_by_x(uint8_t byte, uint8_t num_multiplications){

	// Check if the 7th bit is a one. If yes then reduce the polynomial
	for(uint8_t i = 1; i <= num_multiplications; i++){
		// Need to check if 8th bit of input byte is 1. If yes, xor is required.
		if(byte > 127){
			byte <<= 1;
			byte ^= 0x1b;
		}
		else byte <<= 1;
	}

	return byte;
}


uint8_t mult_by_x_expansion(uint8_t expansion_hex, uint8_t byte){

	uint8_t out;
	// inv_mix_col_words only has four expansions: {09}, {0b}, {0d}, and {0e}
	if(expansion_hex == 0x09){
		// ({08} + {01}) * byte
		out = mult_by_x(byte, 3) ^ byte;
	}
	else if(expansion_hex == 0x0b){
		// ({08} + {02} + {01}) * byte
		out = mult_by_x(byte, 3) ^ mult_by_x(byte, 1) ^ byte;
	}
	else if(expansion_hex == 0x0d){
		// ({08} + {04} + {01}) * byte
		out = mult_by_x(byte, 3) ^ mult_by_x(byte, 2) ^ byte;
	}
	else{ // if expansion_hex == 0x0e
		// ({08} + {04} + {02}) * byte
		out = mult_by_x(byte, 3) ^ mult_by_x(byte, 2) ^ mult_by_x(byte, 1);
	}

	return out;
}
