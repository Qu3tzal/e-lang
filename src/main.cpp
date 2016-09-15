/*
	main.cpp

	The MIT License (MIT)

	Copyright (c) 2013-2014 Maxime Alvarez

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

	Main function of the program.
*/

#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>

#include "datatypes.hpp"
#include "string_utils.hpp"

#include "args.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "runtime.hpp"

/// Uncomment the next line for global debug.
//#define GLOBAL_DEBUG

int interactive_loop()
{
    /** Welcome. */
    std::cout << "\t-*- e-lang -*-" << std::endl;
    std::cout << "Enter \":help\" if you are lost." << std::endl;

    // The runtime has to be outside the loop if we want it to be consistent.
    Runtime runtime;

    do
    {
        /** Prompt (get line and trim). */
        std::string line("");

        std::cout << std::endl << "> ";
        std::getline(std::cin, line);

        line = string_utils::trim(line);

        // Skip empty lines.
        if(line.empty())
            continue;

        /** Specials lines for the runtime. */
        if(line.at(0) == ':')
        {
            if(line == ":quit")
                break;
            else if(line == ":clear_runtime")
            {
                runtime.clear();
                std::cout << "Runtime has been cleared." << std::endl;
                continue;
            }
            else if(line == ":help")
            {
                std::cout << "Interactive loop help." << std::endl;
                std::cout << "\t" << ":quit -> quit the interactive loop" << std::endl;
                std::cout << "\t" << ":clear_runtime -> clear all variables assigned" << std::endl;
                std::cout << "\t" << ":help -> display this help" << std::endl;

                continue;
            }
        }

        Node* ast_root = nullptr;

        try
        {
            /** Lex (cut the input source in a tokens list). */
            #ifdef GLOBAL_DEBUG
                std::cout << "Lexing..." << std::endl;
            #endif // GLOBAL_DEBUG
            Lexer lexer(line);
            lexer.lex();

            /** Parse (build the AST from the tokens list). */
            #ifdef GLOBAL_DEBUG
                std::cout << "Parsing..." << std::endl;
            #endif // GLOBAL_DEBUG
            Parser parser(lexer);
            ast_root = parser.parse();

            /** Evaluation of the AST tree. */
            #ifdef GLOBAL_DEBUG
                std::cout << "AST evaluation..." << std::endl;
            #endif // GLOBAL_DEBUG
            Value result = runtime.eval(ast_root);

            if(result.type == ValueType::VT_NUMERIC)
                std::cout << result.numeric << std::endl;
            else if(result.type == ValueType::VT_STRING)
                std::cout << result.string << std::endl;
        }
        catch(std::exception& e)
        {
            std::cerr << e.what() << "." << std::endl;

            // Clear the runtime in case of errors.
            // Because even if we crashed, operations may have be performed on the runtime environment.
            runtime.clear();
            std::cerr << "Runtime has been cleared." << std::endl;
        }

        if(ast_root)
            delete ast_root;
    }while(true);

	return 0;
}

int execute_from_file(const std::string& filepath)
{
    Node* ast_root = nullptr;

    try
    {
        /** Get file content. */
        std::string filecontent("");
        std::ifstream inputfile(filepath.c_str());

        if(!inputfile)
            errors::runtimeError("cannot open file : \"" + filepath + "\"");

        std::string line("");
        while(std::getline(inputfile, line))
        {
            // We add '\n' to skip comments.
            // The lexer won't have to deal with differents : LF, CR or CRLF.
            filecontent += line + '\n';
        }

        /** Lex (cut the input source in a tokens list). */
        #ifdef GLOBAL_DEBUG
            std::cout << "Lexing..." << std::endl;
        #endif // GLOBAL_DEBUG
        Lexer lexer(filecontent);
        lexer.lex();

        /** Parse (build the AST from the tokens list). */
        #ifdef GLOBAL_DEBUG
            std::cout << "Parsing..." << std::endl;
        #endif // GLOBAL_DEBUG
        Parser parser(lexer);
        ast_root = parser.parse();

        /** Evaluation of the AST tree. */
        #ifdef GLOBAL_DEBUG
            std::cout << "AST evaluation..." << std::endl;
        #endif // GLOBAL_DEBUG
        Runtime runtime;
        runtime.eval(ast_root);
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << "." << std::endl;

        if(ast_root)
            delete ast_root;

        return 1;
    }

    if(ast_root)
        delete ast_root;

    return 0;
}

int main(int argc, char* argv[])
{
	std::map<std::string, std::string> args = map_args(parse_args(argc, argv));

	if(!args["file"].empty())
        return execute_from_file(args["file"]);
    else
        return interactive_loop();
}
