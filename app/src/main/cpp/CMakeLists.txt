cmake_minimum_required(VERSION 3.0.0)

project(Native_core VERSION 0.1.0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")

SET(EXECUTABLE_OUTPUT_PATH /home/ilost/bin-output/) #调试用
SET(LIBRARY_OUTPUT_PATH /home/ilost/bin-output/) #调试用

# Android 5.0 以上需要在此处设置 PIE
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIE -pie")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")

include_directories("include")
find_library(z-lib z)
find_library(log-lib log)
add_library(local_crypto STATIC IMPORTED)
add_library(local_openssl STATIC IMPORTED)
add_library(local_nghttp2 STATIC IMPORTED)
add_library(local_curl STATIC IMPORTED)
#add_library(local_memory STATIC IMPORTED)

#goron
#间接跳转,并加密跳转目标(-mllvm -irobf-indbr)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mllvm -irobf-indbr")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mllvm -irobf-indbr")

#间接函数调用,并加密目标函数地址(-mllvm -irobf-icall)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mllvm -irobf-icall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mllvm -irobf-icall")

#间接全局变量[引用]^(ref),并加密变量地址(-mllvm -irobf-indgv)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mllvm -irobf-indgv")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mllvm -irobf-indgv")

#字符串(c string)加密功能(-mllvm -irobf-cse)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mllvm -irobf-cse")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mllvm -irobf-cse")

#过程相关控制流平坦混淆(-mllvm -irobf-cff)
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -mllvm -irobf-cff")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mllvm -irobf-cff")

set_target_properties(local_crypto PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcrypto.a)
set_target_properties(local_openssl PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libssl.a)
set_target_properties(local_nghttp2 PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libnghttp2.a)
set_target_properties(local_curl PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcurl.a)
#set_target_properties(local_memory PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/libmemory.a)




add_library(tencentloc SHARED tencentloc.cpp)
target_link_libraries(tencentloc local_curl local_nghttp2 local_openssl local_crypto ${z-lib} ${log-lib})

add_library(native-lib SHARED native-lib.cpp)
target_link_libraries(native-lib local_curl local_nghttp2 local_openssl local_crypto ${z-lib} ${log-lib})


#part1 注入 二进制
add_executable(part1 umbrella1.cpp)
target_link_libraries(part1 local_curl local_nghttp2 local_openssl local_crypto ${z-lib})

#part2 root 二进制
add_executable(part2 umbrella2.cpp)
target_link_libraries(part2 local_curl local_nghttp2 local_openssl local_crypto ${z-lib})

#part3 注入

#part4 原文件