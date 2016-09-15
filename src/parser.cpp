#include "parser.hpp"
#include "errors.hpp"

Parser::Parser(Lexer& lexer)
    : m_lexer(lexer)
{
}

Node* Parser::parse()
{
    return getExpression();
}

Token Parser::match(TokenType type)
{
    Token token = m_lexer.getLook();

    if(m_lexer.getLook().type == type)
        m_lexer.getNext();
    else
        errors::parserError("expected " + string_utils::from(type));

    return token;
}

Token Parser::match(const std::string& value)
{
    Token token = m_lexer.getLook();

    if(m_lexer.getLook().value == value)
        m_lexer.getNext();
    else
        errors::parserError("expected " + value);

    return token;
}

std::string Parser::getIdentifier()
{
    Token identifier = match(TokenType::TT_IDENTIFIER);
    return identifier.value;
}

std::string Parser::getString()
{
    Token string = match(TokenType::TT_STRING);
    return string.value;
}

float Parser::getNumeric()
{
    Token numeric = match(TokenType::TT_NUMERIC);
    return string_utils::to<float>(numeric.value);
}

Operator Parser::getOperator()
{
    Token op = match(TokenType::TT_OPERATOR);

    if(operatorsTable.find(op.value) != operatorsTable.end())
       return operatorsTable.at(op.value);
    else
        return Operator::OP_NONE;
}

std::string Parser::getNull()
{
    Token null_token = match(TokenType::TT_NULL);
    return null_token.value;
}

Node* Parser::getExpression()
{
    if(m_lexer.getLook().type == TokenType::TT_LEFT_PAR)
    {
        match(TokenType::TT_LEFT_PAR);

        if(m_lexer.getLook().type == TokenType::TT_NUMERIC)
        {
            Node* tempNode = new Node(getNumeric());
            match(TokenType::TT_RIGHT_PAR);

            return tempNode;
        }
        else if(m_lexer.getLook().type == TokenType::TT_STRING)
        {
            Node* tempNode = new Node(getString(), NodeType::NT_CONST_VALUE);
            match(TokenType::TT_RIGHT_PAR);

            return tempNode;
        }
        else if(m_lexer.getLook().type == TokenType::TT_IDENTIFIER)
        {
            Node* tempNode = new Node(getIdentifier(), NodeType::NT_IDENTIFIER);
            match(TokenType::TT_RIGHT_PAR);

            return tempNode;
        }
        else if(m_lexer.getLook().type == TokenType::TT_NULL)
        {
            Node* tempNode = new Node(NodeType::NT_CONST_VALUE, Operator::OP_NONE, Value());
            match(TokenType::TT_RIGHT_PAR);

            return tempNode;
        }
        else
        {
            Operator op = getOperator();
            std::vector<Node*> children = getExpressionList();

            Node* expression = new Node(op);

            for(Node* child : children)
                expression->addChild(child);

            match(TokenType::TT_RIGHT_PAR);

            return expression;
        }
    }
    else if(m_lexer.getLook().type == TokenType::TT_NUMERIC)
    {
        return new Node(getNumeric());
    }
    else if(m_lexer.getLook().type == TokenType::TT_STRING)
    {
        return new Node(getString(), NodeType::NT_CONST_VALUE);
    }
    else if(m_lexer.getLook().type == TokenType::TT_IDENTIFIER)
    {
        return new Node(getIdentifier(), NodeType::NT_IDENTIFIER);
    }
    else if(m_lexer.getLook().type == TokenType::TT_NULL)
    {
        // For a call to match.
        getNull();
        return new Node(NodeType::NT_CONST_VALUE, Operator::OP_NONE, Value());
    }
    else
    {
        errors::parserError("unexpected token \"" + m_lexer.getLook().value + "\"");

        // Totally useless, since errors::parserError() calls abort() which calls std::exit().
        // But the compiler keep throwing a warning for non-returning in a non-void function.
        return nullptr;
    }
}

std::vector<Node*> Parser::getExpressionList()
{
    std::vector<Node*> nodes;

    while(m_lexer.getLook().type != TokenType::TT_RIGHT_PAR && !m_lexer.isEmpty())
        nodes.push_back(getExpression());

    return nodes;
}
