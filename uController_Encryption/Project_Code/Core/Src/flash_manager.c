/*
 ============================================================================
 Name        : flash_manager.c
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Apr 10, 2022
 Description : Includes functions for copying flash to RAM on startup and 
               saving encrypted messages to STM32 flash before shutdown. Provides
							 users get and set functions for encrypted data in RAM. 
 ============================================================================
 */


#include "flash_manager.h"


static void program_flash(uint32_t type_program, uint32_t* flash_address, uint32_t data, flash_status_msg* status_msg);
static void flash_write_error(flash_status_msg* status_msg);
static void startup_cpy_flash_to_ram(uint8_t* messages, char* titles, uint32_t* msg_lengths);
static flash_status_msg cpy_ram_to_flash(uint8_t* messages, char* titles, uint32_t* msg_lengths);

static uint32_t msg_lengths[NUM_MSGS] = {0};

static uint8_t encrypted_data[NUM_MSGS * MSG_LEN_BYTES] = {0};  

static char msg_titles[NUM_MSGS * MSG_TITLE_LEN] = {0}; 


/*---------------- CORE FCNS FLASH STARTUP/SHUTDOWN -------------------*/ 

static void startup_cpy_flash_to_ram(uint8_t* messages, char* titles, uint32_t* msg_lengths){
	
	uint32_t flash_addr = FLASH_BASE_ADDR; 
	
	// Read the valid_msgs bit field from flash into RAM
	for(int msg = 0; msg < NUM_MSGS; msg++){
		msg_lengths[msg] = *(uint32_t*) flash_addr; 
		flash_addr += 4;
	}
	
		
	// Use msg_lengths to determine which messages to copy. 
	for(int msg = 0; msg < NUM_MSGS; msg++){

			for(int byte = 0; byte < MSG_LEN_BYTES; byte++){
				messages[(msg * MSG_LEN_BYTES) + byte] = *(uint8_t*) flash_addr; 
				flash_addr++; 
			}
	}
	
	for(int msg = 0; msg < NUM_MSGS; msg++){
			
		for(int byte = 0; byte < MSG_TITLE_LEN; byte++){
				titles[(msg * MSG_TITLE_LEN) + byte] = *(char*) flash_addr; 
				flash_addr++; 
		}
	}
}


static flash_status_msg cpy_ram_to_flash(uint8_t* messages, char* titles, uint32_t* msg_lengths){

	flash_status_msg status_out = {
		.msg = "FLASH SUCCESS!",
		.status = flash_success
	};
	
	uint32_t flash_addr = FLASH_BASE_ADDR; 
	
	HAL_FLASH_Unlock(); 
	
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR |
										 FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR); 
	
	FLASH_Erase_Sector(FLASH_SECTOR_5, FLASH_VOLTAGE_RANGE_3);
	
	
	for(int msg = 0; msg < NUM_MSGS; msg++){
		program_flash(FLASH_TYPEPROGRAM_WORD, &flash_addr, msg_lengths[msg], &status_out);
	}
		
	for(int msg = 0; msg < NUM_MSGS; msg++){
		
		for(int byte = 0; byte < MSG_LEN_BYTES; byte++){
			program_flash(FLASH_TYPEPROGRAM_BYTE, &flash_addr, messages[(msg * MSG_LEN_BYTES) + byte], &status_out);
		}
	}
		
	for(int msg = 0; msg < NUM_MSGS; msg++){
		for(int byte = 0; byte < MSG_TITLE_LEN; byte++){
			program_flash(FLASH_TYPEPROGRAM_BYTE, &flash_addr, titles[(msg * MSG_TITLE_LEN) + byte], &status_out);
		}
	}

	HAL_FLASH_Lock(); 
	
	return status_out; 
}


void manage_flash_startup(void){
	startup_cpy_flash_to_ram(encrypted_data, msg_titles, msg_lengths); 
}
	

flash_status_msg save_state_in_flash(void){
	return cpy_ram_to_flash(encrypted_data, msg_titles, msg_lengths);
}


