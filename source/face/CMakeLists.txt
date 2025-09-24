add_library( ${SON8PROJ}_faceimpl_interface INTERFACE
    app.hxx
    app/alias.hxx
    app/lexer.hxx
    app/lexer/token.hxx
    app/limits.hxx
    app/utility/error.hxx
    app/utility/print.hxx
)
target_link_libraries( ${SON8PROJ}_faceimpl INTERFACE ${SON8PROJ}_faceimpl_interface )
