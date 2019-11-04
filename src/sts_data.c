#include "sts_data.h"
#include "sts_util.h"
#include "sts_buf.h"
#include <string.h>

static void sort(sts_params_t *params[], int size) 
{
    int i, j; 
    for (i = 0; i < size-1; i++) {
        for (j = i+1; j < size; j++) {
            if (strcmp(params[i]->key, params[j]->key) > 0) {
                sts_params_t *tmp = params[i];
                params[i] = params[j];
                params[j] = tmp;
            }
        }
    }
}

int compare(const void *x, const void *y) {
    sts_params_t *px = (sts_params_t*) x;
    sts_params_t *py = (sts_params_t*) y;
    return strcmp(px->key, py->key);
}

void sts_sign(char *signature, int size, sts_params_t *params[], int params_size, sts_context_t *ctx)
{
    int len = 0;
    int i;
    Buffer(buf, 1024);
   
    BufferAppend0(buf, "POST");
    BufferAppend0(buf, ctx->req->mod);
    BufferAppend0(buf, ".api.qcloud.com/v2/index.php?");
    for (int i = 0; i < params_size; i++) {
        if (i != 0) {
            BufferAppend0(buf, "&");
        }
        BufferAppend0(buf, params[i]->key); 
        BufferAppend0(buf, "=");
        BufferAppend0(buf, params[i]->value);
    }
    unsigned char hmac[20];
    hmac_sha1(hmac, ctx->options->secret_key, buf);
    char b64[((20 + 1) * 4) / 3];
    int b64_len = base64_encode(hmac, 20, b64);
    char b64_encoded[256];
    url_encode(b64_encoded, b64, b64_len);
    memcpy(signature, b64_encoded, strlen(b64_encoded));
    BufferRelease(buf);
}

/*
 *  需要调用者释放内存
 */
int sts_build_body(sts_context_t *ctx, char **buf)
{
    int i;
    sts_params_t *params[32];
    int params_size = 0;
    time_t now = time(NULL);
    char timebuf[64];
    char nonce[64];
    char expiration[64];
    struct timeval tv;
    Buffer(buffer, 1024);

#define add_params(_k, _v)                      \
    do {                                        \
        sts_params_t *param = (sts_params_t*) malloc(sizeof(sts_params_t)); \
        param->key = _k;                        \
        param->value = _v;                      \
        params[params_size++] = param;          \
    } while (0);

    snprintf(timebuf, sizeof(timebuf), "%ld", now);

    gettimeofday(&tv, NULL);
    snprintf(nonce, sizeof(nonce), "%ld", tv.tv_usec);

    snprintf(expiration, sizeof(expiration), "%d", ctx->options->expiration);

    add_params("Nonce", nonce);
    add_params("Timestamp", timebuf);
    add_params("Action", ctx->action);
    add_params("SecretId", ctx->options->secret_id);
    add_params("durationSeconds", expiration);
    add_params("name", "user");
    add_params("policy", ctx->policy);

    sort(params, params_size);
    //qsort(params, params_size, sizeof(sts_params_t*), compare);
    /*
    for (i = 0; i < params_size; i++) {
        printf("%s\n", params[i]->key);
    }*/
    char signature[256];
    sts_sign(signature, 256, params, params_size, ctx);

    add_params("Signature", signature);

    for (i = 0; i < params_size; i++) {
        if (i != 0) {
            BufferAppend0(buffer, "&");
        }
        BufferAppend0(buffer, params[i]->key); 
        BufferAppend0(buffer, "=");
        BufferAppend0(buffer, params[i]->value);
        free(params[i]);
    }

    *buf = buffer;
    return STSE_OK;
}

