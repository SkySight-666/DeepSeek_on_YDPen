cmake_minimum_required(VERSION 3.10)

# 交叉编译工具链（假设已正确配置）
set(CMAKE_C_COMPILER ${CMAKE_SOURCE_DIR}/toolchain/arm-buildroot-linux-gnueabihf/bin/arm-buildroot-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER ${CMAKE_SOURCE_DIR}/toolchain/arm-buildroot-linux-gnueabihf/bin/arm-buildroot-linux-gnueabihf-g++)

project(dsProject)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 移除强制静态链接标志
# set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static")

# -----------------------
# 头文件路径（包含 curl、openssl 和 sqlite3）
# -----------------------
include_directories(
    ${PROJECT_SOURCE_DIR}/src  # h头文件
    ${PROJECT_SOURCE_DIR}/lib/curl/include  # curl 头文件
    ${PROJECT_SOURCE_DIR}/lib/openssl/include  # OpenSSL 头文件
    ${PROJECT_SOURCE_DIR}/lib/sqlite/include  # sqlite3 头文件
)

# -----------------------
# 库文件路径（明确指定 curl、openssl 和 sqlite3 库的位置）
# -----------------------
set(CURL_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib/curl/lib)
set(OPENSSL_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib/openssl/lib)
set(SQLITE3_LIBRARY_DIR ${PROJECT_SOURCE_DIR}/lib/sqlite/lib)

# -----------------------
# 查找库文件（显式查找 curl、ssl、crypto 和 sqlite3）
# 动态查找 curl 和 openssl 库，静态查找 sqlite 库
# -----------------------
find_library(CURL_LIBRARY NAMES curl PATHS ${CURL_LIBRARY_DIR})
find_library(SSL_LIBRARY NAMES ssl PATHS ${OPENSSL_LIBRARY_DIR})
find_library(CRYPTO_LIBRARY NAMES crypto PATHS ${OPENSSL_LIBRARY_DIR})
find_library(SQLITE3_LIBRARY NAMES libsqlite3.a PATHS ${SQLITE3_LIBRARY_DIR})

# -----------------------
# 添加可执行文件
# -----------------------
add_executable(dsProject ${PROJECT_SOURCE_DIR}/src/main.cpp)

# -----------------------
# 链接库（必须包含 curl、ssl、crypto、sqlite3 以及其他依赖）
# -----------------------
target_link_libraries(dsProject
    ${SQLITE3_LIBRARY}    # sqlite3 库，静态链接
    ${CURL_LIBRARY}       # libcurl 库，动态链接
    ${SSL_LIBRARY}        # OpenSSL SSL 库，动态链接
    ${CRYPTO_LIBRARY}     # OpenSSL 加密库，动态链接
    pthread                # 线程库
    dl                     # 动态链接库
)

# -----------------------
# 设置可执行文件输出路径
# -----------------------
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/out)

# 添加运行时库路径（解决动态库找不到的问题，临时方案）
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${CURL_LIBRARY_DIR}:${OPENSSL_LIBRARY_DIR}")