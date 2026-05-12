cmake_minimum_required( VERSION 3.20...3.27 )
project( sonogram VERSION 0.0.0 LANGUAGES CXX )
set( APP ${PROJECT_NAME} )
set( APP_LIB app__${APP} )
add_library( ${APP_LIB} INTERFACE )
# private configuration under source directory
add_subdirectory( source )
# configuration
target_include_directories( ${APP_LIB} INTERFACE source/include )
target_compile_options( ${APP_LIB} INTERFACE $<$<CXX_COMPILER_ID:MSVC>:/utf-8 /permissive- /Zc:__cplusplus> )

# GNU Affero General Public License v3.0 or later
# NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
# SPDX-License-Identifier: AGPL-3.0-or-later
# app: `sonogram` C++17 Programming Language Transpiler
# Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
