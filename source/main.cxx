#include <app.hxx>
// son8
// -- cxx
#include <son8/cxx/file.hxx>
#include <son8/cxx/flow.hxx>
#include <son8/cxx/text.hxx>
// -- mainland
#include <son8/main.hxx>



APP_PROC son8::main( Args args ) try {
   auto constexpr New_Line = '\n';
   cxx::cout << "sonogram:\n";
   cxx::cout << "-- Max File Size: " << app::Max::print( app::Max::File_Size ) << New_Line;
   cxx::cout << cxx::endl;
   // validate arguments
   if ( args.size( ) != 2 ) throw cxx::runtime_error{ "expect exactly one argument" };
   auto fileName = *( args.begin( ) + 1 );
   namespace fs = cxx::filesystem;
   app::Size fileSize = fs::file_size( fileName );
   if ( app::Max::File_Size < fileSize ) throw cxx::runtime_error{ "source file are too big" };
   // open file
   using File = cxx::ifstream;
   File sourceFile{ fileName, cxx::ios::binary };
   if ( not sourceFile.is_open( ) ) throw cxx::runtime_error{ "source file cannot be open" };
   // check last byte for new line character
   auto isFileEndsWithNewLine = []( File &file ) -> bool {
      auto lastByte = file.seekg( -1, std::ios::end ).get( );
      file.seekg( 0, std::ios::beg );
      return lastByte == New_Line;
   };
   if ( not isFileEndsWithNewLine( sourceFile ) ) throw cxx::runtime_error{ "source file does not ends with new line character" };
   // read whole file
   using ItStreamBuf = cxx::istreambuf_iterator< typename File::char_type >;
   std::string source{ ItStreamBuf{ sourceFile }, ItStreamBuf{ } };
   sourceFile.close( );
   // TODO process file
   std::cout << source << std::endl;
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
