INCLUDE_DIRECTORIES(../)

#SET(TEST_CASE_LIST blocking_deque.cpp)
#ADD_EXECUTABLE(test_case ${TEST_CASE_LIST})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_THREAD_NAME})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_DATETIME_NAME})
#TARGET_LINK_LIBRARIES(test_case lib_eventengine)

#SET(TEST_CASE_LIST event_engine_test.cpp)
#ADD_EXECUTABLE(test_case ${TEST_CASE_LIST})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_THREAD_NAME})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_DATETIME_NAME})
#TARGET_LINK_LIBRARIES(test_case lib_eventengine)

#SET(TEST_CASE_LIST log_test.cpp)
#ADD_EXECUTABLE(test_case ${TEST_CASE_LIST})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_THREAD_NAME})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_DATETIME_NAME})
#TARGET_LINK_LIBRARIES(test_case lib_eventengine)
#TARGET_LINK_LIBRARIES(test_case lib_log)

#SET(TEST_CASE_LIST configurator_test.cpp)
#ADD_EXECUTABLE(test_case ${TEST_CASE_LIST})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_THREAD_NAME})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_DATETIME_NAME})
#TARGET_LINK_LIBRARIES(test_case lib_configurator)
#TARGET_LINK_LIBRARIES(test_case lib_common)
#TARGET_LINK_LIBRARIES(test_case lib_eventengine)
#TARGET_LINK_LIBRARIES(test_case lib_log)

#SET(TEST_CASE_LIST db_test.cpp)
#ADD_EXECUTABLE(test_case ${TEST_CASE_LIST})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_THREAD_NAME})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_DATETIME_NAME})
#TARGET_LINK_LIBRARIES(test_case ${LIB_BOOST_SYSTEM_NAME})
#TARGET_LINK_LIBRARIES(test_case ${LIB_MYSQLPP_PATH})
#TARGET_LINK_LIBRARIES(test_case lib_eventengine)
#TARGET_LINK_LIBRARIES(test_case lib_database)
#TARGET_LINK_LIBRARIES(test_case lib_log)

SET(TEST_CASE_LIST common_test.cpp)
ADD_EXECUTABLE(test_case ${TEST_CASE_LIST})
TARGET_LINK_LIBRARIES(test_case lib_common)