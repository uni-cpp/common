project( uni-common )

set( HEADERS
    "include/uni/common/BaseListener.hpp"
    "include/uni/common/Constants.hpp"
    "include/uni/common/Defines.hpp"
    "include/uni/common/Queue.hpp"
    "include/uni/common/Runnable.hpp"
    "include/uni/common/Thread.hpp"
    "include/uni/common/ThreadPool.hpp"
)

set( SOURCES
    "src/uni/common/Thread.cpp"
    "src/uni/common/ThreadPool.cpp"
)

treat_all_warnings_as_errors( )

add_library( ${PROJECT_NAME} SHARED
    ${HEADERS}
    ${SOURCES}
)

target_include_directories( ${PROJECT_NAME}
    PUBLIC
        ${SOURCE_DIR}/common/include
    PRIVATE
        ${SOURCE_DIR}/common/src
)

target_link_libraries( ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
)

install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# The trailing slash is important
install( DIRECTORY ${CMAKE_INSTALL_INCLUDEDIR}/
         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

add_subdirectory( test )

if( CODE_COVERAGE )
    generate_coverage( ${PROJECT_NAME} ${PROJECT_NAME}-unittests)

    install( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${COVERAGE_OUTPUT_DIR_NAME}/
             DESTINATION ${CMAKE_INSTALL_DOCDIR}/uni/common
    )
endif( )

