/*
	datatypes.hpp

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

	Defines the data structures used between differents modules.
*/

#ifndef DATATYPES_HPP_INCLUDED
#define DATATYPES_HPP_INCLUDED

#ifndef M_PI
    #define M_PI 3.1415926535
#endif

#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>

#include "string_utils.hpp"
#include "errors.hpp"

enum class TokenType
{
	TT_NUMERIC,
	TT_IDENTIFIER,
	TT_NULL,
	TT_OPERATOR,
	TT_STRING,
	TT_LEFT_PAR,
	TT_RIGHT_PAR,

	TT_NONE
};

template<>
std::string string_utils::from(TokenType type);

struct Token
{
	Token(TokenType type = TokenType::TT_NONE, std::string value = "")
        : type(type), value(value)
	{};

	TokenType type;
	std::string value;
};

enum class Operator
{
    /** Special built-in operators. */
    OP_PROGRAM,
    OP_ASSIGN,
    OP_TO_NUMERIC,
    OP_TO_STRING,
    OP_PRINT,
    OP_INPUT,

    /** Maths built-in operators. */
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    OP_POW,
    OP_SIN,
    OP_COS,
    OP_TAN,
    OP_ACOS,
    OP_ASIN,
    OP_ATAN,
    OP_TO_RAD,
    OP_TO_DEG,
    OP_LN,
    OP_EXP,
    OP_LOG10,

    OP_NONE
};

const std::map<std::string, Operator> operatorsTable =
{
    /** Special built-in operators. */
    {"program", Operator::OP_PROGRAM},
    {"assign", Operator::OP_ASSIGN},
    {"to_numeric", Operator::OP_TO_NUMERIC},
    {"to_string", Operator::OP_TO_STRING},
    {"print", Operator::OP_PRINT},
    {"input", Operator::OP_INPUT},

    /** Maths built-in operators. */
    {"+", Operator::OP_ADD},
    {"-", Operator::OP_SUB},
    {"*", Operator::OP_MUL},
    {"/", Operator::OP_DIV},
    {"%", Operator::OP_MOD},
    {"^", Operator::OP_POW},
    {"sin", Operator::OP_SIN},
    {"cos", Operator::OP_COS},
    {"tan", Operator::OP_TAN},
    {"acos", Operator::OP_ACOS},
    {"asin", Operator::OP_ASIN},
    {"atan", Operator::OP_ATAN},
    {"to_rad", Operator::OP_TO_RAD},
    {"to_deg", Operator::OP_TO_DEG},
    {"ln", Operator::OP_LN},
    {"exp", Operator::OP_EXP},
    {"log10", Operator::OP_LOG10}
};

enum class ValueType
{
    VT_NUMERIC,
    VT_STRING,

    VT_NONE
};

struct Value
{
    Value()
        : type(ValueType::VT_NONE)
        , numeric(0.f)
        , string("")
    {}

    Value(float numeric)
        : type(ValueType::VT_NUMERIC)
        , numeric(numeric)
        , string("")
    {}

    Value(std::string string)
        : type(ValueType::VT_STRING)
        , numeric(0.0)
        , string(string)
    {}

    ValueType type;

    float numeric;
    std::string string;
};

enum class NodeType
{
    NT_IDENTIFIER,
    NT_CONST_VALUE,
    NT_EXPRESSION,

    NT_NONE
};

class Node
{
    public:
        Node(NodeType type, Operator op, Value value)
            : m_type(type)
            , m_identifier("")
            , m_value(value)
            , m_op(op)
        {}

        Node(const std::string& identifier_or_string, NodeType type)
            : m_type(type)
            , m_op(Operator::OP_NONE)
        {
            if(type == NodeType::NT_CONST_VALUE)
                m_value = Value(identifier_or_string);
            else
                m_identifier = identifier_or_string;
        }

        Node(float value)
            : m_type(NodeType::NT_CONST_VALUE)
            , m_identifier("")
            , m_value(value)
            , m_op(Operator::OP_NONE)
        {}

        Node(Operator op)
            : m_type(NodeType::NT_EXPRESSION)
            , m_identifier("")
            , m_value(0.f)
            , m_op(op)
        {}

        ~Node()
        {
            for(Node* child : m_children)
                delete child;
        }

        void addChild(Node* child)
        {
            m_children.push_back(child);
        }

        std::vector<Node*> getChildren() const
        {
            return m_children;
        }

        NodeType getType() const
        {
            return m_type;
        }

        std::string getIdentifier() const
        {
            return m_identifier;
        }

        Value getValue() const
        {
            return m_value;
        }

        Operator getOperator() const
        {
            return m_op;
        }

    protected:
        NodeType m_type;
        std::string m_identifier;
        Value m_value;
        Operator m_op;

        std::vector<Node*> m_children;
};

#endif // DATATYPES_HPP_INCLUDED
