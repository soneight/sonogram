cmake_minimum_required( VERSION 3.8...3.27 )
project( sonogram VERSION 0.0.0 LANGUAGES CXX )
# prefix for targets
set( SON8PROJ son8__${PROJECT_NAME} )
# for son8::mainland library
include( FetchContent )
add_subdirectory( source )
