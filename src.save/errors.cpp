#include "errors.hpp"

namespace errors
{
    void abort()
    {
        std::exit(1);
    }

    void lexerError(const std::string& message)
    {
        throw lexer_exception("Lexer error: " + message);
    }

    void parserError(const std::string& message)
    {
        throw parser_exception("Parser error: " + message);
    }

    void runtimeError(const std::string& message)
    {
        throw runtime_exception("Runtime error: " + message);
    }
}
