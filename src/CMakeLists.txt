# ---
# add source files to library output

set(SRC
	"test.c"
	"test.h"
)

if (BUILD_SHARED_LIBS)
	add_library(${PROJECT_NAME} SHARED ${SRC})
else()
	add_library(${PROJECT_NAME} STATIC ${SRC})
endif()