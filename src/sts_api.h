#ifndef _STS_API_H_
#define _STS_API_H_

#include "sts_transport.h"
#include "sts_define.h"

void sts_init_policy(sts_policy_t *policy);

void sts_init_statement(sts_policy_statement_t *ps,
            char *action[], 
            int action_size,
            char *resource[],
            int resource_size,
            int allow);

void sts_add_statement(sts_policy_t *policy, sts_policy_statement_t *ps);

sts_status_t get_federation_token(sts_options_t *options, sts_policy_t *policy, sts_data_t *sts_data);


#endif
