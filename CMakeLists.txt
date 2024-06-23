cmake_minimum_required(VERSION 3.0.0)
project(cexim VERSION 0.1.0 LANGUAGES C CXX)

# libraries
add_library(prompt modules/prompt.cpp)
add_library(ui modules/ui.cpp)

# cexim binary
add_executable(cexim main/main.cpp)
target_link_libraries(cexim PRIVATE prompt)
target_include_directories(cexim PRIVATE "${PROJECT_SOURCE_DIR}/modules")

# tests
add_executable(e__proxy_class test/e__proxy_class.cpp)

add_executable(t__prompt_runLoop test/t__prompt_runLoop.cpp)
target_link_libraries(t__prompt_runLoop PRIVATE prompt pthread ncurses)
target_include_directories(t__prompt_runLoop PRIVATE "${PROJECT_SOURCE_DIR}/modules")

add_executable(t__ui test/t__ui.cpp)
target_link_libraries(t__ui PRIVATE ui pthread ncurses)
target_include_directories(t__ui PRIVATE "${PROJECT_SOURCE_DIR}/modules")

include(CTest)
enable_testing()

add_test(
  NAME t__prompt_runLoop__null
  COMMAND $<TARGET_FILE:t__prompt_runLoop> "auto" "null"
)
