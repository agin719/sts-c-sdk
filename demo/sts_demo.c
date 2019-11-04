#include "sts_api.h"
#include <stdio.h>

void log_status(sts_status_t *status)
{
    printf("status code: %d\n", status->code);
    printf("status code_desc: %s\n", status->code_desc);
    printf("status message: %s\n", status->message);
}
int main(int argc, char *argv[])
{
    sts_data_t sts_data;
    sts_status_t status;
    int i;
    char *action[] = {"name/cos:PutObject", "name/cos:GetObject"};
    // 这里改成允许的路径前缀，可以根据自己网站的用户登录态判断允许上传的具体路径，例子： a.jpg 或者 a/* 或者 * (使用通配符*存在重大安全风险, 请谨慎评估使用)
    char *resource[] = {"qcs::cos::uid/1259654469:prefix//1259654469/test/exampleobject"};  
    sts_policy_statement_t scope1;
    sts_policy_statement_t scope2;
    sts_policy_t policy;
    sts_options_t config = {
        "<SECRET_ID>",                      // your SecretId
        "<SECRET_KEY>",                     // your SecretKey
        3600                                // timeout
    };

    sts_init_policy(&policy);
    sts_init_statement(&scope1, action, 2, resource, 1, 1);
    sts_add_statement(&policy, &scope1);


    sts_init_statement(&scope2, action, 2, resource, 1, 1);
    sts_add_statement(&policy, &scope2);

    status = get_federation_token(&config, &policy, &sts_data);
    log_status(&status);

    printf("tmp secret id: %s\n", sts_data.tmp_secret_id); 
    printf("tmp secret key: %s\n", sts_data.tmp_secret_key); 
    printf("token: %s\n", sts_data.security_token); 
    return 0;
}
