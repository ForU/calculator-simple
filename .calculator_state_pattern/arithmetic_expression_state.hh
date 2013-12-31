#ifndef INCLUDE_ARITHMETIC_EXPRESSION_STATE_HPP
#define INCLUDE_ARITHMETIC_EXPRESSION_STATE_HPP

#include <string>

class ArithmeticExpression;
class AEState
{
public:
    AEState(const std::string& name_val="") : m_name(name_val) {}
    ~AEState() {}

    virtual bool handleFlag(ArithmeticExpression *ae) { return false; }
    virtual bool handleOperator(ArithmeticExpression *ae) { return false; }
    virtual bool handleOperand(ArithmeticExpression *ae) { return false; }
    virtual bool handleLeftParenthesis(ArithmeticExpression *ae) { return false; }
    virtual bool handleRightParenthesis(ArithmeticExpression *ae) { return false; }

    const std::string& name() const { return m_name; }

protected:
    bool handleFlagImp(ArithmeticExpression *ae);
    bool handleOperatorImp(ArithmeticExpression *ae);
    bool handleOperandImp(ArithmeticExpression *ae);
    bool handleLeftParenthesisImp(ArithmeticExpression *ae);
    bool handleRightParenthesisImp(ArithmeticExpression *ae);

protected:
    std::string m_name;
};

class AEStateStart: public AEState
{
public:
    AEStateStart() : AEState("start") {}
    static AEState* instance() {
        // not thread-safe version
        static AEStateStart s_st_instance;
        return &s_st_instance;
    }

    virtual bool handleFlag(ArithmeticExpression *ae) { return handleFlagImp(ae); }
    virtual bool handleOperand(ArithmeticExpression *ae) { return handleOperandImp(ae); }
    virtual bool handleLeftParenthesis(ArithmeticExpression *ae) { return handleLeftParenthesisImp(ae); }
};

class AEStateFlag : public AEState
{
public:
    AEStateFlag() : AEState("flag") {}
    static AEState* instance() {
        // not thread-safe version
        static AEStateFlag s_st_instance;
        return &s_st_instance;
    }
    virtual bool handleOperand(ArithmeticExpression *ae) { return handleOperandImp(ae); }
};

class AEStateOperator : public AEState
{
public:
    AEStateOperator() : AEState("operator") {}
    static AEState* instance() {
        // not thread-safe version
        static AEStateOperator s_st_instance;
        return &s_st_instance;
    }
    virtual bool handleFlag(ArithmeticExpression *ae) { return handleFlagImp(ae); }
    virtual bool handleOperand(ArithmeticExpression *ae) { return handleOperandImp(ae); }
    virtual bool handleLeftParenthesis(ArithmeticExpression *ae) { return handleLeftParenthesisImp(ae); }
};

class AEStateOperand : public AEState
{
public:
    AEStateOperand() : AEState("operand") {}
    static AEState* instance() {
        // not thread-safe version
        static AEStateOperand s_st_instance;
        return &s_st_instance;
    }
    virtual bool handleOperator(ArithmeticExpression *ae) { return handleOperatorImp(ae); }
    virtual bool handleRightParenthesis(ArithmeticExpression *ae) { return handleRightParenthesisImp(ae); }
};

class AEStateLeftParenthesis : public AEState
{
public:
    AEStateLeftParenthesis() : AEState("left parenthesis") {}
    static AEState* instance() {
        // not thread-safe version
        static AEStateLeftParenthesis s_st_instance;
        return &s_st_instance;
    }
    virtual bool handleFlag(ArithmeticExpression *ae) { return handleFlagImp(ae); }
    virtual bool handleOperand(ArithmeticExpression *ae) { return handleOperandImp(ae); }
    virtual bool handleLeftParenthesis(ArithmeticExpression *ae) { return handleLeftParenthesisImp(ae); }
};

class AEStateRightParenthesis : public AEState
{
public:
    AEStateRightParenthesis() : AEState("right parenthesis") {}
    static AEState* instance() {
        // not thread-safe version
        static AEStateRightParenthesis s_st_instance;
        return &s_st_instance;
    }

    virtual bool handleOperator(ArithmeticExpression *ae) { return handleOperatorImp(ae); }
    virtual bool handleRightParenthesis(ArithmeticExpression *ae) { return handleRightParenthesisImp(ae); }
};

#endif /* INCLUDE_ARITHMETIC_EXPRESSION_STATE_HPP */
