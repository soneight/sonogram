set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )
# app
add_executable( ${APP} )
target_sources( ${APP} PRIVATE main.cxx )

# son8 libraries
# -- mainland
if( NOT TARGET son8__mainland )
    include( FetchContent )
    message( STATUS "${APP}: FetchContent `soneight/mainland`" )
    fetchcontent_declare(
        son8__mainland
        GIT_REPOSITORY https://github.com/soneight/mainland.git
        GIT_TAG        8145becfc18b4c5968ddd61d19540d5a64ec282f # v0.1.1
    )
    fetchcontent_makeavailable( son8__mainland )
endif( )
message( STATUS "${APP}: found `son8__mainland` target" )
# -- core_lib
if( NOT TARGET son8__core_lib )
    include( FetchContent )
    message( STATUS "${APP}: FetchContent `soneight/core_lib`" )
    fetchcontent_declare(
        son8__core_lib
        GIT_REPOSITORY https://github.com/soneight/core_lib.git
        GIT_TAG        85a4e685d0e5b245f8cbb94e625a8ab1365c5f16 # v0.0.5
    )
    fetchcontent_makeavailable( son8__core_lib )
endif( )
message( STATUS "${APP}: found `son8__core_lib` target" )

target_link_libraries( ${APP} PRIVATE ${APP_LIB} son8::mainland son8::c_header son8::cxx_unit son8::core_lib )

# GNU Affero General Public License v3.0 or later
# NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
# SPDX-License-Identifier: AGPL-3.0-or-later
# app: `sonogram` Programming Language
# Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
