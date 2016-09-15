#include "datatypes.hpp"

template<>
std::string string_utils::from(TokenType type)
{
    if(type == TokenType::TT_NUMERIC)
        return "numeric";
    else if(type == TokenType::TT_IDENTIFIER)
        return "identifier";
    else if(type == TokenType::TT_OPERATOR)
        return "operator";
    else if(type == TokenType::TT_STRING)
        return "string";
    else if(type == TokenType::TT_LEFT_PAR)
        return "left parenthesis";
    else if(type == TokenType::TT_RIGHT_PAR)
        return "right parenthesis";
    else if(type == TokenType::TT_NONE)
        return "none";

    return "";
}
