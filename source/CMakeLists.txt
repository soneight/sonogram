set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

include( FetchContent )

FetchContent_Declare(
    son8__mainland
    GIT_REPOSITORY https://github.com/soneight/mainland.git
    GIT_TAG        main
)
FetchContent_MakeAvailable( son8__mainland )

set( ${SON8PROJ}_SOURCES
    main.cxx
    compiler/generate.cxx
    compiler/transfer.cxx
)

add_executable( ${SON8PROJ} ${${SON8PROJ}_SOURCES} )
target_link_libraries( ${SON8PROJ} PRIVATE son8::mainland )
target_include_directories( ${SON8PROJ} PRIVATE include )

if ( CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/temp/CMakeLists.txt" )
    add_subdirectory(temp)
endif( )
