include_directories (${TEST_SOURCE_DIR}/src)
add_executable (Test test.c)
if (UNIX)
	set_target_properties(
    Test 
    PROPERTIES 
        SUFFIX ".bin"
)
endif (UNIX)

if (ANDROID) 
   target_link_libraries(Test fortest-static ${FLAGS_C} ${FLAGS_ANDROID} ${LIBRARY_SDL} ${LIBRARY_STATIC} ${CMAKE_C_FLAGS} ${FLAGS_ERRORS} -lm)
elseif (WIN32)
	target_link_libraries(Test fortest-static ${FLAGS_C} ${LIBRARY_STATIC} -O2 -lmingw32 ${LIBRARY_SDL} ${FLAGS_ERRORS})
elseif (UNIX)
	target_link_libraries(Test fortest-static ${LIBRARY_STATIC} ${FLAGS_C} -O2 ${LIBRARY_SDL} ${FLAGS_ERRORS} -lm)
endif ()
