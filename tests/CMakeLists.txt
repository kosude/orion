# ---
# public: testing the general public interface of Orion.

add_executable(public "public.c")
target_link_libraries(public ${PROJECT_NAME})