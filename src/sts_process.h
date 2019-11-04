#ifndef _STS_PROCESS_H_
#define _STS_PROCESS_H_

#include "sts_define.h"
#include "sts_transport.h"

sts_context_t *sts_create_context();

void sts_release_context(sts_context_t *ctx);

char *sts_parse_policy_to_json(sts_policy_t *policy);

int sts_parse_from_json(const char *body, sts_data_t *sts_data, sts_status_t *status);

void sts_transfer_code_to_status(int code, sts_status_t *status);
#endif
