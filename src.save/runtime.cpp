#include "Runtime.hpp"

Runtime::Runtime()
{}

void Runtime::clear()
{
    m_variables.clear();
}

Value Runtime::eval(Node* node)
{
    if(node->getOperator() == Operator::OP_NONE)
    {
        if(node->getType() == NodeType::NT_IDENTIFIER)
            return getVariable(node->getIdentifier());
        else if(node->getType() == NodeType::NT_CONST_VALUE)
            return node->getValue();
    }

    if(node->getChildren().size() == 0)
        errors::runtimeError("expression with operator but no parameters");

    switch(node->getOperator())
    {
        /** Specials built-in operations. */
        case Operator::OP_PROGRAM:
            return program(node->getChildren());
            break;
        case Operator::OP_ASSIGN:
            return assign(node->getChildren());
            break;
        case Operator::OP_TO_NUMERIC:
            return to_numeric(node->getChildren());
            break;
        case Operator::OP_TO_STRING:
            return to_string(node->getChildren());
            break;
        case Operator::OP_PRINT:
            return print(node->getChildren());
            break;
        case Operator::OP_INPUT:
            return input(node->getChildren());
            break;

        /** Maths built-in operations. */
        case Operator::OP_ADD:
            return add(node->getChildren());
            break;
        case Operator::OP_SUB:
            return sub(node->getChildren());
            break;
        case Operator::OP_MUL:
            return mul(node->getChildren());
            break;
        case Operator::OP_DIV:
            return div(node->getChildren());
            break;
        case Operator::OP_MOD:
            return mod(node->getChildren());
            break;
        case Operator::OP_POW:
            return pow(node->getChildren());
            break;
        case Operator::OP_SIN:
            return sin(node->getChildren());
            break;
        case Operator::OP_COS:
            return cos(node->getChildren());
            break;
        case Operator::OP_TAN:
            return tan(node->getChildren());
            break;
        case Operator::OP_ASIN:
            return asin(node->getChildren());
            break;
        case Operator::OP_ACOS:
            return acos(node->getChildren());
            break;
        case Operator::OP_ATAN:
            return atan(node->getChildren());
            break;
        case Operator::OP_TO_RAD:
            return to_rad(node->getChildren());
            break;
        case Operator::OP_TO_DEG:
            return to_deg(node->getChildren());
            break;
        case Operator::OP_LN:
            break;
            return ln(node->getChildren());
        case Operator::OP_EXP:
            return exp(node->getChildren());
            break;
        case Operator::OP_LOG10:
            return log10(node->getChildren());
            break;
        default:
            break;
    }

    return 0.f;
}

Value Runtime::getVariable(const std::string& identifier)
{
    if(m_variables.find(identifier) != m_variables.end())
        return m_variables.at(identifier);
    else
        errors::runtimeError("unassigned identifier " + identifier);

    // Useless but prevent compiler's warnings.
    return 0.f;
}

/** Special built-in operations. */
Value Runtime::program(std::vector<Node*> nodes)
{
    for(Node* child : nodes)
        this->eval(child);

    return Value();
}

Value Runtime::assign(std::vector<Node*> nodes)
{
    if(nodes.size() != 2)
        errors::runtimeError("assign operator takes exactly two operators");

    if(nodes.front()->getType() != NodeType::NT_IDENTIFIER)
        errors::runtimeError("first parameter of assign operator must be an identifier");

    if(nodes.back()->getType() != NodeType::NT_CONST_VALUE && nodes.back()->getType() != NodeType::NT_EXPRESSION)
        errors::runtimeError("second parameter of assign operator must be a constant value or an expression");

    // Get the variable name & eval the value.
    // If the variable already exist, it is overwritten.
    // Otherwise it create the new variable.
    m_variables[nodes.front()->getIdentifier()] = this->eval(nodes.back());

    // Return value type = VT_NONE.
    return Value();
}

