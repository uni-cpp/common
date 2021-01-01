project( uni-common )

set( HEADERS
    "public/uni/common/Constants.hpp"
    "public/uni/common/Defines.hpp"
    "public/uni/common/Queue.hpp"
    "public/uni/common/Runnable.hpp"
    "public/uni/common/Thread.hpp"
    "public/uni/common/ThreadPool.hpp"
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
        ${SOURCE_DIR}/common/public
    PRIVATE
        ${SOURCE_DIR}/common/src
)

target_link_libraries( ${PROJECT_NAME}
    PUBLIC
        ${CMAKE_THREAD_LIBS_INIT}
)

set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    PUBLIC_HEADER "${HEADERS}"
)

install( TARGETS ${PROJECT_NAME}
    ARCHIVE DESTINATION usr/lib
    LIBRARY DESTINATION usr/lib
    PUBLIC_HEADER DESTINATION usr/include
    RUNTIME DESTINATION usr/bin
)
