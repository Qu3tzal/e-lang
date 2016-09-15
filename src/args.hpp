/*
	args.hpp

	The MIT License (MIT)

	Copyright (c) 2014 Maxime Alvarez.

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

	Useful functions to parse the command line arguments.
*/

#ifndef ARGS_HPP_INCLUDED
#define ARGS_HPP_INCLUDED

#include <vector>
#include <string>
#include <sstream>
#include <map>

// Undef this if you are not using C++11.
#define CPP_11

// Structure containing one argument data.
struct argument
{
	std::string name;
	std::string value;
};

// Parse the arguments of the program.
// Accept arguments of form : -variable=value
// Arguments like -flag are considered as -flag=true
std::vector<argument> parse_args(int argc, char* argv[])
{
	std::vector<argument> arguments;

	for(int i(0) ; i < argc ; ++i)
	{
		char* arg = argv[i];
		std::stringstream ss(arg);

        std::string arg_var("");
        std::string arg_value("");

        std::getline(ss, arg_var, '=');
        std::getline(ss, arg_value, '\n');

        argument current_arg;

		if(arg_value == "")
		{
			current_arg.name = arg_var;
			current_arg.value = "true";
		}
		else
		{
			current_arg.name = arg_var;
			current_arg.value = arg_value;
		}

		arguments.push_back(current_arg);
	}

	return arguments;
}

// Convert the vector given by parse_args into a map.
std::map<std::string, std::string> map_args(const std::vector<argument>& arguments)
{
	std::map<std::string, std::string> args_map;

	#ifdef CPP_11
	for(const argument& arg : arguments)
	{
	#else
	for(int i(0) ; i < arguments.size() ; ++i)
	{
		argument arg = arguments.at(i);
	#endif // CPP_11
		args_map[arg.name] = arg.value;
	}

	return args_map;
}

#endif // ARGS_HPP_INCLUDED
