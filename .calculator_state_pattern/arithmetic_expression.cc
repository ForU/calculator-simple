#include "arithmetic_expression.hh"
#include "arithmetic_expression_state.hh"
#include "log.hh"

#include <math.h>

const int ArithmeticExpression::FLG_POSITIVE = 1;
const int ArithmeticExpression::FLG_NEGATIVE = -1;
const char ArithmeticExpression::LOWEST_PRIO_OP = '#';

ArithmeticExpression::ArithmeticExpression(const std::string& infix_expression)
{
    m_arithmetic_expression = infix_expression;
    m_parse_pos = 0;
    m_flg = FLG_POSITIVE;
    m_lp_count = 0;
    m_operator_stack.push(LOWEST_PRIO_OP);
    m_state = AEStateStart::instance();
}

const std::string& ArithmeticExpression::getCurStateStr() const
{
    return m_state->name();
}

int ArithmeticExpression::getPriority(char c) const
{
    /* priority order for high to low:
     * 4: '('
     * 3: '*' '/'
     * 2: '+' '-'
     * 1: ')'
     * 0: LOWEST_PRIO_OP
     */
    assert( c == '(' || c == ')' ||
            c == '^' ||
            c == '*' || c == '/' ||
            c == '+' || c == '-' ||
            c == LOWEST_PRIO_OP );
    int prio = 0;
    switch (c) {
    case '(': prio = 5; break;
    case '^': prio = 4; break;
    case '*':
    case '/': prio = 3; break;
    case '+':
    case '-': prio = 2; break;
    case ')': prio = 1; break;
    case LOWEST_PRIO_OP: prio = 0; break;
    default: prio = 0; break;
    }
    return prio;
}

CharType ArithmeticExpression::getCharType(int c) const
{
    if ( ' ' == c || '\t' == c )              return CT_WHT;
    if ( (c <= '9' && c >= '0') || '.' == c ) return CT_OPD;
    if ( '(' == c )                           return CT_LPS;
    if ( ')' == c )                           return CT_RPS;
    if ( '*' == c || '/' == c || '^' == c)    return CT_OPR;
    // process CT_OP (+,-) and CT_FLG separately, make
    // sure return right char type
    if ( '+' == c || '-' == c) {
        if ( AEStateStart::instance() == m_state ||
             AEStateOperator::instance() == m_state ||
             AEStateFlag::instance() == m_state ||
             AEStateLeftParenthesis::instance() == m_state ) {
            return CT_FLG;
        }
        if ( AEStateOperand::instance() == m_state ||
             AEStateRightParenthesis::instance() == m_state) {
            return CT_OPR;
        }
    }
    return CT_UPPER;
}

bool ArithmeticExpression::handleError() { return false; }
bool ArithmeticExpression::handleNone() { ++m_parse_pos; return true; }
bool ArithmeticExpression::handleFlag() { return m_state->handleFlag(this); }
bool ArithmeticExpression::handleOperator() { return m_state->handleOperator(this); }
bool ArithmeticExpression::handleOperand() { return m_state->handleOperand(this); }
bool ArithmeticExpression::handleLeftParenthesis() { return m_state->handleLeftParenthesis(this); }
bool ArithmeticExpression::handleRightParenthesis() { return m_state->handleRightParenthesis(this); }

bool ArithmeticExpression::handle(char c)
{
    PR_TRACE("current state=\"%s\", coming char = '%c', remaining=\"%s\"",
             getCurStateStr().c_str(), c, &m_arithmetic_expression[m_parse_pos+1]);

    CharType type = getCharType(c);
    if ( CT_UPPER == type) {
        PR_ERROR("invalid char type, argument char='%c'",
                 m_arithmetic_expression[m_parse_pos]);
        return false;
    }
    bool rc = false;
    switch (type) {
    case CT_WHT: rc = handleNone(); break;
    case CT_FLG: rc = handleFlag(); break;
    case CT_OPR: rc = handleOperator();break;
    case CT_OPD: rc = handleOperand(); break;
    case CT_LPS: rc = handleLeftParenthesis(); break;
    case CT_RPS: rc = handleRightParenthesis(); break;
    case CT_ERR: rc = handleError(); break;
    default: rc = false;
    }
    return rc;
}

