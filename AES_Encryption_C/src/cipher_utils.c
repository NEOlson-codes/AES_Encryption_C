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

// Translate 2D state matrix row/col to index of flat 16-bit array
uint8_t get_idx_rc[ROWS_IN_STATE][COLS_IN_STATE] = {
	{0, 4, 8, 12},
	{1, 5, 9, 13},
	{2, 6, 10, 14},
	{3, 7, 11, 15}
};

void add_round_key(uint8_t* state, uint8_t* round_key, uint8_t* round_num, uint8_t is_decrypt){
	for(uint8_t i = 0; i < BYTES_IN_STATE; i++){
		state[i] ^= round_key[(BYTES_IN_STATE * (*round_num)) + i];
	}

	if (is_decrypt == 0){
		(*round_num)++; // '0' for encryption
	}
	else if (is_decrypt == 1){
		(*round_num)--; // '1' for decryption
	}
}


void shift_rows(uint8_t* state){
	// Row 0 is untouched.

	// Shift elements in row 1 one spot to the left
	uint8_t temp_byte = state[get_idx_rc[1][0]];

	state[get_idx_rc[1][0]] = state[get_idx_rc[1][1]];
	state[get_idx_rc[1][1]] = state[get_idx_rc[1][2]];
	state[get_idx_rc[1][2]] = state[get_idx_rc[1][3]];
	state[get_idx_rc[1][3]] = temp_byte;


	// Shift elements in row 2 two spots to the left
	temp_byte = state[get_idx_rc[2][0]];

	uint8_t temp_byte2 = state[get_idx_rc[2][1]];

	state[get_idx_rc[2][0]] = state[get_idx_rc[2][2]];
	state[get_idx_rc[2][1]] = state[get_idx_rc[2][3]];
	state[get_idx_rc[2][2]] = temp_byte;
	state[get_idx_rc[2][3]] = temp_byte2;

	// Shift elements in row 3 one spot to the right (eq. 3 spots to the left)
	temp_byte = state[get_idx_rc[3][3]];
	state[get_idx_rc[3][3]] = state[get_idx_rc[3][2]];
	state[get_idx_rc[3][2]] = state[get_idx_rc[3][1]];
	state[get_idx_rc[3][1]] = state[get_idx_rc[3][0]];
	state[get_idx_rc[3][0]] = temp_byte;

}


void mix_col_words(uint8_t* state){

	uint8_t r0, r1, r2, r3;

	for(uint8_t col = 0; col < COLS_IN_STATE; col++){

		r0 = state[get_idx_rc[0][col]];
		r1 = state[get_idx_rc[1][col]];
		r2 = state[get_idx_rc[2][col]];
		r3 = state[get_idx_rc[3][col]];

		// All equations shown in comments are finite fields operations
		// r0 = ({02} * r0) + ({03} * r1) + r2 + r3
		state[get_idx_rc[0][col]] = mult_by_x(r0, 1) ^ (r1 ^ mult_by_x(r1, 1)) ^ r2 ^ r3;

		// r1 = r0 + ({02} * r1) + ({03} * r2) + r3
		state[get_idx_rc[1][col]] = r0 ^ mult_by_x(r1, 1) ^ (r2 ^ mult_by_x(r2, 1)) ^ r3;

		// r2 = r0 + r1 + ({02} * r2) + ({03} * r3)
		state[get_idx_rc[2][col]] = r0 ^ r1 ^ mult_by_x(r2, 1) ^ (r3 ^ mult_by_x(r3, 1));

		// r3 = ({03} * r0) + r1 + r2 + ({02} * r3)
		state[get_idx_rc[3][col]] = (r0 ^ mult_by_x(r0, 1)) ^ r1 ^ r2 ^ mult_by_x(r3, 1);
	}

}


