/*********************************************************************\
 PROJECT        : AES-256 Encryption Module
 PURPOSE        : C language practice, learn more encryption algorithms
 DATE STARTED   : 2-20-22
 MAIN INTERFACE : aes_encryption.c
 NOTES          : 

I thought it would be a fun exercise to do the following - create an 
implementation of the AES-256 encryption algorithm, then use it to store
a private message on the STM32. This also requies an input method for the
message and some cryptographic key to use for encryption/decryption. My
plan is to wire up an LCD screen and a rotary encoder to the STM32 Discovery
board (which I own), then create a program that allows the user to either 
enter a new message or view/delete an old message using a password. The 
password will be alphanumeric characters. That password will be turned into
a hash by the SHA-256 algorithm (which I also plan on implementing). I'll 
put the base encryption/decryption and hash modules in a different spot within
the repo. Also, the AES and SHA implementations will be hardware agnostic. 

/*********************************************************************\

