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
        GIT_TAG        b66cbfb7a93669bbdadc1344537642412ce88047 # v0.0.1
    )
    fetchcontent_makeavailable( son8__mainland )
endif( )
message( STATUS "${APP}: found `son8__mainland` target" )
# -- c_header
if( NOT TARGET son8__c_header )
    include( FetchContent )
    message( STATUS "${APP}: FetchContent `soneight/c_header`" )
    fetchcontent_declare(
        son8__c_header
        GIT_REPOSITORY https://github.com/soneight/c_header.git
        GIT_TAG        d7e6e7188101d4af0738a6b3a67d47c180d8c559 # v0.1.1
    )
    fetchcontent_makeavailable( son8__c_header )
endif( )
message( STATUS "${APP}: found `son8__c_header` target" )
# -- cxx_unit
if( NOT TARGET son8__cxx_unit )
    include( FetchContent )
    message( STATUS "${APP}: FetchContent `soneight/cxx_unit`" )
    fetchcontent_declare(
        son8__cxx_unit
        GIT_REPOSITORY https://github.com/soneight/cxx_unit.git
        GIT_TAG        cfdeca7f25de2c1e55e28fc629d7961a0e87d625 # v0.0.1
    )
    fetchcontent_makeavailable( son8__cxx_unit )
endif( )
message( STATUS "${APP}: found `son8__cxx_unit` target" )

target_link_libraries( ${APP} PRIVATE ${APP_LIB} son8::mainland son8::c_header son8::cxx_unit )

# GNU Affero General Public License v3.0 or later
# NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
# SPDX-License-Identifier: AGPL-3.0-or-later
# app: `sonogram` Programming Language
# Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
