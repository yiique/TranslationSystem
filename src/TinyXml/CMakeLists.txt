INCLUDE_DIRECTORIES(../)
AUX_SOURCE_DIRECTORY(. TINYXML_SRC_LIST)
ADD_LIBRARY(lib_tinyxml ${TINYXML_SRC_LIST})
