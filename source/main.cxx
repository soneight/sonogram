#include <app.hxx>
// son8
#include <son8/cxx/file.hxx>
#include <son8/cxx/flow.hxx>
#include <son8/main.hxx>

APP_PROC son8::main( Args args ) try {
   cxx::cout << "sonogram" << cxx::endl;
   // validate arguments
   if ( args.size( ) != 2 ) throw cxx::runtime_error{ "expect exactly one argument" };
   auto fileName = *( args.begin( ) + 1 );
   namespace fs = cxx::filesystem;
   app::Size fileSize = fs::file_size( fileName );
   if ( app::Max::File_Size < fileSize ) throw cxx::runtime_error{ "source file are too big" };
} catch ( cxx::exception const &e ) {
   cxx::cerr << "son8::main: cxx::exception: " << e.what( ) << cxx::endl;
} catch ( ... ) {
   cxx::cerr << "... uncaught exception" << cxx::endl;
}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
