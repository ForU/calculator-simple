// a state machine to parse infix math Expression
#ifndef INCLUDE_CALCULATOR_HPP
#define INCLUDE_CALCULATOR_HPP

#include <string>
#include "stack.h"

enum State
{
    ST_NON = 0,
    ST_OPR,                     // operator
    ST_OPD,                     // operand
    ST_LPS,
    ST_RPS,
    ST_ERR,
    ST_UPPER                    // upper bound
};

enum CharType
{
    CT_WHITESPACE = 0,
    CT_OP,
    CT_NUM,
    CT_LP,
    CT_RP,
    CT_UPPER                    // upper bound
};

enum ActionType
{
    ACT_NON = 0,
    ACT_OPR,                    // operator
    ACT_OPD,                    // operand
    ACT_LPS,
    ACT_RPS,
    ACT_ERR,
    ACT_UPPER                   // upper bound
};

class ArithmeticExpression
{
public:
    ArithmeticExpression(const std::string& infix_expression);
    ~ArithmeticExpression() {}

    bool parse();
    const std::string& getRpnExpression() const {
        return m_rpn_expression;
    }
    const std::string& getInfixExpression() const {
        return m_infix_expression;
    }

    bool handle(CharType type);
    CharType getCharType(int c);
    bool handleAction(ActionType type);

private:
    bool isOnTerminalState() {
        return (ST_OPD == m_state || ST_RPS == m_state);
    }
    bool handleNone();
    bool handleError();
    bool handleOperator();
    bool handleOperand();
    bool handleLeftParenthesis();
    bool handleRightParenthesis();

    int getPriority(char c);

private:
    Stack<double> m_operand_stack;
    Stack<char> m_operator_stack;

    State m_state;
    std::string m_infix_expression;
    int m_parse_pos;
    int m_lp_count;
    std::string m_rpn_expression;

    struct StateTable{
        State new_state;
        ActionType action_type;
    };
    static StateTable m_state_table[ST_UPPER][CT_UPPER];
};

#endif /* INCLUDE_CALCULATOR_HPP */

