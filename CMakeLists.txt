CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(TransMaster)
SET( CMAKE_BUILD_TYPE Release )

SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)

ADD_DEFINITIONS( -DWINDOWS )

SET(BASE_INCLUDE_PATH "C:/Users/sally/Desktop/Code/master/src/")
INCLUDE_DIRECTORIES("C:/Users/sally/Desktop/Code/boost_1_48_0/")
INCLUDE_DIRECTORIES(C:/Users/sally/Desktop/Code/mysql/include/)
INCLUDE_DIRECTORIES(C:/Users/sally/Desktop/Code/mysql++-3.1.0/)
#INCLUDE_DIRECTORIES(C:/Users/sally/Desktop/Code/icu/include/win64/)

#SET(ICU_LIB_PATH "C:/Users/sally/Desktop/TRTrans/icu/lib/win64/")

SET(LIB_MYSQL_PATH   "C:/Users/sally/Desktop/Code/mysql/lib/opt/libmysql.lib")
SET(LIB_MYSQLPP_PATH "C:/Users/sally/Desktop/Code/mysql++-3.1.0/vc2008/Release/mysqlpp.lib")
SET(LIB_BOOST_PATH "")
SET(LIB_BOOST_THREAD_NAME "C:/Users/sally/Desktop/Code/boost_1_48_0/stage/lib/libboost_thread-vc100-mt-1_48.lib")
SET(LIB_BOOST_REGEX_NAME  "C:/Users/sally/Desktop/Code/boost_1_48_0/stage/lib/libboost_regex-vc100-mt-1_48.lib")
SET(LIB_BOOST_DATETIME_NAME "C:/Users/sally/Desktop/Code/boost_1_48_0/stage/lib/libboost_date_time-vc100-mt-1_48.lib")
SET(LIB_BOOST_SYSTEM_NAME "C:/Users/sally/Desktop/Code/boost_1_48_0/stage/lib/libboost_system-vc100-mt-1_48.lib")

#SET(LIB_JSON_NAME "ljson.lib")
#SET(LIB_JSON_PATH "C:/Users/sally/Desktop/TRTrans/json/build_x64/src/Release/")

#SET(LIB_EN_TOKEN_NAME "lib_zdqtokenizer.lib")
#SET(LIB_EN_TOKEN_PATH "C:/Users/sally/Desktop/TRTrans/entokenizer/build_x64/bin/Release/")
#SET(LIB_LM_PATH "C:/Users/sally/Desktop/TRTrans/lm/win64/")
#SET(LIB_LM_NAME "lmdll.lib")

ADD_SUBDIRECTORY(src)
MESSAGE(STATUS "This is SOURCE dir " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "This is BINARY dir " ${PROJECT_BINARY_DIR})