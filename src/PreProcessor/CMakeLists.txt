INCLUDE_DIRECTORIES(../)
AUX_SOURCE_DIRECTORY(. PREPROCESSOR_SRC_LIST)
ADD_LIBRARY(lib_preprocessor ${PREPROCESSOR_SRC_LIST})