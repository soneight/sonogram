#include <app/utility/error.hxx>

#include <stdexcept>

namespace app {

    void Error::throw_exception( ) const {
        throw std::runtime_error( message_ );
    }

    void Error::add( Message const &message ) {
        message_ += '\n' + message;
        --limit_;
        if ( limit_ == 0 ) throw_exception( );
    }

    bool Error::has_errors( ) const {
        return !message_.empty( );
    }

    void Error::check_exception( ) const {
        if ( has_errors( ) ) throw_exception( );
    }

} // namespace app

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
