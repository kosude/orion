# ---
# dependencies

set(DEPENDENCIES_DIR "${PROJECT_SOURCE_DIR}/tests/deps")

set(ZML_USE_FLOATS ON CACHE BOOL "" FORCE)		# no docs
add_subdirectory("${DEPENDENCIES_DIR}/zetaml" "${DEPENDENCIES_DIR}/zetaml/build")

# ---
# interface: testing the general Orion public interface

add_executable(interface "interface.c")
target_link_libraries(interface ${PROJECT_NAME} zetaml)