/*---------------- FLASH UTILITY FUNCTIONS -------------------*/

static void program_flash(uint32_t type_program, uint32_t* flash_address, uint32_t data, flash_status_msg* status_msg){
	
	if(status_msg->status == flash_failed){
		return; 
	}
	
	if(HAL_FLASH_Program(type_program, *flash_address, data) == HAL_OK){
		if(type_program == FLASH_TYPEPROGRAM_WORD){
			*flash_address += 4;
		}
		if(type_program == FLASH_TYPEPROGRAM_BYTE){
			(*flash_address)++; 
		}
		
	}
	else{
		flash_write_error(status_msg);
	}
}


static void flash_write_error(flash_status_msg* status_msg){
	status_msg->status = flash_failed; 
	strcpy(status_msg->msg, "FLASH FAILED!");
}

/*---------------- MESSAGE FETCH & MODIFY -------------------*/ 


void get_encrypted_msg(uint8_t msg_num, uint8_t* msg_save_loc, uint32_t* msg_len_save_loc){
	
	for(int byte = 0; byte < MSG_LEN_BYTES; byte++){
		msg_save_loc[byte] = encrypted_data[(msg_num * MSG_LEN_BYTES) + byte];
	}
	
		*msg_len_save_loc = msg_lengths[msg_num];
}

void get_msg_title(uint8_t msg_num, char* msg_title_save_loc){
	
	for(int byte = 0; byte < MSG_TITLE_LEN; byte++){
		msg_title_save_loc[byte] = msg_titles[(msg_num * MSG_TITLE_LEN) + byte];
	}	
}


msg_write_status write_message(uint8_t msg_num, char* new_title_loc, uint8_t* new_msg_loc, uint8_t msg_title_len, uint32_t msg_len){
	
	msg_write_status status_out = {
		.return_msg = "WRITE SUCCESS!",
		.status = write_success
	};
	
	if(msg_title_len > MSG_TITLE_LEN || msg_title_len == 0 || msg_len > MSG_LEN_BYTES || msg_len == 0){
		status_out.status = write_failed;
		
		if(msg_title_len > MSG_TITLE_LEN){
			strcpy(status_out.return_msg, "WRITE FAILED - MESSAGE TITLE TOO LONG");
		}
		else if(msg_title_len <= 0){
			strcpy(status_out.return_msg, "WRITE FAILED - INVALID MESSAGE TITLE LENGTH");
		}
		else if(msg_len > MSG_LEN_BYTES){
			strcpy(status_out.return_msg, "WRITE FAILED - MESSAGE EXCEEDS MAX LENGTH");
		}
		else if(msg_len <= 0){
			strcpy(status_out.return_msg, "WRITE FAILED - INVALID MESSAGE LENGTH");
		}
		
		return status_out;
	}
	
	msg_lengths[msg_num] = msg_len; 
	
	for(int byte = 0; byte < MSG_LEN_BYTES; byte++){
		if(byte < msg_len){
			encrypted_data[(msg_num * MSG_LEN_BYTES) + byte] = new_msg_loc[byte];
		}
		else{
			encrypted_data[(msg_num * MSG_LEN_BYTES) + byte] = 0;
		}
	}
	 
	uint8_t count = 0; 
	while(new_title_loc[count] != '\0' && count < MSG_TITLE_LEN){
		msg_titles[(msg_num * MSG_TITLE_LEN) + count] = new_title_loc[count]; 
		count++; 
	}
	
	return status_out;
}


void delete_message(uint8_t msg_num){
	msg_lengths[msg_num] = 0;
	
	for(int byte = 0; byte < MSG_LEN_BYTES; byte++){
		encrypted_data[(msg_num * MSG_LEN_BYTES) + byte] = 0;
	}
	
	for(int byte = 0; byte < MSG_TITLE_LEN; byte++){
		msg_titles[(msg_num * MSG_TITLE_LEN) + byte] = 0;
	}
	
}

