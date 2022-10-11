# Inspired by
# https://github.com/KDAB/kdabtv/blob/master/Qt-Widgets-and-more/UnitTests/Tests/CMakeLists.txt

add_library(testhelpers STATIC
    )

target_include_directories(testhelpers
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    )

target_link_libraries(testhelpers
    PUBLIC
    Qt5::Test
    )

file(GLOB JSONS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.json
    )

add_custom_target(json-test-files ALL
    SOURCES ${JSONS}
    COMMAND echo ${JSONS}
    )

add_definitions(-DCURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")


list(JOIN SESSION_FILES "," C_SESSION_FILES)
add_definitions(-DSESSION_FILES="${C_SESSION_FILES}")

list(JOIN LOGGER_CONFIG_FILES "," C_LOGGER_CONFIG_FILES)
add_definitions(-DLOGGER_CONFIG_FILES="${C_LOGGER_CONFIG_FILES}")


function(SETUP_TESTS)
    foreach(_testname ${ARGN})
        add_test(NAME ${_testname}
                 COMMAND ${_testname})
        add_executable(${_testname}
            ${_testname}.h
            ${_testname}.cpp
            )
        target_include_directories(${_testname}
            PRIVATE
            $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${PROJECT_SOURCE_DIR}/src
            )
        target_link_libraries(${_testname}
            PUBLIC
            testhelpers
            zera-modulemanager-lib
            )
    endforeach()
endfunction()

SETUP_TESTS(
    test_jsonloggercontentloader
    )
