set(sts_src sts_util.c 
        sts_transport.c 
        sts_data.c
        sts_process.c
        sts_define.c
        sts_api.c)

add_library(cos_sts SHARED ${sts_src})
target_link_libraries(cos_sts curl json-c ssl)
set_target_properties(cos_sts PROPERTIES VERSION ${CMAKE_VERSION}  SOVERSION ${CMAKE_VERSION})

install(TARGETS cos_sts LIBRARY DESTINATION lib)
install(FILES 
    sts_api.h 
    sts_buf.h
    sts_data.h
    sts_define.h
    sts_list.h
    sts_process.h
    sts_transport.h
    sts_util.h
    DESTINATION include/sts_c_sdk)
