/*
 ============================================================================
 Name        : pre_cipher_utils.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 21, 2022
 Description : Contains function to create the key schedule
 Note 1      : All of the operations on the state vector are byte-indexed
               (as opposed to word indexed). Otherwise, words would need to be
               decomposed in order to operate on the bytes, adding work.
 ============================================================================
 */

#include "pre_cipher_utils.h"


uint8_t* generate_key_schedule(uint8_t* cipher_key, uint8_t Nr, uint8_t Nk){

    static uint8_t key_schedule[240] = {0};

    uint8_t word_num = 0;
    uint8_t current_word_byte_zero;

    while(word_num < Nk){
    	current_word_byte_zero = word_num * BYTES_IN_WORD;

    	for(uint8_t offset = 0; offset < BYTES_IN_WORD; offset++){
			key_schedule[current_word_byte_zero + offset] = cipher_key[current_word_byte_zero + offset];
    	}
		word_num++;
	}

    uint8_t key_last_word_byte_zero = (Nk - 1) * BYTES_IN_WORD;

    uint8_t temp_word[4];
    for(uint8_t offset = 0; offset < BYTES_IN_WORD; offset++){
    	temp_word[offset] = key_schedule[key_last_word_byte_zero + offset];
    }

    uint8_t round_constant = 0;

    uint8_t key_schedule_len = BYTES_IN_STATE * (Nr + 1);
    uint8_t words_in_key_schedule = (key_schedule_len / 4);

	while(word_num < words_in_key_schedule){
		current_word_byte_zero = word_num * BYTES_IN_WORD;

		if(word_num % Nk == 0){
			uint8_t temp_byte = temp_word[0];
			temp_word[0] = temp_word[1];
			temp_word[1] = temp_word[2];
			temp_word[2] = temp_word[3];
			temp_word[3] = temp_byte;

			for(uint8_t i = 0; i < BYTES_IN_WORD; i++){
				temp_word[i] = apply_sbox(temp_word[i]);
			}

			uint8_t Nk_multiple = word_num / Nk;
			if(Nk_multiple == 1){
				round_constant = 1;
			}
			else{
				round_constant = mult_by_x(1 , Nk_multiple - 1);
			}

			// XOR temp and round constant. Bytes 2, 3, and 4 of round constant are 0.
			temp_word[0] ^= round_constant;
		}
		// One extra operation for 256-bit AES
		else if(Nk == 8 && (word_num % Nk == 4)){
			// Apply the s-box to every individual byte
			for(uint8_t i = 0; i < 4; i++){
				temp_word[i] = apply_sbox(temp_word[i]);
			}
		}

		for(uint8_t offset = 0; offset < BYTES_IN_WORD; offset++){
			temp_word[offset] ^= key_schedule[(word_num - Nk) * BYTES_IN_WORD + offset];
			key_schedule[current_word_byte_zero + offset] = temp_word[offset];
		}

		word_num++;
	}

	return key_schedule;
}


