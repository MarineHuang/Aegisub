获取uuid所依赖的库
Windows: Rpcrt4.lib
例如：
target_link_libraries(${PROJECT_NAME} 
    PRIVATE rpcrt4.lib
)
liunx: openssl