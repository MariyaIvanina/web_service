cmake_minimum_required (VERSION 3.5)
if (UNIX)
  add_definitions(-Wno-sign-compare -Wno-enum-compare)
endif(UNIX)
project(restserver)
set (CMAKE_CXX_STANDARD 11)
option(DBMS "Dbms" OFF)
include_directories("${CMAKE_SOURCE_DIR}")
set (CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/Build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)


add_executable(restserver
  main.cpp
  src/handler.cpp
  src/database_impl.cpp
  )

set(Casablanca_LIBRARIES "-lboost_system -lcrypto -lssl -lcpprest -L/usr/local/lib -lmongocxx -lbsoncxx")
if(DBMS)
	set(NEW_DB_SRC_DIR ${CMAKE_SOURCE_DIR}/dbms/src)
	add_library(dbms SHARED
		${NEW_DB_SRC_DIR}/Dbms.cpp
		)
	set(Dbms_dep_lib "-lpqxx -lpq")
	target_link_libraries(restserver dbms ${Casablanca_LIBRARIES} ${Dbms_dep_lib})
else(DBMS)
	target_include_directories(restserver PUBLIC /usr/local/include/mongocxx/v_noabi /usr/local/include/bsoncxx/v_noabi)
	target_link_libraries(restserver ${Casablanca_LIBRARIES})
endif(DBMS)





