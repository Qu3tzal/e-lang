#include "lexer.hpp"
#include "errors.hpp"

namespace
{
	// Return true if the given char is in the given std::string.
	bool is_in_string(std::string str, char c)
	{
		return str.find(c) != std::string::npos;
	}

	// Return true if the given word is an operator.
	bool is_operator(const std::string& word)
	{
		return operatorsTable.find(word) != operatorsTable.end();
	}

    // Return true if the given word is a keyword.
	bool is_keyword(const std::string& word)
	{
		return keywordsTable.find(word) != keywordsTable.end();
	}

	// Return true if the given char is a bracket.
	bool is_bracket(char c)
	{
		return is_in_string("()", c);
	}

	// Return true if the given char is a whitespace.
	bool is_whitespace(char c)
	{
		return is_in_string("\t\r\f\n ", c);
	}

	// Return true if the given char is a delimiter.
	bool is_delimiter(char c)
	{
	    return is_whitespace(c) || is_bracket(c);
	}

	// Return true if the given char is a digit.
	bool is_digit(char c)
	{
		return is_in_string("0123456789", c);
	}

	// Return true if the given char is alpha.
	bool is_alpha(char c)
	{
	    c = std::toupper(c);
	    return is_in_string("ABCDEFGHIJKLMNOPQRSTUVWXYZ", c);
	}

	// Return true if the given char is alpha or digit.
	bool is_alphanum(char c)
	{
        return is_digit(c) || is_alpha(c);
	}
}

Lexer::Lexer(std::string input)
    : m_input(input)
    , m_currentTokenIndex(0)
{}

void Lexer::lex()
{
    Token current_token(TokenType::TT_NONE, "");
    bool is_token_complete(false), is_numeric(false), is_identifier(false), is_string(false), is_string_escaped(false);

    for(std::size_t i(0) ; i < m_input.size() ; ++i)
    {
        char current_char(m_input.at(i));

        // First of all : check for comments and skip them.
        if(current_char == '#' && !is_string)
        {
            while(current_char != '\n' && i < m_input.size())
            {
                ++i;
                current_char = m_input.at(i);
            }
        }

        // No current token, let's see what type it is.
        if(current_token.value.empty() && !is_numeric && !is_identifier && !is_string)
        {
            if(std::isdigit(current_char))
            {
                is_numeric = true;
                current_token.value += current_char;
                current_token.type = TokenType::TT_NUMERIC;
            }
            else if(is_whitespace(current_char))
            {
                // Do nothing.
            }
            else
            {
                switch(current_char)
                {
                    case '(':
                        current_token.value = "(";
                        current_token.type = TokenType::TT_LEFT_PAR;

                        is_token_complete = true;
                        break;
                    case ')':
                        current_token.value = ")";
                        current_token.type = TokenType::TT_RIGHT_PAR;

                        is_token_complete = true;
                        break;
                    case '"':
                        current_token.value = "";
                        current_token.type = TokenType::TT_STRING;

                        is_string = true;
                        break;
                    default:
                        is_identifier = true;
                        current_token.value += current_char;
                        current_token.type = TokenType::TT_IDENTIFIER;
                        break;
                }
            }
        }
        // We are in string so we catch everything.
        else if(is_string)
        {
            if(current_char == '"' && !is_string_escaped)
                is_token_complete = true;
            else if(current_char == '\\' && !is_string_escaped)
                is_string_escaped = true;
            else
            {
                if(is_string_escaped)
                {
                    switch(current_char)
                    {
                        case '"':
                            current_token.value += '"';
                            break;
                        case 'n':
                            current_token.value += '\n';
                            break;
                        case 't':
                            current_token.value += '\t';
                            break;
                        case 'r':
                            current_token.value += '\r';
                            break;
                        case 'f':
                            current_token.value += '\f';
                            break;
                        case 'v':
                            current_token.value += '\v';
                            break;
                        default:
                            current_token.value += current_char;
                            break;
                    }

                    is_string_escaped = false;
                }
                else
                    current_token.value += current_char;
            }
        }
        // Random character except whitespaces and brackets.
        else if(!is_whitespace(current_char) && !is_bracket(current_char))
        {
            // If parsing a numeric and get something else than digit or dot.
            if(is_numeric && !is_digit(current_char) && current_char != '.')
            {
                is_token_complete = true;
                --i;
            }
            else
            {
                current_token.value += current_char;
            }
        }
        // Brackets while we are parsing a numeric/identifier.
        else if(is_bracket(current_char))
        {
            is_token_complete = true;
            --i;
        }
        // Whitespace.
        else
        {
            is_token_complete = true;
        }

        // Push token and reset flags if complete.
        if(is_token_complete)
        {
            // Do not trim strings.
            if(current_token.type != TokenType::TT_STRING)
                current_token.value = string_utils::trim(current_token.value);

            // To tell difference between operators and identifiers.
            if(current_token.type == TokenType::TT_IDENTIFIER && is_operator(current_token.value))
                current_token.type = TokenType::TT_OPERATOR;

            // To tell difference between keywords (non-operators) and identifiers.
            if(current_token.type == TokenType::TT_IDENTIFIER && is_keyword(current_token.value))
                current_token.type = keywordsTable.at(current_token.value);

            if(!current_token.value.empty())
                m_tokens.push_back(current_token);

            current_token.value = "";
            current_token.type = TokenType::TT_NONE;
            is_token_complete = is_numeric = is_identifier = is_string = is_string_escaped = false;
        }
    }

    if(!current_token.value.empty())
        m_tokens.push_back(current_token);

    #ifdef DEBUG_LEXER
    for(Token t : m_tokens)
        std::cout << "\ttype: " << string_utils::from(t.type) << std::endl;
    #endif // DEBUG_LEXER
}

Token Lexer::getLook()
{
    return m_tokens[m_currentTokenIndex];
}

void Lexer::getNext()
{
    if(m_tokens.size() > m_currentTokenIndex)
        ++m_currentTokenIndex;
    else
        errors::lexerError("token list is empty");
}

bool Lexer::isEmpty()
{
    return m_currentTokenIndex >= m_tokens.size();
}
