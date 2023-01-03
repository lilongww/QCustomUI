add_subdirectory(QCustomUi)

option(BUILD_EXAMPLES "Build example applications." FALSE)
if(BUILD_EXAMPLES)
add_subdirectory(Examples)
endif(BUILD_EXAMPLES)

option(BUILD_TESTS "Build test applications." FALSE)
if(BUILD_TESTS)
    add_subdirectory(Tests)
endif(BUILD_TESTS)

option(BUILD_DESIGNER "Build designer plugin." FALSE)
if(BUILD_DESIGNER)
add_subdirectory(Designer)
endif(BUILD_DESIGNER)