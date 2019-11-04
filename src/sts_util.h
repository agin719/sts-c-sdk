#ifndef _STS_UTIL_H_
#define _STS_UTIL_H_

int base64_encode(const unsigned char *src, int src_len, char *dst);

int url_encode(char *dest, const char *src, int max_src_size);

int hmac_sha1(unsigned char hmac[20], const char *key, const char *message);

#endif
