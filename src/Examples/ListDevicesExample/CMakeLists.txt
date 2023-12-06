set(TARGET_NAME ListDevicesExample)

file(GLOB SOURCES "*.cpp")

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} PUBLIC OpenVisa)
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "Examples")

install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${INSTALL_BINDIR}/${CMAKE_BUILD_TYPE})
