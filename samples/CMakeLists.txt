function(glCreateSampleGTC NAME)
	set(SAMPLE_NAME ${NAME})

	add_executable(${SAMPLE_NAME} ${NAME}.cpp ${SHADER_PATH})
	add_test(NAME ${SAMPLE_NAME} COMMAND $<TARGET_FILE:${SAMPLE_NAME}>)

	target_link_libraries(${SAMPLE_NAME} ${FRAMEWORK_NAME} ${BINARY_FILES})
	add_dependencies(${SAMPLE_NAME} glfw ${FRAMEWORK_NAME} ${COPY_BINARY})

	install(TARGETS ${SAMPLE_NAME} DESTINATION .)
endfunction(glCreateSampleGTC)

glCreateSampleGTC(atomic-01-blend)
glCreateSampleGTC(atomic-02-inc01-buffer-add)
glCreateSampleGTC(atomic-03-inc01-counter-inc)
glCreateSampleGTC(atomic-04-inc01-counter-add)
glCreateSampleGTC(atomic-05-if02x02)
glCreateSampleGTC(atomic-05-if04x04)
glCreateSampleGTC(atomic-05-if08x08)
glCreateSampleGTC(atomic-06-barrier)
glCreateSampleGTC(atomic-07-no-barrier)
