#ifndef _STS_DEFINE_H_
#define _STS_DEFINE_H_

#include "sts_list.h"

typedef enum {
    STSE_OK = 0,
    STSE_INVALID_ARGUMENT = 1,
    STSE_HMAC_FAILED = 2,
    STSE_INIT_REQUEST_FAILED = 3,
    STSE_HOST_FAILED = 4,
    STSE_FAILED_TO_CONNECT = 5,
    STSE_CURL_TIMEOUT = 6,
    STSE_VERIFICATION_FAILED = 7,
    STSE_INTERNAL_ERROR = 8,
    STSE_RET_DATA_ERROR = 9,
} sts_error_code_t;

typedef struct {
    int code;
    char message[1024];
    char code_desc[256];
} sts_status_t;

extern const char *CODE_MESSAGE[];
extern int CODE_SIZE;

typedef struct {
    char **action;
    char **resource;
    char *effect;
    int action_size;
    int resource_size;
    sts_list_t node;
} sts_policy_statement_t;

typedef sts_list_t sts_policy_t;

typedef struct {
    char *secret_id;
    char *secret_key;
    int expiration;
} sts_options_t;

typedef struct {
    char tmp_secret_id[64];
    char tmp_secret_key[64];
    char security_token[2048]; 
} sts_data_t;

#endif
