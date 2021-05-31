
add_executable(${PROJECT_NAME} ${FSM_SRC})
if (UNIX)
	set_target_properties(
    ${PROJECT_NAME} 
    PROPERTIES 
        SUFFIX ".bin"
)
endif (UNIX)

target_compile_options(${PROJECT_NAME} PUBLIC $<$<COMPILE_LANGUAGE:C>:${FLAGS_ERRORS}>)

if (ANDROID) 
   target_link_libraries(${PROJECT_NAME} ${FLAGS_ANDROID} ${LIBRARY_SDL} ${LIBRARY_STATIC} -lm)
elseif (WIN32)
	target_link_libraries(${PROJECT_NAME} ${LIBRARY_STATIC} -lmingw32 ${LIBRARY_SDL})
elseif (UNIX)
	target_link_libraries(${PROJECT_NAME} ${LIBRARY_STATIC} ${LIBRARY_SDL} -lm)
endif ()
