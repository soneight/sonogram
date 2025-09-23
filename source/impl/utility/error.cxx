// own header
#include <app/utility/error.hxx>
// app headers
#include <app/limits.hxx>
// std headers
#include <stdexcept>

namespace app {

    void Error::add( Message const &message ) {
        message_ += '\n' + message;
        --limit_;
        if ( limit_ == 0 ) throw_exception( );
    }

    void Error::reset( ) {
        message_.clear( );
        limit_ = limits::Max::Error_Count;
    }

    void Error::check_exception( ) const {
        if ( has_errors( ) ) throw_exception( );
    }

    bool Error::has_errors( ) const {
        return !message_.empty( );
    }

    void Error::throw_exception( ) const {
        throw std::runtime_error( message_ );
    }

} // namespace app

// Ⓒ 2025 Oleg'Ease'Kharchuk ᦒ
