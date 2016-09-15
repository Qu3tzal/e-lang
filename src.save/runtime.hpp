/*
	runtime.hpp

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

	Defines a simple runtime environment.
*/

#ifndef RUNTIME_HPP_INCLUDED
#define RUNTIME_HPP_INCLUDED

#include <map>
#include <string>

#include "datatypes.hpp"
#include "errors.hpp"

class Runtime
{
    public:
        Runtime();

        void clear();

        Value eval(Node* node);

    protected:
        Value getVariable(const std::string& identifier);

        /** Specials built-in operations. */
        Value program(std::vector<Node*> nodes);
        Value assign(std::vector<Node*> nodes);

        Value to_numeric(std::vector<Node*> nodes);
        Value to_string(std::vector<Node*> nodes);

        Value print(std::vector<Node*> nodes);
        Value input(std::vector<Node*> nodes);

        /** Maths built-in operations. */
        Value add(std::vector<Node*> nodes),
              sub(std::vector<Node*> nodes),
              mul(std::vector<Node*> nodes),
              div(std::vector<Node*> nodes),
              mod(std::vector<Node*> nodes),
              pow(std::vector<Node*> nodes),

              sin(std::vector<Node*> nodes),
              cos(std::vector<Node*> nodes),
              tan(std::vector<Node*> nodes),
              asin(std::vector<Node*> nodes),
              acos(std::vector<Node*> nodes),
              atan(std::vector<Node*> nodes),

              to_rad(std::vector<Node*> nodes),
              to_deg(std::vector<Node*> nodes),

              ln(std::vector<Node*> nodes),
              exp(std::vector<Node*> nodes),
              log10(std::vector<Node*> nodes);

    protected:
        std::map<std::string, Value> m_variables;
};

#endif // RUNTIME_HPP_INCLUDED
