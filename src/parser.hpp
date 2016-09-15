/*
	parser.hpp

	The MIT License (MIT)

	Copyright (c) 2014 Maxime Alvarez

	Permission is hereby granted, free of charge, to any person obtaining a copy of
	this software and associated documentation files (the "Software"), to deal in
	the Software without restriction, including without limitation the rights to
	use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
	the Software, and to permit persons to whom the Software is furnished to do so,
	subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
	FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
	COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
	IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
	CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	Defines a simple parser.
*/

#ifndef PARSER_HPP_INCLUDED
#define PARSER_HPP_INCLUDED

#include <vector>

#include "lexer.hpp"

class Parser
{
    public:
        Parser(Lexer& lexer);

        Node* parse();

    protected:
        Token match(TokenType type);
        Token match(const std::string& value);

        std::string getIdentifier();
        std::string getString();
        float getNumeric();
        Operator getOperator();
        std::string getNull();
        Node* getExpression();
        std::vector<Node*> getExpressionList();

    protected:
        Lexer m_lexer;
};

#endif // PARSER_HPP_INCLUDED
