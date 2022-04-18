/*
 ============================================================================
 Name        : flash_manager.h
 Author      : NEOlson
 Version     : 1
 Copyright   : N/A
 Date        : Apr 10, 2022
 Description : Includes functions for copying flash to RAM on startup and 
               saving encrypted messages to STM32 flash before shutdown. Provides
							 users get and set functions for encrypted data in RAM. 
 ============================================================================
 */


#ifndef FLASH_MANAGER_H_
#define FLASH_MANAGER_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>
#include <string.h>
#include "stm32f4xx_hal.h"

#define BYTES_IN_WORD_   4
#define NUM_MSGS         10
#define MSG_LEN_BYTES    256
#define MSG_TITLE_LEN    16
#define FLASH_BASE_ADDR  0x08020000  // Start of flash Sector 5, 128KB

#define MSG(msg_num)                (0x00000001 << msg_num) 

#define MSG_MARK_VALID(valid_bits, msg_num)  (valid_bits ^= msg_num)
#define MSG_MARK_INVALID(valid_bits, msg_num)   (valid_bits ^= msg_num)
#define MSG_VALID(valid_bits, msg_num)          (valid_bits & msg_num)  


typedef enum{flash_success, flash_failed} flash_status; 
typedef enum{write_success, delete_success, write_failed} write_status;

typedef struct{ 
	
	char msg[50]; 
	flash_status status; 
	
} flash_status_msg; 

typedef struct{
	
	char return_msg[50];
	write_status status; 
	
} msg_write_status; 

// Functions saving to or reading from flash memory
void manage_flash_startup(void);
flash_status_msg save_state_in_flash(void);

// Functions fetching or modifying encrypted messages in RAM
void get_encrypted_msg(uint8_t msg_num, uint8_t* msg_save_loc, uint32_t* msg_len_save_loc);
void get_msg_title(uint8_t msg_num, char* msg_title_save_loc);
msg_write_status write_message(uint8_t msg_num, char* new_title_loc, uint8_t* new_msg_loc, uint8_t msg_title_len, uint32_t msg_len);
void delete_message(uint8_t msg_num);


#ifdef __cplusplus
}
#endif

#endif /* FLASH_MANAGER_H_ */
