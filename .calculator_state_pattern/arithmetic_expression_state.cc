#include "arithmetic_expression_state.hh"
#include "arithmetic_expression.hh"
#include "stack.hh"
#include "log.hh"

#include <string.h>             // memset
#include <math.h>

bool AEState::handleFlagImp(ArithmeticExpression *ae)
{
    int cur_pos = ae->getParsePosition();
    const char& f_c= ae->getArithmeticExpression().at(cur_pos);
    ('-' == f_c) ? ae->setFlagNegative() : ae->setFlagPositive();
    ae->setParsePosition(cur_pos+1);
    ae->updateState(AEStateFlag::instance());
    return true;
}

bool AEState::handleOperatorImp(ArithmeticExpression *ae)
{
    OperatorStack& opr_stack = ae->getOperatorStack();
    int cur_pos = ae->getParsePosition();
    const char& cur_opr = ae->getArithmeticExpression().at(cur_pos);
    char top = opr_stack.top();
    while ( ae->getPriority(top) >= ae->getPriority(cur_opr)
            && '(' != top ) {
        // update rpn
        std::string& rpn_exp = ae->getRpnExpression();
        rpn_exp.push_back(top);rpn_exp.push_back(' ');
        // calculate
        if (! ae->calculate(top)) return false;
        // update new top
        opr_stack.pop();
        top = opr_stack.top();
    }
    PR_TRACE("pushing operator='%c', rpn=\"%s\"",
             cur_opr, ae->getRpnExpression().c_str());
    opr_stack.push(cur_opr);

    ae->setParsePosition(cur_pos+1);
    ae->updateState(AEStateOperator::instance());
    return true;
}

bool AEState::handleOperandImp(ArithmeticExpression *ae)
{
    double int_val = 0;
    double dot_val = 0;
    double rst_val = 0;

    int cur_pos = ae->getParsePosition();
    const std::string& ae_str = ae->getArithmeticExpression();
    // sucks for fetching size each time for operand
    int size = ae_str.size();
    bool dot_occurred = false;
    int dot_part_length = 0;

    while ( cur_pos < size ) {
        char c = ae_str[cur_pos];
        if ( ! ('.' == c || (c >= '0' && c <= '9'))) {
            break;
        }
        if ( '.' == c && dot_occurred) {
            PR_ERROR("dot occurs again");
            return false;
        }
        // now, three combination:
        // 1: '.' == c && ! dot_occurred
        if ( '.' == c ) {
            dot_occurred = true;
            c = ae_str[++cur_pos];
            continue;
        }
        // 2: '.' != c || dot_occurred
        if ( dot_occurred ) {
            ++dot_part_length;
            dot_val = dot_val * 10 + c - '0';
        }
        // 3: '.' != c || ! dot_occurred
        else {
            int_val = int_val * 10 + c - '0';
        }
        c = ae_str[++cur_pos];
    }
    if ( dot_occurred ) {
        dot_val *= pow(0.1, dot_part_length);
    }
    rst_val = ae->getFlag() * (int_val + dot_val);
    ae->resetFlag();

    // push operand into stack
    OperandStack& opd_stack = ae->getOperandStack();
    opd_stack.push(rst_val);

    // update m_rpn_expression
    static char val_str[20];
    memset(val_str, sizeof val_str, 0);
    snprintf(val_str, sizeof val_str, "%g ", rst_val);
    std::string& rpn_exp = ae->getRpnExpression();
    rpn_exp.append(val_str);

    ae->setParsePosition(cur_pos);
    ae->updateState(AEStateOperand::instance());

    PR_TRACE("rst part=\"%g\", int part=\"%g\", dot part=\"%g\", dot occurred=\"%s\"",
             rst_val, int_val, dot_val, dot_occurred ? "yes" : "no");
    return true;
}
bool AEState::handleLeftParenthesisImp(ArithmeticExpression *ae)
{
    int cur_pos = ae->getParsePosition();
    const char& cur_opr = ae->getArithmeticExpression().at(cur_pos);
    OperatorStack& opr_stack = ae->getOperatorStack();
    opr_stack.push(cur_opr);

    ae->updateLeftParenthesisCount(1);
    ae->setParsePosition(cur_pos+1);
    ae->updateState(AEStateLeftParenthesis::instance());
    return true;
}
bool AEState::handleRightParenthesisImp(ArithmeticExpression *ae)
{
    int cur_pos = ae->getParsePosition();
    ae->updateLeftParenthesisCount(-1);
    int lp_count = ae->getLeftParenthesisCount();
    if ( lp_count < 0 ) {
        PR_ERROR("exists extra right parenthesis");
        return false;
    }

    // pop till '('
    char top;
    OperatorStack& opr_stack = ae->getOperatorStack();
    std::string& rpn_exp = ae->getRpnExpression();
    while ( ! opr_stack.empty()
            && '(' != (top = opr_stack.top())) {
        if ( ae->getLowestPrioOp() != top ) {
            rpn_exp.push_back(top);
            rpn_exp.push_back(' ');
        }
        if (! ae->calculate(top)) return false;
        // always pop
        opr_stack.pop();
    }
    // just pop '(', do not append to RPN
    opr_stack.pop();
    // DO NOT save this operator:')' into stack
    ae->updateState(AEStateRightParenthesis::instance());
    ae->setParsePosition(cur_pos+1);
    return true;
}
