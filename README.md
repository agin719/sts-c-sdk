## 开发准备
### 相关资源
[QCloud COS 服务临时密钥 SDK](https://github.com/tencentyun/qcloud-cos-sts-sdk)

### 开发环境
1、安装依赖包
```bash
# apt-get install cmake
# apt-get install libssl-dev
# apt-get install libcurl4-openssl-dev
# apt-get install libjson-c-dev
```

2、安装SDK
```bash 
# cd sts-c-sdk
# mkdir build
# cd build 
# cmake ..
# make 
# sudo make install
```

### 样例代码
[demo](./demo/sts_demo.c)