bool ArithmeticExpression::parse()
{
    int size = m_arithmetic_expression.size();
    if ( size == 0 ) {
        PR_DEBUG("empty arithmetic expression");
        return true;
    }
    while ( m_parse_pos < size ) {
        const char& c = m_arithmetic_expression.at(m_parse_pos);
        if ( ! handle(c) ) {
            PR_ERROR("failed to handle, "
                     "tailing=\"%s\", pos=%d, "
                     "char='%c', raw exp=\"%s\", "
                     "state=\"%s\"",
                     &m_arithmetic_expression[m_parse_pos],
                     m_parse_pos,
                     m_arithmetic_expression[m_parse_pos],
                     m_arithmetic_expression.c_str(),
                     getCurStateStr().c_str()
                );
            return false;
        }
    }
    // check program terminal state
    if ( ! isOnTerminalState() ) {
        PR_ERROR("program in not on terminal state, "
                 "current state=\"%s\"",
                 getCurStateStr().c_str());
        return false;
    }
    // check whether parenthesis matches
    if ( 0 != m_lp_count ) {
        PR_ERROR("open left parenthesis exists, count=\"%d\"",
                 m_lp_count);
        return false;
    }
    // fulfill m_rpn_expression using operator stack
    while ( ! m_operator_stack.empty() ) {
        const char& top = m_operator_stack.top();
        if ( LOWEST_PRIO_OP != top ) {
            m_rpn_expression.push_back(top);
            m_rpn_expression.push_back(' ');
        }
        if (! calculate(top)) return false;
        // always pop
        m_operator_stack.pop();
    }

    return true;
}

bool ArithmeticExpression::isOnTerminalState() const
{
    return (AEStateOperand::instance() == m_state ||
            AEStateRightParenthesis::instance() == m_state ||
            AEStateStart::instance() == m_state);
}

bool ArithmeticExpression::calculate(char opr)
{
    if ( '(' == opr || ')' == opr || LOWEST_PRIO_OP == opr ) {
        PR_DEBUG("do nothing for '%c'", opr);
        return true;
    }

    double r_opd = m_operand_stack.pop();
    double l_opd = m_operand_stack.pop();
    switch (opr) {
    case '^': m_operand_stack.push( pow(l_opd,r_opd) ); break;
    case '+': m_operand_stack.push( l_opd + r_opd ); break;
    case '-': m_operand_stack.push( l_opd - r_opd ); break;
    case '*': m_operand_stack.push( l_opd * r_opd ); break;
    case '/': {
        if ( 0 == r_opd ) {
            PR_FATAL("divide 0");
            return false;
        }
        m_operand_stack.push( l_opd / r_opd);
        break;
    }
    default: PR_ERROR("unknown operator='%c'", opr); return false;
    }

    PR_TRACE("opr='%c', l_opd=\"%g\", r_opd=\"%g\", rst=\"%g\"",
             opr, l_opd, r_opd, m_operand_stack.top());
    return true;
}

bool ArithmeticExpression::getExpressionValue(double &val) const
{
    if ( !isOnTerminalState() ) {
        PR_ERROR("program in not on terminal state, "
                 "current state=\"%s\"",
                 getCurStateStr().c_str());
        return false;
    }
    bool stack_empty = m_operand_stack.empty();
    if ( ! stack_empty || (stack_empty && AEStateStart::instance() == m_state)) {
        val = stack_empty ? 0 : m_operand_stack.top();
        PR_TRACE("return value=\"%g\"", val);
        return true;
    }
    PR_ERROR("the arithmetic expression is not successfully calculated");
    return false;
}
