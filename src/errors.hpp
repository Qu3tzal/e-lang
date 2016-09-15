/*
	errors.hpp

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

	Defines errors functions.
*/

#ifndef ERRORS_HPP_INCLUDED
#define ERRORS_HPP_INCLUDED

#include <iostream>
#include <cstdlib>
#include <exception>

namespace errors
{
    class runtime_exception : public std::exception
    {
        public:
            runtime_exception(const std::string& message)
                : m_what(message)
            {}

            const char* what() const noexcept(true)
            {
                return m_what.c_str();
            }

        protected:
            std::string m_what;
    };

    typedef runtime_exception lexer_exception;
    typedef runtime_exception parser_exception;

    void abort();

    void lexerError(const std::string& message);
    void parserError(const std::string& message);
    void runtimeError(const std::string& message);
}

#endif // ERRORS_HPP_INCLUDED