void inv_shift_rows(uint8_t* state){
	// Row 0 is not to be modified.

	// Shift elements in row 1 one spot to the right
	uint8_t temp_byte = state[get_idx_rc[1][3]];

	state[get_idx_rc[1][3]] = state[get_idx_rc[1][2]];
	state[get_idx_rc[1][2]] = state[get_idx_rc[1][1]];
	state[get_idx_rc[1][1]] = state[get_idx_rc[1][0]];
	state[get_idx_rc[1][0]] = temp_byte;

	// Shift elements in row 2 two spots to the right
	temp_byte = state[get_idx_rc[2][2]];

	uint8_t temp_byte2 = state[get_idx_rc[2][3]];

	state[get_idx_rc[2][3]] = state[get_idx_rc[2][1]];
	state[get_idx_rc[2][2]] = state[get_idx_rc[2][0]];
	state[get_idx_rc[2][0]] = temp_byte;
	state[get_idx_rc[2][1]] = temp_byte2;

	// Shift elements in row 3 one spot to the left (eq. 3 to the right)
	temp_byte = state[get_idx_rc[3][0]];
	state[get_idx_rc[3][0]] = state[get_idx_rc[3][1]];
	state[get_idx_rc[3][1]] = state[get_idx_rc[3][2]];
	state[get_idx_rc[3][2]] = state[get_idx_rc[3][3]];
	state[get_idx_rc[3][3]] = temp_byte;
}


void inv_mix_col_words(uint8_t* state){

	uint8_t r0, r1, r2, r3;

	// Like with shift_rows, the state matrix is conceptualized as a 4x4 2D matrix
	for(uint8_t col = 0; col < COLS_IN_STATE; col++){

		r0 = state[get_idx_rc[0][col]];
		r1 = state[get_idx_rc[1][col]];
		r2 = state[get_idx_rc[2][col]];
		r3 = state[get_idx_rc[3][col]];

		// All equations shown in comments are finite fields operations
		// r0 = ({0e} * r0) + ({0b} * r1) + ({0d} * r2) + ({09} * r3)
		state[get_idx_rc[0][col]] = mult_by_x_expansion(0x0e, r0) ^ mult_by_x_expansion(0x0b, r1) ^
				           mult_by_x_expansion(0x0d, r2) ^ mult_by_x_expansion(0x09, r3);

		// r1 = ({09} * r0) + ({0e} * r1) + ({0b} * r2) + ({0d} * r3)
		state[get_idx_rc[1][col]] = mult_by_x_expansion(0x09, r0) ^ mult_by_x_expansion(0x0e, r1) ^
				               mult_by_x_expansion(0x0b, r2) ^ mult_by_x_expansion(0x0d, r3);

		// r2 = ({0d} * r0) + ({09} * r1) + ({0e} * r2) + ({0b} * r3)
		state[get_idx_rc[2][col]] = mult_by_x_expansion(0x0d, r0) ^ mult_by_x_expansion(0x09, r1) ^
	               	           mult_by_x_expansion(0x0e, r2) ^ mult_by_x_expansion(0x0b, r3);

		// r3 = ({0b} * r0) + ({0d} * r1) + ({09} * r2) + ({0e} * r3)
		state[get_idx_rc[3][col]] = mult_by_x_expansion(0x0b, r0) ^ mult_by_x_expansion(0x0d, r1) ^
    	           	   	       mult_by_x_expansion(0x09, r2) ^ mult_by_x_expansion(0x0e, r3);
	}

}


uint8_t mult_by_x(uint8_t byte, uint8_t num_multiplications){

	// Check if the 7th bit is a one. If yes then reduce the polynomial
	for(uint8_t i = 1; i <= num_multiplications; i++){
		// Need to check if 8th bit of input byte is 1. If yes, xor.
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
	else if(expansion_hex == 0x0e){
		// ({08} + {04} + {02}) * byte
		out = mult_by_x(byte, 3) ^ mult_by_x(byte, 2) ^ mult_by_x(byte, 1);
	}

	return out;
}
