function(glCreateSampleGTC NAME)
	set(SAMPLE_NAME ${NAME})

	add_executable(${SAMPLE_NAME} ${NAME}.cpp ${SHADER_PATH})
	add_test(NAME ${SAMPLE_NAME} COMMAND $<TARGET_FILE:${SAMPLE_NAME}>)

	target_link_libraries(${SAMPLE_NAME} framework ${BINARY_FILES})
	add_dependencies(${SAMPLE_NAME} glfw framework ${COPY_BINARY})

	install(TARGETS ${SAMPLE_NAME} DESTINATION .)
endfunction(glCreateSampleGTC)

glCreateSampleGTC(test-atomic-blend)
glCreateSampleGTC(test-atomic-ops)
glCreateSampleGTC(test-shader-fetch)
glCreateSampleGTC(test-texture-fetch)
