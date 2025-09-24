set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
# mainland library as main function and its arguments wrapper
FetchContent_Declare(
    son8__mainland
    GIT_REPOSITORY https://github.com/soneight/mainland.git
    GIT_TAG        main
)
FetchContent_MakeAvailable( son8__mainland )
# header and source files for _faceimpl are added via add_subdirectory
add_library( ${SON8PROJ}_faceimpl STATIC )
add_subdirectory( face )
add_subdirectory( impl )
# main executable
add_executable( ${SON8PROJ} main.cxx )
target_link_libraries( ${SON8PROJ} PRIVATE ${SON8PROJ}_faceimpl son8::mainland )
# temporary files for testing purposes
if ( CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/temp/CMakeLists.txt" )
    add_subdirectory(temp)
endif( )
