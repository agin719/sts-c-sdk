#include "sts_transport.h"
#include "sts_util.h"
#include "sts_define.h"
#include "sts_buf.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static int transfer_code(CURLcode code) {
    switch (code) {
        case CURLE_COULDNT_RESOLVE_PROXY:
        case CURLE_COULDNT_RESOLVE_HOST:
            return STSE_HOST_FAILED;
        case CURLE_COULDNT_CONNECT:
            return STSE_FAILED_TO_CONNECT;
        case CURLE_OPERATION_TIMEDOUT:
            return STSE_CURL_TIMEOUT;
        case CURLE_SSL_CACERT:
            return STSE_VERIFICATION_FAILED;
        default:
            return STSE_INTERNAL_ERROR;
    }
}
static size_t default_write_callback(char *ptr, size_t size, size_t nmemb, void *data)
{
    sts_request_t *request = (sts_request_t*) data;
    int len = size * nmemb;

    CURLcode code = curl_easy_getinfo(request->curl, 
                        CURLINFO_RESPONSE_CODE, &request->resp->http_code);
    if (code != CURLE_OK) {
        return 0;
    }
    BufferAppend(request->resp->body, ptr, len); 
    
    return len;
}

int setup_curl(sts_request_t* request)
{
#define curl_easy_setopt_safe(opt, val)                                 \
    if (curl_easy_setopt(request->curl, opt, val) != CURLE_OK) {        \
        return STSE_INIT_REQUEST_FAILED;                                \
    }

    curl_easy_setopt_safe(CURLOPT_PRIVATE, request);
    curl_easy_setopt_safe(CURLOPT_URL, request->uri);
    curl_easy_setopt_safe(CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt_safe(CURLOPT_SSL_VERIFYHOST, 0);
    //curl_easy_setopt_safe(CURLOPT_VERBOSE, 1);
    //curl_easy_setopt_safe(CURLOPT_TIMEOUT, 120);
    curl_easy_setopt_safe(CURLOPT_POST, 1);
    curl_easy_setopt_safe(CURLOPT_POSTFIELDS, request->req->body);
    curl_easy_setopt_safe(CURLOPT_POSTFIELDSIZE, strlen(request->req->body));
    
    curl_easy_setopt_safe(CURLOPT_WRITEFUNCTION, request->write_callback);
    curl_easy_setopt_safe(CURLOPT_WRITEDATA, request);
    
    request->headers = curl_slist_append(request->headers, "Content-Type: application/x-www-form-urlencoded");
    curl_easy_setopt_safe(CURLOPT_HTTPHEADER, request->headers);

    return STSE_OK;
}

static sts_request_t* sts_create_request(sts_context_t *ctx)
{
    sts_request_t *request = (sts_request_t*) malloc(sizeof(sts_request_t));
    request->req = ctx->req; 
    request->resp = ctx->resp; 
    if (!(request->curl = curl_easy_init())) {
        free(request);
        return NULL;
    }
    request->headers = NULL;
    snprintf(request->uri, MAX_URI_SIZE, 
            "https://%s.api.qcloud.com/v2/index.php", request->req->mod);
    request->write_callback = default_write_callback;
    
    setup_curl(request);
    return request;
}

static void sts_destroy_request(sts_request_t *request)
{
    if (request == NULL) {
        return;
    }
    if (request->headers) {
        curl_slist_free_all(request->headers);
    }
    curl_easy_cleanup(request->curl);
    free(request);
}

int sts_request(sts_context_t *ctx)
{
    sts_request_t *request = sts_create_request(ctx);
    if (request == NULL) {
        return STSE_INIT_REQUEST_FAILED;
    }

    CURLcode code = curl_easy_perform(request->curl);
    if (code != CURLE_OK && code != CURLE_PARTIAL_FILE) {
        request->resp->http_code = 0;
        sts_destroy_request(request);
        return transfer_code(code);
    }
    sts_destroy_request(request);
    
    return STSE_OK;
}
