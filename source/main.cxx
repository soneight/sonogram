#include "son8/cxx/file.hxx"
#include <app.hxx>

using Str = app::String;
static app::Source GlobalSourceWrite_;
static app::Ref< Str > Global_Source_Read = GlobalSourceWrite_.get( );

void son8::main( Args args ) try {
   auto constexpr New_Line = '\n';
   cxx::cout << "sonogram:\n";
   cxx::cout << "-- Max File Size: " << app::Max::print( app::Max::File_Size ) << New_Line;
   cxx::cout << "-- Max Line Length: " << app::Max::print( app::Max::Line_Length ) << New_Line;
   cxx::cout << cxx::endl;
   // validate arguments
   using app::Err;
   if ( args.size( ) != 2 ) throw Err{ "expect exactly one argument" };
   auto fileName = *( args.begin( ) + 1 );
   namespace fs = cxx::filesystem;
   app::Size fileSize = fs::file_size( fileName );
   if ( app::Max::File_Size < fileSize ) throw Err{ "source file size exceeds maximum limit" };
   // open file
   using InputFile = cxx::ifstream;
   InputFile sourceFile{ fileName, cxx::ios::binary };
   if ( not sourceFile.is_open( ) ) throw Err{ "source file cannot be open" };
   // check last byte for new line character
   auto isFileCorrect = []( app::Out< InputFile > file ) -> bool {
      auto lastByte = file.seekg( -1, cxx::ios::end ).get( );
      auto firstByte = file.seekg( 0, cxx::ios::beg ).peek( );
      return firstByte != New_Line && lastByte == New_Line;
   };
   if ( not isFileCorrect( sourceFile ) ) throw Err{ "source file begin or does not ends with new line character" };
   // read whole file
   using ItStreamBuf = cxx::istreambuf_iterator< typename InputFile::char_type >;
   GlobalSourceWrite_ = Str{ ItStreamBuf{ sourceFile }, ItStreamBuf{ } };
   sourceFile.close( );
   // validate maximum line length
   auto isLineLengthValid = []( app::Ref< Str > str, app::Long max ) -> bool {
      APP_ASSERT( app::Diff( 0 ) <= max && "app: max cannot be negative" );
      auto const end = str.end( );
      auto beg = str.begin( );
      auto it = beg;
      // NOTE: Backtracking helper
      // \ find the nearest newline and update `beg` iterator
      // \ to avoid re-scanning verified region in the future
      auto found = [&beg,&it]( auto const checkpoint ) -> bool {
         while ( beg < it ) {
            if ( *--it != New_Line ) continue;
            ++it; // it was new line so skip it
            beg = checkpoint + 1;
            return true;
         }
         return false;
      };
      // NOTE: Jump-skipping loop
      // \ short-circuit if jumps larger than buffer
      // \ fast-paths if landing exactly on new line
      // \ falls back to `found` backtracking helper
      while ( it < end ) {
         //if ( APP_CAST( app::Size, end - it ) <= max ) return true;
         if ( end - it <= max  ) return true;
         it += max;
         if ( *it == New_Line ) {
            beg = ++it;
            continue;
         }
         // `it` used here as local var replacement
         if ( not found( it ) ) return false;
      }
      return true;
   };
   app::Ref< Str > source = Global_Source_Read;
   if ( not isLineLengthValid( source, app::Max::Line_Length ) ) throw Err{ "source file contains lines with length exceeding maximum limit" };
   // tokens
   auto tokens = app::lex_tokens( source );
   for ( app::Token::Ref token : tokens ) {
      cxx::cout << app::to_string( token ) << New_Line;
   }
   cxx::cout << cxx::endl;
   switch ( tokens[0].kind ) {
      using k = app::Token::Kind;
      case k::Apptype_Program: break;
      default: throw Err{ "not correct application type token" };
   };
   // program
   auto program = gen_program( tokens );
   using OutputFile = cxx::ofstream;
   OutputFile programFile{ fs::current_path( ) / "temp" / ( program.fileName + ".cxx" ), cxx::ios::binary };
   if ( not programFile.is_open( ) ) throw Err{ "cannot open program file for writing" };
   programFile << program.mainHead << New_Line;
   programFile << program.mainBody << New_Line;
   programFile << program.mainFoot << New_Line;
   programFile << cxx::endl;
} catch ( app::Ref< cxx::exception > e ) {
   cxx::cerr << "son8::main: cxx::exception: " << e.what( ) << cxx::endl;
} catch ( ... ) {
   cxx::cerr << "... uncaught exception" << cxx::endl;
}

// GNU Affero General Public License v3.0 or later
// NO WARRANTY OF ANY KIND more details at <https://www.gnu.org/licenses/>
// SPDX-License-Identifier: AGPL-3.0-or-later
// app: `sonogram` Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
