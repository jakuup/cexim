add_executable(cexim main.cpp)

target_link_libraries(cexim PRIVATE prompt)
target_include_directories(cexim PRIVATE "${PROJECT_SOURCE_DIR}/modules")