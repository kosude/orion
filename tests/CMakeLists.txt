add_executable(public "public.c" "${DEPENDENCIES_DIR}/execdeps/stb_image/stb_image.c")
add_custom_command(TARGET public PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/tests/resources $<TARGET_FILE_DIR:public>/resources)
target_link_libraries(public ${PROJECT_NAME} zetaml)
target_include_directories(public PUBLIC "${DEPENDENCIES_DIR}/execdeps")

add_executable(lighting "lighting.c" "${DEPENDENCIES_DIR}/execdeps/stb_image/stb_image.c")
add_custom_command(TARGET lighting PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/tests/resources $<TARGET_FILE_DIR:lighting>/resources)
target_link_libraries(lighting ${PROJECT_NAME} zetaml)
target_include_directories(lighting PUBLIC "${DEPENDENCIES_DIR}/execdeps")
