#---------------------------------------
# test folder
#---------------------------------------
if(BUILD_TESTS)
    add_subdirectory(tests)
endif(BUILD_TESTS)

#---------------------------------------
# examples folder
#---------------------------------------
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)