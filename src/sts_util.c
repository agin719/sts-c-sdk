#include "sts_util.h"
#include "sts_define.h"
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <openssl/hmac.h>

int base64_encode(const unsigned char *src, int src_len, char *dst)
{
    static const char *ENC = 
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    char *original_dst = dst;

    while (src_len) {
        // first 6 bits of char 1
        *dst++ = ENC[*src >> 2];
        if (!--src_len) {
            // last 2 bits of char 1, 4 bits of 0
            *dst++ = ENC[(*src & 0x3) << 4];
            *dst++ = '=';
            *dst++ = '=';
            break;
        }
        // last 2 bits of char 1, first 4 bits of char 2
        *dst++ = ENC[((*src & 0x3) << 4) | (*(src + 1) >> 4)];
        src++;
        if (!--src_len) {
            // last 4 bits of char 2, 2 bits of 0
            *dst++ = ENC[(*src & 0xF) << 2];
            *dst++ = '=';
            break;
        }
        // last 4 bits of char 2, first 2 bits of char 3
        *dst++ = ENC[((*src & 0xF) << 2) | (*(src + 1) >> 6)];
        src++;
        // last 6 bits of char 3
        *dst++ = ENC[*src & 0x3F];
        src++, src_len--;
    }

    return (dst - original_dst);
}

int url_encode(char *dest, const char *src, int max_src_size)
{
    static const char *hex = "0123456789ABCDEF";

    int len = 0;
    unsigned char c;

    while (*src) {
        if (++len > max_src_size) {
            *dest = 0;
            return STSE_INVALID_ARGUMENT;
        }
        c = *src;
        if (isalnum(c) || (c == '-') || (c == '_') || (c == '.') || (c == '~')) {
            *dest++ = c;
        } else if (*src == ' ') {
            *dest++ = '%';
            *dest++ = '2';
            *dest++ = '0';
        } else {
            *dest++ = '%';
            *dest++ = hex[c >> 4];
            *dest++ = hex[c & 15];
        }
        src++;
    }

    *dest = 0;

    return STSE_OK;
}

int hmac_sha1(unsigned char hmac[20], const char *key, const char *message)
{
    int len;

    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);
    HMAC_Init_ex(&ctx, key, strlen(key), EVP_sha1(), NULL);
    HMAC_Update(&ctx, (unsigned char*)message, strlen(message));
    HMAC_Final(&ctx, hmac, &len);
    HMAC_CTX_cleanup(&ctx); 
    
    if (len != 20) {
        return STSE_HMAC_FAILED;
    }
    return STSE_OK;
}

