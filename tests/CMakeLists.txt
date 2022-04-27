# ---
# public: testing the general public interface of Orion.

add_executable(public "public.c")
target_link_libraries(public ${PROJECT_NAME})
add_custom_command(TARGET public PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/tests/resources $<TARGET_FILE_DIR:public>/resources)