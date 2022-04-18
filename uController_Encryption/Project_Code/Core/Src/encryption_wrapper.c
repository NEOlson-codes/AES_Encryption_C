/*
 ============================================================================
 Name        : encryption_wrapper.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Apr 5, 2022
 Description : Hardware-specific wrapper for running SHA-256 and AES-256 
               together on the STM32. Message and password limits are 
							 set according to memory constraints. Encryption or decryption
							 is done with a single function call. 
 ============================================================================
 */


#include "encryption_wrapper.h" 


cipher_process_msg run_sha_and_aes(cipher_process process, uint32_t* password, uint8_t* msg_contents, uint8_t pswd_len_words, uint32_t msg_len_bytes){

	cipher_process_msg status_out;
	
	if(process == encrypt){
		status_out.status = encryption_success;
		strcpy(status_out.msg, "ENCRYPTION SUCCESS!");
	}
	else{ 
		status_out.status = decryption_success;
		strcpy(status_out.msg, "DECRYPTION SUCCESS!");
	}
	
	char temp_str[] = "ENCRYPTION FAILED "; 
	cipher_process_status temp_status = encryption_failed; 
	if(pswd_len_words > PSWD_WORD_LIMIT || pswd_len_words == 0 || msg_len_bytes > MSG_CHAR_LIMIT || msg_len_bytes == 0){
		if(process == decrypt){
			strcpy(temp_str, "DECRYPTION FAILED "); 
			temp_status = decryption_failed; 
		}
		
		status_out.status = temp_status;

		if(pswd_len_words > PSWD_WORD_LIMIT){
			strcpy(status_out.msg, strcat(temp_str, "- PASSWORD TOO LONG"));
		}
		else if(pswd_len_words == 0){
			strcpy(status_out.msg, strcat(temp_str, "- INVALID PASSWORD LENGTH"));
		}
		else if(msg_len_bytes > MSG_CHAR_LIMIT){
			strcpy(status_out.msg, strcat(temp_str, "- MESSAGE TOO LONG"));
		}
		else if(msg_len_bytes == 0){
			strcpy(status_out.msg, strcat(temp_str, "- INVALID MESSAGE LENGTH"));
		}
		
		return status_out;
	}
	
	
	uint32_t cipher_key_words[AES_KEY_LEN_WORDS] = {0}; 
	use_sha_256(password, pswd_len_words, cipher_key_words); 
	
	uint8_t cipher_key_bytes[AES_KEY_LEN_BYTES]; 
	for(uint32_t word = 0; word < AES_KEY_LEN_WORDS; word++){
		
		for(int byte = MSB; byte >= 0; byte--){
			cipher_key_bytes[(word * BYTES_IN_WORD) + byte] = (uint8_t) (cipher_key_words[word] >> (BITS_IN_BYTE * byte));
		}
	} 
	
	for(uint8_t word; word < AES_KEY_LEN_WORDS; word++){
		cipher_key_words[word] = 0; 
	}
	 
	uint32_t data_16_byte_blocks = (msg_len_bytes / AES_INPUT_BLOCK_SIZE);
	
	if(msg_len_bytes % AES_INPUT_BLOCK_SIZE != 0){
		data_16_byte_blocks++; 
	}
	
	
	for(uint32_t block = 0; block < data_16_byte_blocks; block++){
		use_aes(&msg_contents[block * AES_INPUT_BLOCK_SIZE], AES_KEY_LEN_BITS, cipher_key_bytes, process); 
	}

	
	return status_out; 
}






