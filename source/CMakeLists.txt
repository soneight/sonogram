set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
# subject to removal in near future
# set( ${SON8PROJ}_LIBRARY_SOURCES
#     compiler/compiler.cxx
#     compiler/generate.cxx
#     compiler/transfer.cxx
#     tokenizer/token.cxx
#     tokenizer/tokenizer.cxx
#     utility/error.cxx
#     utility/print.cxx
# )
# add_library( ${SON8PROJ}_library STATIC ${${SON8PROJ}_LIBRARY_SOURCES} )
# target_compile_definitions( ${SON8PROJ}_library PRIVATE SON8_SONOGRAM_INCLUDE_OLD_IMPLEMENTATION )
# target_include_directories( ${SON8PROJ}_library PUBLIC include )
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
# target_compile_definitions( ${SON8PROJ} PRIVATE SON8_SONOGRAM_INCLUDE_OLD_IMPLEMENTATION )
target_link_libraries( ${SON8PROJ} PRIVATE ${SON8PROJ}_faceimpl son8::mainland )
# temporary files for testing purposes
if ( CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/temp/CMakeLists.txt" )
    add_subdirectory(temp)
endif( )
