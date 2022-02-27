
#include "sha_256.h"
#include "pre_hash_funcs.h"
#include <stdlib.h>
#include "math_funcs.h"


void test_padding(void);
void test_sha(void);
void test_math(void);
void test_k(void);

int main(){

	// test_padding();
	test_sha();

	//test_math();
	//test_k();

	return 0;
}

void test_padding(void){

	uint64_t msg_len = 13;

	uint64_t pad_amt = 16 - (msg_len % 16);
	//pad_amt = 18;
	uint32_t* pad_array = pad_msg(pad_amt, msg_len);

	printf("Pad Length = %d\n", pad_amt);

	for (int i =0; i < pad_amt; i++){
		printf("0x%x ", pad_array[i]);
	}

	printf("\n%x",msg_len * 32);

}


void test_sha(void){

	uint64_t msg_len = 1;
	uint32_t message[1] = {0x61616161};

	uint32_t out_loc[8] = {0};


	uint32_t out = use_sha_256(message, msg_len, out_loc);

	for(uint32_t i = 0; i < 8; i++ ){
		printf("0x%x ",out_loc[i]);
	}
}


void test_math(void){

	// should be 10100100 (hand calc)
	// uint32_t out = maj_xor(0x11100000, 0x10110111, 0x10000100);

	//
	// uint32_t out = ch_xor(
	uint32_t out = rot_r(0x1100010010, 20);
	uint32_t out2 = rot_l(0x1100010010, 12);


	printf("%x\n",out);
	printf("%x", out2);
}


void test_k(void){
	printf("%x", k_vals[63]);

}
