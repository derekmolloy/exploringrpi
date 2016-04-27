cmake_minimum_required(VERSION 3.0.2)
project (TestERPiLibrary)
 
#For the shared library:
set ( PROJECT_LINK_LIBS libExploringRPi.so )
link_directories( ~/exploringrpi/library/build )
 
#For the static library:
#set ( PROJECT_LINK_LIBS libExploringRPi.a )
#link_directories( ~/exploringrpi/library/build )
 
include_directories(~/exploringrpi/library/)
 
add_executable(libtest libtest.cpp)
target_link_libraries(libtest ${PROJECT_LINK_LIBS} )
