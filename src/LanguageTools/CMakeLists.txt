INCLUDE_DIRECTORIES(../)
AUX_SOURCE_DIRECTORY(. LANGUAGETOOLS_SRC_LIST)
ADD_LIBRARY(lib_languagetools ${LANGUAGETOOLS_SRC_LIST})