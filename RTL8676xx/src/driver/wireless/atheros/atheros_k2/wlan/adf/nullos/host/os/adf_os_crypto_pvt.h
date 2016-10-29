#ifndef __ADF_HST_OS_CRYPTO_PVT_H
#define __ADF_HST_OS_CRYPTO_PVT_H

#define __ADF_AES_BLOCK_LEN  15   /*Arbitary value */

/**
 * @brief Representation of a cipher context.
 */ 
typedef void * __adf_os_cipher_t;

typedef enum {
    __ADF_OS_CRYPTO_AES,
    __ADF_OS_CRYPTO_OTHER
} __adf_os_crypto_alg_t;

/**
 * @brief allocate the cipher context
 * @param[in] type crypto algorithm
 * 
 * @return the new cipher context
 */
static adf_os_inline __adf_os_cipher_t
__adf_os_crypto_alloc_cipher(__adf_os_crypto_alg_t type)
{
    return NULL;
}

/**
 * @brief free the cipher context
 * 
 * @param[in] cipher cipher context
 */
static adf_os_inline void
__adf_os_crypto_free_cipher(__adf_os_cipher_t cipher)
{
    return;
}

/**
 * @brief set the key for cipher context with length keylen
 * 
 * @param[in] cipher    cipher context
 * @param[in] key       key material
 * @param[in] keylen    length of key material
 * 
 * @return a_uint32_t
 */
static adf_os_inline a_uint32_t
__adf_os_crypto_cipher_setkey(__adf_os_cipher_t cipher, const a_uint8_t *key, a_uint8_t keylen)
{
    return 0;
}

/**
 * @brief encrypt the data with AES
 * 
 * @param[in]   cipher  cipher context
 * @param[in]   src     unencrypted data
 * @param[out]  dst     encrypted data
 */
static adf_os_inline a_status_t
__adf_os_crypto_rijndael_encrypt(__adf_os_cipher_t cipher, const void *src, void *dst)
{
    return A_STATUS_OK;
}

#endif
