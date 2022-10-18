set(TARGET_NAME AdlTest)

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

# OpenVisa::decode<bool>
function(bool_test NUM IN EXP)
    add_test(NAME "OpenVisa::decode<bool> ${NUM}" COMMAND ${TARGET_NAME} "0" ${IN} WORKING_DIRECTORY ${INSTALL_BINDIR}/Debug)
    set_tests_properties("OpenVisa::decode<bool> ${NUM}" PROPERTIES PASS_REGULAR_EXPRESSION ${EXP})
endfunction(bool_test)

bool_test("1_1" "ON\n" "true")
bool_test("1_2" "ON\r\n" "true")
bool_test("2_1" "OFF\n" "false")
bool_test("2_2" "OFF\r\n" "false")
bool_test("3_1" "1\n" "true")
bool_test("3_2" "1\r\n" "true")
bool_test("4_1" "+1\n" "true")
bool_test("4_2" "+1\r\n" "true")
bool_test("5_1" "0\n" "false")
bool_test("5_2" "0\r\n" "false")
bool_test("6" "NFF\n" "exception")

# OpenVisa::decode<double>

function(double_test NUM IN EXP)
    add_test(NAME "OpenVisa::decode<double> ${NUM}" COMMAND ${TARGET_NAME} "1" ${IN} WORKING_DIRECTORY ${INSTALL_BINDIR}/Debug)
    set_tests_properties("OpenVisa::decode<double> ${NUM}" PROPERTIES PASS_REGULAR_EXPRESSION ${EXP})
endfunction(double_test)

double_test("1_1" "0.1\n" "0.1")
double_test("1_2" "0.1\r\n" "0.1")
double_test("2_1" "-0.1\n" "-0.1")
double_test("2_2" "-0.1\r\n" "-0.1")
double_test("3_1" "+0.1\n" "0.1")
double_test("3_2" "+0.1\r\n" "0.1")
double_test("4" "100A" "100")
double_test("5" "1.8E-01" "0.18")
double_test("6" "1.8e-01" "0.18")

# OpenVisa::decode<int>
function(int_test NUM IN EXP)
    add_test(NAME "OpenVisa::decode<int> ${NUM}" COMMAND ${TARGET_NAME} "2" ${IN} WORKING_DIRECTORY ${INSTALL_BINDIR}/Debug)
    set_tests_properties("OpenVisa::decode<int> ${NUM}" PROPERTIES PASS_REGULAR_EXPRESSION ${EXP})
endfunction(int_test)

int_test("1_1" "1\n" "1")
int_test("1_2" "1\r\n" "1")
int_test("2_1" "-1\n" "-1")
int_test("2_2" "-1\r\n" "-1")
int_test("3_1" "+1\n" "1")
int_test("3_2" "+1\r\n" "1")
int_test("4" "100.100" "100")

# OpenVisa::decode<EnumTest>
function(enum_test NUM IN EXP)
    add_test(NAME "OpenVisa::decode<EnumTest> ${NUM}" COMMAND ${TARGET_NAME} "3" ${IN} WORKING_DIRECTORY ${INSTALL_BINDIR}/Debug)
    set_tests_properties("OpenVisa::decode<EnumTest> ${NUM}" PROPERTIES PASS_REGULAR_EXPRESSION ${EXP})
endfunction(enum_test)

enum_test("1" "MLIN\n" "0")
enum_test("2" "MLOG\n" "1")
enum_test("3" "PHAS\n" "2")
enum_test("4" "UPH\n" "3")
enum_test("5" "IMAG\n" "4")
enum_test("6" "REAL\n" "5")
enum_test("7" "POL\n" "6")
enum_test("8" "SMIT\n" "7")
enum_test("9" "SADM\n" "8")
enum_test("10" "SWR\n" "9")
enum_test("11" "GDEL\n" "10")
enum_test("12" "KELV\n" "11")
enum_test("13" "FAHR\n" "12")
enum_test("14" "CELS\r\n" "13")
enum_test("15" "PPHase\n" "14")
enum_test("16" "PP\n" "exception")
