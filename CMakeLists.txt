cmake_minimum_required(VERSION 3.31)

# set project name
project(VPL C)

# specify C standard
set(CMAKE_C_STANDARD_REQUIRED True)

# check for CUnit testing library installation
find_library(CUNIT_LIB cunit)
if(NOT CUNIT_LIB)
  message(FATAL_ERROR "CUnit library not found! Please install it.")
endif()

# spedify compiler flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wundef -Wshadow -Wpointer-arith")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wcast-align -Wstrict-prototypes")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wstrict-overflow=5")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wconversion -Wunreachable-code")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=undefined")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=integer-divide-by-zero")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=null")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=signed-integer-overflow")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize-address-use-after-scope")


# specify include directory
include_directories(include)

# include these source files
set(SOURCES
    src/dynamic_array.c
    src/linked_list.c
    src/doubly_linkedlist.c
    src/stack.c
    src/stack_ll.c
    src/queue.c
    src/queue_ll.c
    src/deque.c
    src/deque_ll.c
)

# create a static library from the specified source files
add_library(dsa STATIC ${SOURCES})

# include the test files
set(TEST_SOURCES
    tests/test_dynamic_array.c
    tests/test_linked_list.c
    tests/test_doubly_linkedlist.c
    tests/test_stack.c
    tests/test_stackll.c
    tests/test_queue.c
    tests/test_queue_ll.c
    tests/test_deque.c
    tests/test_deque_ll.c
)

# create seperate executable for each test files
foreach(TEST_FILE ${TEST_SOURCES})
  # extract the file name (without directory extension)
  get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

  # generate executable
  add_executable(${TEST_NAME} ${TEST_FILE})

  # link the executable with dsa library
  target_link_libraries(${TEST_NAME} dsa ${CUNIT_LIB})
endforeach()