Value Runtime::to_numeric(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("to_numeric operator takes only one operators");

    Value conversion_result = eval(nodes.front());

    // Convert type.
    conversion_result.type = ValueType::VT_NUMERIC;

    // Convert str -> num.
    if(conversion_result.type == ValueType::VT_STRING)
        conversion_result.numeric = string_utils::to<float>(conversion_result.string);

    return conversion_result;
}

Value Runtime::to_string(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("to_string operator takes only one operator");

    Value conversion_result = eval(nodes.front());

    // Convert type.
    conversion_result.type = ValueType::VT_STRING;

    // Convert num -> str.
    if(conversion_result.type == ValueType::VT_NUMERIC)
        conversion_result.string = string_utils::from<float>(conversion_result.numeric);

    return conversion_result;
}

Value Runtime::print(std::vector<Node*> nodes)
{
    for(Node* child : nodes)
    {
        Value value = this->eval(child);

        if(value.type == ValueType::VT_NUMERIC)
            std::cout << value.numeric;
        else if(value.type == ValueType::VT_STRING)
            std::cout << value.string;
    }

    // Return value type = VT_NONE.
    return Value();
}

Value Runtime::input(std::vector<Node*> nodes)
{
    // Output prompt.
    print(nodes);

    std::string word("");
    std::cin >> word;

    Value inputvalue(word);
    return inputvalue;
}

/** Maths built-in operations. */
Value Runtime::add(std::vector<Node*> nodes)
{
    Value result = this->eval(nodes.front());

    // The ValueType for the addition.
    ValueType currentValueType = result.type;

    // Cannot add non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply add operator on non-typed values");

    if(nodes.size() > 1)
    {
        for(std::vector<Node*>::iterator it(nodes.begin() + 1) ; it != nodes.end() ; ++it)
        {
            Value childValue = this->eval(*it);

            // Check if no incompatible types.
            if(childValue.type != currentValueType)
                errors::runtimeError("cannot apply add operator on different types");

            // Then add.
            if(currentValueType == ValueType::VT_NUMERIC)
                result.numeric += childValue.numeric;
            else if(currentValueType == ValueType::VT_STRING)
                result.string += childValue.string;
        }
    }

    return result;
}

Value Runtime::sub(std::vector<Node*> nodes)
{
    Value result = this->eval(nodes.front());

    // The ValueType for the substraction.
    ValueType currentValueType = result.type;

    // Cannot sub non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply sub operator on non-typed values");

    // Cannot sub strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply sub on strings");

    if(nodes.size() > 1)
    {
        for(std::vector<Node*>::iterator it(nodes.begin() + 1) ; it != nodes.end() ; ++it)
        {
            Value childValue = this->eval(*it);

            // Check if no incompatible types.
            if(childValue.type != currentValueType)
                errors::runtimeError("cannot apply sub operator on different types");

            // Then sub.
            result.numeric -= childValue.numeric;
        }
    }

    return result;
}

Value Runtime::mul(std::vector<Node*> nodes)
{
    if(nodes.size() < 1)
        errors::runtimeError("mul operator needs at least two arguments");

    Value result = this->eval(nodes.front());

    // The ValueType for the multiplication.
    ValueType currentValueType = result.type;

    // Cannot mul non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply mul operator on non-typed values");

    // Cannot mul strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply mul operator on strings");

    if(nodes.size() > 1)
    {
        for(std::vector<Node*>::iterator it(nodes.begin() + 1) ; it != nodes.end() ; ++it)
        {
            Value childValue = this->eval(*it);

            // Check if no incompatible types.
            if(childValue.type != currentValueType)
                errors::runtimeError("cannot apply mul operator on different types");

            // Then mul.
            result.numeric *= childValue.numeric;
        }
    }

    return result;
}

Value Runtime::div(std::vector<Node*> nodes)
{
    if(nodes.size() < 1)
        errors::runtimeError("div operator needs at least two arguments");

    Value result = this->eval(nodes.front());

    // The ValueType for the division.
    ValueType currentValueType = result.type;

    // Cannot div non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply div operator on non-typed values");

    // Cannot div strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply div operator on strings");

    if(nodes.size() > 1)
    {
        for(std::vector<Node*>::iterator it(nodes.begin() + 1) ; it != nodes.end() ; ++it)
        {
            Value childValue = this->eval(*it);

            // Check if no incompatible types.
            if(childValue.type != currentValueType)
                errors::runtimeError("cannot apply div operator on different types");

            // Then div.
            result.numeric /= childValue.numeric;
        }
    }

    return result;
}

