#ifndef FACE_PROGRAM_HXX
#define FACE_PROGRAM_HXX

#include "alias/text.hxx"
#include "alias/data.hxx"
#include "token.hxx"

namespace app {
   // include `std` headers invariant helper
   class Include {
      APP_DATA Flags_N = Unt0{ 1u };
      using Flags_ = Bits< Flags_N >;
      Flags_ flags_{ };
      using Names_ = Flat< View, Flags_N >;
      APP_DATA names_ = Names_{{ "#include <iostream>\n"sv }};
   public:
      enum class Header : Unt0 {
         Bit_Hiostream = 0u,
      };
      APP_DATA pos( Header header ) -> Unt0 { return APP_CAST( Unt0, header ); }
      APP_PROC add( Header header ) { flags_.set( pos( header )); }
      // generate string of include headers based of flags state
      APP_FUNC gen( ) const -> Text {
         if ( not flags_.any( )) return Text{ };
         Text text;
         for ( Unt0 i = 0; i < Flags_N; ++i ) {
            if ( flags_[i] ) text.append( names_[i] );
         }
         return text;
      }
   };

   // program
   struct Program final {
      // states
      enum class State : Unt0 {
         Global, // program header (program name with program keyword)
         Body, // main function body
         Init, // variable initialization state
      };
      // data members
      State state{ };
      Text fileName{ };
      Text mainHead;
      Text mainBody{ };
      Text mainFoot;
      Include include;
      Program( )
      : fileName{ "program" }
      , mainHead{ "int main( ) {" }
      , mainFoot{ "}" } {
         // TODO: maybe only when `-echo` encountered do it
         // \ for now always include for `-echo`
         include.add( Include::Header::Bit_Hiostream );
      }
   };

   using Tokens = Grow< Token >;
   APP_FUNC gen_program( Ref< Tokens > tokens ) -> Program;

}

#endif//FACE_PROGRAM_HXX
