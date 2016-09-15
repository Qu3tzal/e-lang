/*
	lexer.hpp

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

	Defines a simple lexer.
*/

#ifndef LEXER_HPP_INCLUDED
#define LEXER_HPP_INCLUDED

#include <map>
#include <vector>
#include <sstream>
#include <functional>

#include "datatypes.hpp"

/// Uncomment for debug.
//#define DEBUG_LEXER

class Lexer
{
	public:
		Lexer(std::string input);

		void lex();

		Token getLook();
		void getNext();

		bool isEmpty();

	protected:
	    std::string m_input;

		std::vector<Token> m_tokens;
		std::size_t m_currentTokenIndex;
};

#endif // LEXER_HPP_INCLUDED