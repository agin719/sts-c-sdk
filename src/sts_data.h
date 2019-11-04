#ifndef _STS_DATA_H_
#define _STS_DATA_H_

#include "sts_transport.h"

typedef struct {
    char *key;
    char *value; 
} sts_params_t;

int sts_build_body(sts_context_t *ctx, char **buf);

#endif
