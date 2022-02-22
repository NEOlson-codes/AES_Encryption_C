/*
 ============================================================================
 Name        : test_functions.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Feb 21, 2022
 Description : This file has a main loop and some test functions to validate
 	 	 	   different pieces of the AES encryption program. This file
 	 	 	   will not be used in the final implementation.
 ============================================================================
 */

#include <stdlib.h>
#include <stdint.h>
#include "s_box.h"
#include "pre_cipher_utils.h"

void test_s_box(void);
void test_global_params(void);
void test_mult_by_x(void);
void test_generate_key_schedule(void);

int main(){

//test_s_box();

//test_global_params();

//test_mult_by_x();

test_generate_key_schedule();



	return 0;
}


void test_s_box(void){
	uint8_t temp;

	temp = 0x00;
	temp = apply_sbox(temp);

	printf("%x", temp & 0xff);

}


void test_global_params(void){

set_global_params(128);

printf("Nk %d\n", Nk);
printf("Nb %d\n", Nb);
printf("Nr %d\n", Nr);

}


void test_mult_by_x(void){

	uint8_t test_byte = 0x57;
	test_byte = mult_by_x(test_byte,4);

	printf("%d",test_byte);
}

void test_generate_key_schedule(void){
	set_global_params(128);

	uint8_t cipher_key_256[32] = {
		0x60, 0x3d, 0xeb, 0x10, 0x15, 0xca, 0x71, 0xbe, 0x2b, 0x73, 0xae, 0xf0, 0x85, 0x7d, 0x77, 0x81,
		0x1f, 0x35, 0x2c, 0x07, 0x3b, 0x61, 0x08, 0xd7, 0x2d, 0x98, 0x10, 0xa3, 0x09, 0x14, 0xdf, 0xf4
	};

	uint8_t cipher_key_192[24] = {
		0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b,
		0x80, 0x90, 0x79, 0xe5, 0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b
	};

	uint8_t cipher_key_128[16] = {
		0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
	};

	uint8_t* key_ptr = &cipher_key_128;
	uint8_t* key_schedule_ptr;

	key_schedule_ptr = generate_key_schedule(key_ptr);
	// Word 8 byte 0
	printf("%d\n",key_schedule_ptr[43*4]);
	printf("%d\n",key_schedule_ptr[43*4 + 1]);
	printf("%d\n",key_schedule_ptr[43*4 + 2]);
	printf("%d\n",key_schedule_ptr[43*4 + 3]);
}
