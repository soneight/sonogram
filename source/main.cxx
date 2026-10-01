// face
#include "impl/face/app.hxx"
// son8
#include <son8/cxx/file.hxx>
#include <son8/cxx/flow.hxx>
#include <son8/main.hxx>

using app::Text;
static app::Source GlobalSourceWrite_;
static app::Ref< Text > Global_Source_Read = GlobalSourceWrite_.get( );

void son8::main( Args args ) try {
   using namespace app;
   APP_DATA New_Line = '\n';
   cxx::cout << "sonogram:\n";
   cxx::cout << "-- Max File Size: " << Max::print( Max::File_Size ) << New_Line;
   cxx::cout << "-- Max Line Length: " << Max::print( Max::Line_Length ) << New_Line;
   cxx::cout << "-- Max Scoped Depth: " << Max::print( Max::Scoped_Depth ) << New_Line;
   cxx::cout << "-- Max Nested Depth: " << Max::print( Max::Nested_Depth ) << New_Line;
   cxx::cout << cxx::endl;
   // validate arguments
   if ( args.size( ) != 2 ) throw Error{ "expect exactly one argument" };
   auto fileName = args[1u]; // int range checks, unsigned does not
   namespace fs = cxx::filesystem;
   Size fileSize = fs::file_size( fileName );
   if ( Max::File_Size < fileSize ) throw Error{ "source file size exceeds maximum limit" };
   // open file
   using InputFile = cxx::ifstream;
   InputFile sourceFile{ fileName, cxx::ios::binary };
   if ( not sourceFile.is_open( ) ) throw Error{ "source file cannot be open" };
   // check last byte for new line character
   auto isFileCorrect = []( Out< InputFile > file ) -> bool {
      auto lastByte = file.seekg( -1, cxx::ios::end ).get( );
      auto firstByte = file.seekg( 0, cxx::ios::beg ).peek( );
      return firstByte != New_Line and lastByte == New_Line;
   };
   if ( not isFileCorrect( sourceFile ) ) throw Error{ "source file begin with or does not ends with new line character" };
   // read whole file
   {
      Text contents;
      contents.resize( fileSize );
      sourceFile.read( contents.data( ), APP_CAST( Long, fileSize ));
      sourceFile.close( );
      GlobalSourceWrite_ = cxx::move( contents );
   }
   // validate maximum line length
   auto isLineLengthValid = []( Ref< Text > str, Size max ) -> bool {
      APP_ASSERT( Max::is_valid_file_size( max ) and "app: max file size limit violation" );
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
      Diff diff = APP_CAST( Diff, max );
      // NOTE: Jump-skipping loop
      // \ short-circuit if jumps larger than buffer
      // \ fast-paths if landing exactly on new line
      // \ falls back to `found` backtracking helper
      while ( it < end ) {
         if ( end - it <= diff ) return true;
         it += diff;
         if ( *it == New_Line ) {
            beg = ++it;
            continue;
         }
         // `it` used here as local var replacement
         if ( not found( it ) ) return false;
      }
      return true;
   };
   Ref< Text > source = Global_Source_Read;
   if ( not isLineLengthValid( source, Max::Line_Length ) ) throw Error{ "source file contains lines with length exceeding maximum limit" };
   // tokens
   auto tokens = lex_tokens( source );
   for ( Ref< Token > token : tokens ) {
      cxx::cout << to_string( token ) << New_Line;
   }
   cxx::cout << cxx::endl;
   if ( tokens[0].kind != Token::Kind::Apptype_Program ) throw Error{ "not correct application type token" };
   // program
   auto program = gen_program( tokens );
   fs::path outputPath = fs::current_path( ) / "temp";
   fs::create_directory( outputPath );
   using OutputFile = std::ofstream;
   OutputFile programFile{ outputPath / ( program.fileName + ".cxx" ), cxx::ios::binary };
   if ( not programFile.is_open( ) ) throw Error{ "cannot open program file for writing" };
   programFile << program.include.gen( ) << New_Line;
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
// app: `sonogram` C++17 Programming Language Transpiler
// Ⓒ Copyright (C) 2026 Oleg'Ease'Kharchuk ᦒ
