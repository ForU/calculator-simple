#ifndef INCLUDE_ARITHMETIC_EXPRESSION_HPP
#define INCLUDE_ARITHMETIC_EXPRESSION_HPP

#include "stack.hh"
#include <string>

enum CharType
{
    CT_WHT = 0,
    CT_OPR,
    CT_OPD,
    CT_LPS,
    CT_RPS,
    CT_FLG,
    CT_ERR,
    CT_UPPER                    // upper bound
};


typedef Stack<double> OperandStack;
typedef Stack<char> OperatorStack;

class AEState;
class ArithmeticExpression
{
public:
    ArithmeticExpression(const std::string& infix_expression="");
    ~ArithmeticExpression() {}

    bool parse();
    bool calculate(char opr);

    bool getExpressionValue(double &val) const;
    const std::string& getRpnExpression() const {
        return m_rpn_expression;
    }
    std::string& getRpnExpression() {
        return m_rpn_expression;
    }
    const std::string& getArithmeticExpression() const {
        return m_arithmetic_expression;
    }

    int getPriority(char c) const;
    CharType getCharType(int c) const;
    int  getParsePosition() const {
        return m_parse_pos;
    }
    void setParsePosition(int pos) {
        m_parse_pos = pos;
    }
    int getFlag() const {
        return m_flg;
    }
    void resetFlag() {
        m_flg = FLG_POSITIVE;
    }
    void setFlagPositive() {
        m_flg = FLG_POSITIVE;
    }
    void setFlagNegative() {
        m_flg = FLG_NEGATIVE;
    }
    OperandStack& getOperandStack() {
        return m_operand_stack;
    }
    const OperandStack& getOperandStack() const {
        return m_operand_stack;
    }
    OperatorStack& getOperatorStack() {
        return m_operator_stack;
    }
    const OperandStack& getOperatorStack() const {
        return m_operand_stack;
    }
    char getLowestPrioOp() const {
        return LOWEST_PRIO_OP;
    }
    int getLeftParenthesisCount() const {
        return m_lp_count;
    }
    void updateLeftParenthesisCount(int collapse) {
        m_lp_count += collapse;
    }
    const std::string& getCurStateStr() const;
    void updateState(AEState* new_state) {
        m_state = new_state;
    }
    bool isOnTerminalState() const;

private:
    bool handle(char c);

    bool handleError();
    bool handleNone();
    bool handleFlag();
    bool handleOperator();
    bool handleOperand();
    bool handleLeftParenthesis();
    bool handleRightParenthesis();


private:
    std::string m_arithmetic_expression;
    std::string m_rpn_expression;
    int m_parse_pos;
    int m_flg;
    int m_lp_count;

    OperandStack m_operand_stack;
    OperatorStack m_operator_stack;

    AEState *m_state;

    static const int FLG_POSITIVE;
    static const int FLG_NEGATIVE;
    static const char LOWEST_PRIO_OP;
};

#endif /* INCLUDE_ARITHMETIC_EXPRESSION_HPP */
