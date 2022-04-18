# ---
# interface: testing the general Orion public interface

add_executable(interface "interface.c")
target_link_libraries(interface ${PROJECT_NAME})