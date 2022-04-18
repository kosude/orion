# ---
# add source files to library output

set(SRC
	"internal/internal.c"
	"internal/internal.h"

	"lib/init.c"
	"lib/init.h"
)

if (BUILD_SHARED_LIBS)
	add_library(${PROJECT_NAME} SHARED ${SRC})
else()
	add_library(${PROJECT_NAME} STATIC ${SRC})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}/generated")