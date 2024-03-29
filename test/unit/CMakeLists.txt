project( uni-common-unittests )

set( HEADERS
    ""
)

set( SOURCES
    "uni/common/ThreadTest.hpp"
    "uni/common/ThreadTest.cpp"
)

# treat_all_warnings_as_errors()

add_executable( ${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/common/public
    PRIVATE
        ${SOURCE_DIR}/common/test/unit/
        ${SOURCE_DIR}/external/gtest/googletest/
        ${SOURCE_DIR}/external/gtest/googlemock/
)

target_link_libraries( ${PROJECT_NAME}
    PRIVATE
        ${CMAKE_THREAD_LIBS_INIT}
        uni-common
        gtest
        gtest-main
        gmock
)

if( CODE_COVERAGE )
    add_custom_target( ${PROJECT_NAME}_run ALL
                        ${PROJECT_NAME}
                        DEPENDS ${PROJECT_NAME}
                        VERBATIM
                        USES_TERMINAL )
endif( )

install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

