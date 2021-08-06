project(pbtk)

find_package(PNG)

add_subdirectory(third_party/libsigc++)

file(GLOB SRC src/*.cxx)
if (POCKETBOOK_PRO_FW5)
ADD_DEFINITIONS(-DPOCKETBOOK_PRO_FW5=1)
endif(POCKETBOOK_PRO_FW5) 

include_directories(third_party/boost/include)
include_directories(third_party/libsigc++)

add_library(pbtk STATIC ${SRC})
target_include_directories(pbtk PUBLIC src)

add_executable(pbtk_test.app test/pbtktest.cpp)
target_link_libraries(pbtk_test.app pbtk inkview sigc++)