Value Runtime::mod(std::vector<Node*> nodes)
{
    if(nodes.size() < 1)
        errors::runtimeError("mod operator needs at least two arguments");

    Value result = this->eval(nodes.front());

    // The ValueType for the mod.
    ValueType currentValueType = result.type;

    // Cannot mod non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply mod operator on non-typed values");

    // Cannot mod strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply mod operator on strings");

    if(nodes.size() > 1)
    {
        for(std::vector<Node*>::iterator it(nodes.begin() + 1) ; it != nodes.end() ; ++it)
        {
            Value childValue = this->eval(*it);

            // Check if no incompatible types.
            if(childValue.type != currentValueType)
                errors::runtimeError("cannot apply mod operator on different types");

            // Then mod.
            result.numeric = std::fmod(result.numeric, childValue.numeric);
        }
    }

    return result;
}

Value Runtime::pow(std::vector<Node*> nodes)
{
    if(nodes.size() < 1)
        errors::runtimeError("pow operator needs at least two arguments");

    Value result = this->eval(nodes.front());

    // The ValueType for the pow.
    ValueType currentValueType = result.type;

    // Cannot pow non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply pow operator on non-typed values");

    // Cannot pow strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply pow operator on strings");

    if(nodes.size() > 1)
    {
        for(std::vector<Node*>::iterator it(nodes.begin() + 1) ; it != nodes.end() ; ++it)
        {
            Value childValue = this->eval(*it);

            // Check if no incompatible types.
            if(childValue.type != currentValueType)
                errors::runtimeError("cannot apply pow operator on different types");

            // Then pow.
            result.numeric = std::pow(result.numeric, childValue.numeric);
        }
    }

    return result;
}

Value Runtime::sin(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("sin operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply sin operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply sin operator on strings");

    return Value(std::sin(result.numeric));
}

Value Runtime::cos(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("cos operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply cos operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply cos operator on strings");

    return Value(std::cos(result.numeric));
}

Value Runtime::tan(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("tan operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply tan operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply tan operator on strings");

    return Value(std::tan(result.numeric));
}

Value Runtime::asin(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("asin operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply asin operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply asin operator on strings");

    return Value(std::asin(result.numeric));
}

Value Runtime::acos(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("acos operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply acos operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply acos operator on strings");

    return Value(std::acos(result.numeric));
}

Value Runtime::atan(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("atan operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply atan operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply atan operator on strings");

    return Value(std::atan(result.numeric));
}

Value Runtime::to_rad(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("to_rad operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply to_rad operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply to_rad operator on strings");

    return result.numeric * M_PI / 180.0;
}

Value Runtime::to_deg(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("to_deg operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply to_deg operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply to_deg operator on strings");

    return result.numeric * (180.0 / M_PI);
}

Value Runtime::ln(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("ln operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply ln operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply ln operator on strings");

    return std::log(result.numeric);
}

Value Runtime::exp(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("exp operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply exp operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply exp operator on strings");

    return std::exp(result.numeric);
}

Value Runtime::log10(std::vector<Node*> nodes)
{
    if(nodes.size() != 1)
        errors::runtimeError("log10 operator only takes one argument");

    Value result = this->eval(nodes.front());

    // The ValueType.
    ValueType currentValueType = result.type;

    // Cannot apply on non-typed values.
    if(currentValueType == ValueType::VT_NONE)
        errors::runtimeError("cannot apply log10 operator on non-typed values");

    // Cannot apply on strings.
    if(currentValueType == ValueType::VT_STRING)
        errors::runtimeError("cannot apply log10 operator on strings");

    return std::log10(result.numeric);
}
