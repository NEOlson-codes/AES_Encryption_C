



#ifndef ENCRYPTION_WRAPPER_H_
#define ENCRYPTION_WRAPPER_H_

#ifdef __cplusplus
 extern "C" {
#endif


#include "aes_encryption.h" 
#include "sha_256.h" 
#include "string.h"
#include "flash_manager.h"

#define AES_KEY_LEN_BITS      256U
#define AES_KEY_LEN_WORDS     8U
#define AES_KEY_LEN_BYTES			32U
#define PSWD_CHAR_LIMIT       32U
#define PSWD_WORD_LIMIT       (PSWD_CHAR_LIMIT / 4U)
#define MSG_CHAR_LIMIT        MSG_LEN_BYTES
#define AES_INPUT_BLOCK_SIZE  16U
#define BITS_IN_BYTE          8U
#define MSB                   3U

typedef aes_op_flag cipher_process; 
typedef enum{encryption_success, encryption_failed, decryption_success, decryption_failed} cipher_process_status; 

typedef struct{ 
	
	char msg[50]; 
	cipher_process_status status; 
	
} cipher_process_msg;

cipher_process_msg run_sha_and_aes(cipher_process process, uint32_t* password, uint8_t* msg_contents, uint8_t pswd_len_words, uint32_t msg_len_bytes);


#ifdef __cplusplus
}
#endif

#endif /* ENCRYPTION_WRAPPER_H_ */

