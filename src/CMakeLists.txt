set(sts_src sts_util.c 
        sts_transport.c 
        sts_data.c
        sts_process.c
        sts_define.c
        sts_api.c)

add_library(cos_sts SHARED ${sts_src})
target_link_libraries(cos_sts curl json-c ssl)
