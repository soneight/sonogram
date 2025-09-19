set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

set( ${SON8PROJ}_LIBRARY_SOURCES
    compiler/compiler.cxx
    compiler/generate.cxx
    compiler/transfer.cxx
    tokenizer/token.cxx
    tokenizer/tokenizer.cxx
    utility/error.cxx
    utility/print.cxx
)
add_library( ${SON8PROJ}_library STATIC ${${SON8PROJ}_LIBRARY_SOURCES} )
target_include_directories( ${SON8PROJ}_library PUBLIC include )

include( FetchContent )

FetchContent_Declare(
    son8__mainland
    GIT_REPOSITORY https://github.com/soneight/mainland.git
    GIT_TAG        main
)
FetchContent_MakeAvailable( son8__mainland )

set( ${SON8PROJ}_SOURCES main.cxx )

add_executable( ${SON8PROJ} ${${SON8PROJ}_SOURCES} )
target_link_libraries( ${SON8PROJ} PRIVATE ${SON8PROJ}_library son8::mainland )

if ( CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/temp/CMakeLists.txt" )
    add_subdirectory(temp)
endif( )
