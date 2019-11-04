#ifndef _STS_TRANSPORT_H_
#define _STS_TRANSPORT_H_

#define MAX_URI_SIZE    1024

#include <curl/curl.h>
#include "sts_define.h"

typedef struct {
    char *mod;
    char *body;
} sts_request_params_t;

typedef struct {
    char *body;
    int body_pos;
    int body_len;
    int http_code;
} sts_response_params_t;

typedef struct {
    sts_request_params_t *req;
    sts_response_params_t *resp;

    CURL *curl;
    struct curl_slist *headers;
    char uri[MAX_URI_SIZE];
    curl_write_callback write_callback;
} sts_request_t;

typedef struct {
    sts_options_t *options;
    char *action;
    char *policy;
    sts_request_params_t *req;
    sts_response_params_t *resp;
} sts_context_t;

int sts_request(sts_context_t *ctx);

#endif
