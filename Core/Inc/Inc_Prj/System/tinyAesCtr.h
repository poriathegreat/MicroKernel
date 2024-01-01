#ifndef TINY_AES_CTR_H
#define TINY_AES_CTR_H


#include <stdint.h>
#include <stddef.h>
// define the macros below to 1/0 to enable/disable the mode of operation.
#define AES128 1
//#define AES192 1
//#define AES256 1
#define AES_BLOCKLEN 16 // Block length in bytes - AES is 128b block only
#if defined(AES256) && (AES256 == 1)
    #define AES_KEYLEN 32
    #define AES_keyExpSize 240
#elif defined(AES192) && (AES192 == 1)
    #define AES_KEYLEN 24
    #define AES_keyExpSize 208
#else
    #define AES_KEYLEN 16   // Key length in bytes
    #define AES_keyExpSize 176
#endif


/* The result of the encryption is returned as a value of 0 or 1, 0 being OK and 1 showing the
 * process has failed. */

typedef enum {
    ENCRYPTION_OK = 0,
    ENCRYPTION_FAILED = 1
}encryptionResult_t;


/* These two functions take in a buffer and its size, they also add/remove the padding (needed to be a multiple of 8)
 * the output is then stored in the output buffer. the size may or may not change, that is why it takes a pointer.
 * if padding is added or removed, the size will change for the layer above to handle.
 * 
 * Maximum size of the encryption buffer should not exceed 256 bytes,
 * This should be handled in the above layer of the code. */
encryptionResult_t aes_ctr_encrypt(const uint8_t* key, const uint8_t* iv, const uint8_t* input, uint8_t* output, size_t* size);
encryptionResult_t aes_ctr_decrypt(const uint8_t* key, const uint8_t* iv, const uint8_t* input, uint8_t* output, size_t* size);

#endif /* TINY_AES_CTR_H */
