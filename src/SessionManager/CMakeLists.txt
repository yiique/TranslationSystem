INCLUDE_DIRECTORIES(../)
AUX_SOURCE_DIRECTORY(. SESSIONMANAGER_SRC_LIST)
ADD_LIBRARY(lib_sessionmanager &{SESSIONMANAGER_SRC_LIST})
TARGET_LINK_LIBRARIES(lib_sessionmanager lib_cprocess)