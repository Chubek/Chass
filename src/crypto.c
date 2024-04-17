#include <openssl/err.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chass.h"

void initialize_libcrypto(void) {
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);
}

void cleanup_libcrypto(void) {
  EVP_cleanup();
  CRYPTO_cleanup_all_ex_data();
  ERR_free_strings();
}

int encrypt_symmetric_key(const String *symmetric_key, EVP_PKEY *public_key,
                          String *encrypted_key) {
  EVP_PKEY_CTX *ctx;
  int len;

  if (!(ctx = EVP_PKEY_CTX_new(public_key, NULL)))
    return 0;
  if (EVP_PKEY_encrypt_init(ctx) <= 0)
    return 0;
  if (EVP_PKEY_encrypt(ctx, NULL, &encrypted_key->len, symmetric_key->buffer,
                       symmetric_key->len) <= 0)
    return 0;
  if (!(encrypted_key->buffer = crypto_alloc(encrypted_key->len)))
    return 0;
  if (EVP_PKEY_encrypt(ctx, *encrypted_key->buffer, encrypted_key->len,
                       symmetric_key->buffer, symmetric_key->len) <= 0)
    return 0;
  EVP_PKEY_CTX_free(ctx);
  return 1;
}

int encrypt_data(const String *plaintext, const String *key, const String *iv,
                 const String *ciphertext) {
  EVP_CIPHER_CTX *ctx;
  int len;
  int ciphertext_temp_len;

  ciphertext.len = 0;

  if (!(ciphertext.buffer = (uint8_t *)crypto_alloc(
            plaintext.len + EVP_CIPHER_block_size(EVP_aes_256_cbc())))) {
    return 0;
  }

  if (!(ctx = EVP_CIPHER_CTX_new())) {
    return 0;
  }

  if (1 !=
      EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key->buffer, iv->buffer)) {
    EVP_CIPHER_CTX_free(ctx);
    return 0;
  }

  if (1 != EVP_EncryptUpdate(ctx, ciphertext.buffer, &len, plaintext,
                             plaintext.len)) {
    EVP_CIPHER_CTX_free(ctx);
    return 0;
  }
  ciphertext.len = len;

  if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.buffer + len, &len)) {
    EVP_CIPHER_CTX_free(ctx);
    return 0;
  }
  ciphertext.len += len;

  EVP_CIPHER_CTX_free(ctx);

  return 1;
}

EVP_PKEY *load_ssh_private_key(const char *file_path) {
  EVP_PKEY *pkey = NULL;
  FILE *fp = fopen(file_path, "r");
  if (fp == NULL) {
    fprintf(stderr, "Error opening file: %s\n", file_path);
    return NULL;
  }
  pkey = PEM_read_PrivateKey(fp, NULL, NULL, NULL);
  fclose(fp);
  if (pkey == NULL) {
    ERR_print_errors_fp(stderr);
  }
  return pkey;
}
