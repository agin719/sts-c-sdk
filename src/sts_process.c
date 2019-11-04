#include "sts_process.h"
#include "sts_util.h"
#include "sts_buf.h"
#include <json-c/json.h>

sts_context_t *sts_create_context()
{
    sts_context_t *ctx;

    ctx = (sts_context_t *) malloc(sizeof(sts_context_t));
    ctx->options = NULL;
    ctx->action = NULL;
    ctx->policy = NULL;
    ctx->req = (sts_request_params_t *) malloc(sizeof(sts_request_params_t));
    ctx->req->mod = NULL;
    ctx->req->body = NULL;
    ctx->resp = (sts_response_params_t *) malloc(sizeof(sts_response_params_t));
    ctx->resp->body = NULL;
    ctx->resp->body_len = 0;
    ctx->resp->body_pos = 0;
    ctx->resp->http_code = 0;

    return ctx;
}

void sts_release_context(sts_context_t *ctx)
{
    if (ctx == NULL) 
        return;

    if (ctx->req != NULL) {
        free(ctx->req);
    }
    if (ctx->resp != NULL) {
        if (ctx->resp->body != NULL) {
            free(ctx->resp->body);
        }
        free(ctx->resp);
    }
    free(ctx);
}

char *sts_parse_policy_to_json(sts_policy_t *policy)
{
    int i;
    const char *str;
    sts_policy_statement_t *ps = NULL;
    json_object *obj = json_object_new_object();
    json_object *version = json_object_new_string("2.0");
    json_object *statements = json_object_new_array();

    sts_list_for_each_entry(sts_policy_statement_t, ps, policy, node) {
        json_object *statement = json_object_new_object();
        
        json_object *effect = json_object_new_string(ps->effect);
        json_object_object_add(statement, "effect", effect);

        json_object *actions = json_object_new_array();
        for (i = 0; i < ps->action_size; i++) {
            json_object *action = json_object_new_string(ps->action[i]);
            json_object_array_add(actions, action);
        }
        json_object_object_add(statement, "action", actions);

        json_object *resources = json_object_new_array();
        for (i = 0; i < ps->resource_size; i++) {
            json_object *resource = json_object_new_string(ps->resource[i]);
            json_object_array_add(resources, resource);
        }
        json_object_object_add(statement, "resource", resources);

        json_object_array_add(statements, statement);
    }

    json_object_object_add(obj, "version", version);
    json_object_object_add(obj, "statement", statements);

    str = json_object_to_json_string(obj);
    Buffer(res, strlen(str)+1);
    BufferAppend(res, str, strlen(str));
    
    json_object_put(obj);
    return res;
}

int sts_parse_from_json(const char *body, sts_data_t *sts_data, sts_status_t *status)
{
    json_object *obj = json_tokener_parse(body);
    json_object *code_obj; 
    json_object *msg_obj; 
    json_object *code_desc_obj;
    json_object *data;
    json_bool ret;
    const char *code_str;
    const char *code_desc_str;
    const char *message_str;

#define GetJsonObj(pobj, key, robj)       \
    do {                            \
        ret = json_object_object_get_ex(pobj, key, &robj); \
        if (ret < 0) {              \
            return STSE_RET_DATA_ERROR; \
        }                           \
    } while (0);
    GetJsonObj(obj, "code", code_obj);
    GetJsonObj(obj, "codeDesc", code_desc_obj);
    GetJsonObj(obj, "message", msg_obj);
    code_desc_str = json_object_get_string(code_desc_obj);
    message_str = json_object_get_string(msg_obj);
    status->code = json_object_get_int(code_obj);
    memcpy(status->code_desc, code_desc_str, strlen(code_desc_str));
    memcpy(status->message, message_str, strlen(message_str));

    ret = json_object_object_get_ex(obj, "data", &data);
    if (ret > 0) {
        json_object *credentials;
        json_object *security_token;
        json_object *tmp_secret_id;
        json_object *tmp_secret_key;
        const char *token_str;
        const char *id_str;
        const char *key_str;
        GetJsonObj(data, "credentials", credentials);
        GetJsonObj(credentials, "sessionToken", security_token);
        GetJsonObj(credentials, "tmpSecretId", tmp_secret_id);
        GetJsonObj(credentials, "tmpSecretKey", tmp_secret_key);
        token_str = json_object_get_string(security_token);
        id_str = json_object_get_string(tmp_secret_id);
        key_str = json_object_get_string(tmp_secret_key);
        memcpy(sts_data->security_token, token_str, strlen(token_str));
        memcpy(sts_data->tmp_secret_id, id_str, strlen(id_str));
        memcpy(sts_data->tmp_secret_key, key_str, strlen(key_str));
    }
    json_object_put(obj); 
    return STSE_OK;
}

void sts_transfer_code_to_status(int code, sts_status_t *status)
{
    status->code = code;
    if (code >= 0 && code < CODE_SIZE) {
        memcpy(status->message, CODE_MESSAGE[code], strlen(CODE_MESSAGE[code]));
        memcpy(status->code_desc, CODE_MESSAGE[code], strlen(CODE_MESSAGE[code]));
    }
}
