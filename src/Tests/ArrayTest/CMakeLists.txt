set(TARGET_NAME ArrayTest)

file(GLOB SOURCES "*.cpp")
source_group("Sources" FILES ${SOURCES})

add_executable(${TARGET_NAME} ${SOURCES})

target_link_libraries(${TARGET_NAME} PUBLIC OpenVisa)
set_property(TARGET ${TARGET_NAME} PROPERTY FOLDER "Tests")

install(TARGETS ${TARGET_NAME}
    CONFIGURATIONS Debug
    RUNTIME DESTINATION ${INSTALL_BINDIR}/Debug
)

install(TARGETS ${TARGET_NAME}
    CONFIGURATIONS Release
    RUNTIME DESTINATION ${INSTALL_BINDIR}/Release
)

# OpenVisa::AsciiArray<int>
function(int_test NUM OFFSET IN EXP)
    add_test(NAME "OpenVisa::AsciiArray<int> ${NUM}" COMMAND ${TARGET_NAME} "0" ${OFFSET} ${IN} WORKING_DIRECTORY ${INSTALL_BINDIR}/Debug)
    set_tests_properties("OpenVisa::AsciiArray<int> ${NUM}" PROPERTIES PASS_REGULAR_EXPRESSION ${EXP})
endfunction(int_test)

int_test("1_1" "0" "123,456,789\n" "123 456 789")
int_test("1_2" "4" "abc,123,456,789\n" "123 456 789")
int_test("1_3" "0" "1,2,3,\n" "1 2 3")

# OpenVisa::AsciiArray<double>
function(double_test NUM OFFSET IN EXP)
    add_test(NAME "OpenVisa::AsciiArray<double> ${NUM}" COMMAND ${TARGET_NAME} "1" ${OFFSET} ${IN} WORKING_DIRECTORY ${INSTALL_BINDIR}/Debug)
    set_tests_properties("OpenVisa::AsciiArray<double> ${NUM}" PROPERTIES PASS_REGULAR_EXPRESSION ${EXP})
endfunction(double_test)

double_test("1_1" "0" "123.1,456.256,789.993\n" "123.1 456.256 789.993")
double_test("1_2" "4" "abc,123.1,456.256,789.993\n" "123.1 456.256 789.993")
double_test("1_3" "0" "1,2,3,\n" "1 2 3")

# OpenVisa::BinaryArray<double>
function(bin_int_test NUM EXP)
    add_test(NAME "OpenVisa::BinaryArray<int> ${NUM}" COMMAND ${TARGET_NAME} "2" WORKING_DIRECTORY ${INSTALL_BINDIR}/Debug)
    set_tests_properties("OpenVisa::BinaryArray<int> ${NUM}" PROPERTIES PASS_REGULAR_EXPRESSION ${EXP})
endfunction(bin_int_test)

bin_int_test("1_1" "1 2")