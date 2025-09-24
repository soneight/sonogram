target_sources( ${SON8PROJ}_faceimpl PRIVATE
    app.hxx
    app/alias.hxx
    app/lexer.hxx
    app/lexer/token.hxx
    app/limits.hxx
    app/utility/error.hxx
    app/utility/print.hxx
    dirty/parser.hxx
)
target_include_directories(${SON8PROJ}_faceimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
