#include "sts_buf.h"
#include "sts_data.h"
#include "sts_process.h"
#include "sts_transport.h"
#include <stdio.h>

void sts_init_data(sts_data_t *data)
{
    memset(data->security_token, 0, sizeof(data->security_token));
    memset(data->tmp_secret_id, 0, sizeof(data->tmp_secret_id));
    memset(data->tmp_secret_key, 0, sizeof(data->tmp_secret_key));
}

void sts_init_status(sts_status_t *status)
{
    status->code = STSE_OK;
    memset(status->code_desc, 0, sizeof(status->code_desc));
    memset(status->message, 0, sizeof(status->message));
}

void sts_init_policy(sts_policy_t *policy) {
    sts_list_init(policy);
}

void sts_init_statement(sts_policy_statement_t *ps,
            char *action[], 
            int action_size,
            char *resource[],
            int resource_size,
            int allow)
{
    ps->action = action;
    ps->action_size = action_size;
    ps->resource = resource;
    ps->resource_size = resource_size;
    ps->effect = allow ? "allow" : "deny";
    sts_list_init(&ps->node);
}

void sts_add_statement(sts_policy_t *policy, sts_policy_statement_t *ps)
{
    sts_list_add_tail(&ps->node, policy);
}

sts_status_t get_federation_token(sts_options_t *options, sts_policy_t *policy, sts_data_t* sts_data) 
{ 
    int ret;
    sts_status_t status;
    char *post_data = NULL;
    char *policy_str = sts_parse_policy_to_json(policy);
    sts_context_t *ctx;

    sts_init_data(sts_data);
    sts_init_status(&status);

    ctx = sts_create_context();
    ctx->options = options;
    ctx->action = "GetFederationToken";
    ctx->policy = policy_str;

    ctx->req->mod = "sts";
    sts_build_body(ctx, &post_data);
    ctx->req->body = post_data;
    
    ret = sts_request(ctx); 
    if (ret != STSE_OK) {
        sts_transfer_code_to_status(ret, &status);
        goto done;
    }

    ret = sts_parse_from_json(ctx->resp->body, sts_data, &status);
    if (ret != STSE_OK) {
        sts_transfer_code_to_status(ret, &status);
        goto done;
    }

done:
    BufferRelease(post_data);
    sts_release_context(ctx);
    return status;
}
