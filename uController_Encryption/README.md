Project: Use the AES-256 algorithm to store encrypted messages in STM32 flash. Provide a user interface
         to facilitate writing to and reading from the flash. UI will consist of an LCD display, two 
         push buttons, and a rotary encoder. Messages will be handled internally such that there is no
         temporary saving of passwords or message text. 

Expansions: The first iteration of the project is as described above. However, I would like to use it as
         a starting place for exploring some other aspects of the STM32, such as internal memory protection. 
         Potential add on features: reading from and writing to SD card, read/write from/to PC, augment protection 
         of stored data using any chip-specific functions