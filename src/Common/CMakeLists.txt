INCLUDE_DIRECTORIES(../)
AUX_SOURCE_DIRECTORY(. COMMON_SRC_LIST)
ADD_LIBRARY(lib_common ${COMMON_SRC_LIST})