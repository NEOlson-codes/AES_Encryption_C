
#include "sha_256.h"
#include "pre_hash_funcs.h"
#include <stdlib.h>


void test_padding(void);
void test_sha(void);


int main(){

	test_sha();



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

	uint64_t msg_len = 5;
	uint32_t message[msg_len] = {4321, 1193, 222222, 118931, 83838};

	uint32_t out_loc[16] = {0};


	uint32_t out = use_sha_256(message, msg_len, out_loc);

}

