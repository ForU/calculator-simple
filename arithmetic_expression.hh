// State Machine to parse Infix Arithmetic Expression
#ifndef INCLUDE_CALCULATOR_HPP
#define INCLUDE_CALCULATOR_HPP

#include <string>
#include "stack.hh"

enum State
{
    ST_NON = 0,
    ST_OPR,                     // operator
    ST_OPD,                     // operand
    ST_LPS,
    ST_RPS,
    ST_FLG,
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
    CT_FLG,
    CT_UPPER                    // upper bound
};

enum ActionType
{
    AT_NON = 0,
    AT_OPR,                    // operator
    AT_OPD,                    // operand
    AT_LPS,
    AT_RPS,
    AT_ERR,
    AT_FLG,
    AT_UPPER                   // upper bound
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
    const std::string& getArithmeticExpression() const {
        return m_arithmetic_expression;
    }
    bool getExpressionValue(double &val) const;

private:
    bool handle(char c);
    bool handleNone();
    bool handleFlag();
    bool handleError();
    bool handleOperator();
    bool handleOperand();
    bool handleLeftParenthesis();
    bool handleRightParenthesis();

    bool handleAction(ActionType type);
    bool calculate(char opr);

    bool isOnTerminalState() const {
        return (ST_OPD == m_state ||
                ST_RPS == m_state ||
                ST_NON == m_state);
    }

    int getPriority(char c);
    CharType getCharType(int c);
    const char* getStateStr(State) const;
    const char* getCurStateStr() const;

private:
    Stack<double> m_operand_stack;
    Stack<char> m_operator_stack;

    State m_state;
    std::string m_arithmetic_expression;
    int m_parse_pos;
    int m_lp_count;
    int m_flg;

    std::string m_rpn_expression;

    typedef struct {
        State new_state;
        ActionType action_type;
    } StateTable;
    static StateTable m_state_table[ST_UPPER][CT_UPPER];

    typedef struct {
        State state;
        const char* state_str;
    } StateInfo;
    static StateInfo m_state_info[ST_UPPER+1];
};

#endif /* INCLUDE_CALCULATOR_HPP */

