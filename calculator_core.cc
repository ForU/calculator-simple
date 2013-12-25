#include "calculator_core.hh"
#include "log.hh"

#include <string.h>

#define LOWEST_PRIO_OP '#'

ArithmeticExpression::ArithmeticExpression(const std::string& infix_expression)
    : m_state(ST_NON),
      m_infix_expression(infix_expression),
      m_parse_pos(0),
      m_lp_count(0)
{
    m_operator_stack.push(LOWEST_PRIO_OP);
}

ArithmeticExpression::StateTable ArithmeticExpression::m_state_table[ST_UPPER][CT_UPPER] = {
    /*       {CT_WHITESPACE}, {CT_OP}, {CT_NUM}, {CT_LP}, {CT_RP}, */
    /* NON  */ {{ST_NON, ACT_NON}, {ST_ERR, ACT_ERR}, {ST_OPD, ACT_OPD}, {ST_LPS, ACT_LPS}, {ST_ERR, ACT_ERR}},
    /* OPR  */ {{ST_OPR, ACT_NON }, {ST_ERR, ACT_ERR}, {ST_OPD, ACT_OPD}, {ST_LPS, ACT_LPS}, {ST_ERR, ACT_ERR}},
    /* OPD  */ {{ST_OPD, ACT_NON }, {ST_OPR, ACT_OPR}, {ST_ERR, ACT_ERR}, {ST_ERR, ACT_ERR}, {ST_RPS, ACT_RPS}},
    /* LPS  */ {{ST_LPS, ACT_NON }, {ST_ERR, ACT_ERR}, {ST_OPD, ACT_OPD}, {ST_LPS, ACT_LPS}, {ST_ERR, ACT_ERR}},
    /* RPS  */ {{ST_RPS, ACT_NON }, {ST_OPR, ACT_OPR}, {ST_ERR, ACT_ERR}, {ST_ERR, ACT_ERR}, {ST_ERR, ACT_ERR}}
};

int ArithmeticExpression::getPriority(char c)
{
    /* priority order for high to low:
     * 4: '('
     * 3: '*' '/'
     * 2: '+' '-'
     * 1: ')'
     * 0: LOWEST_PRIO_OP
     */
    assert( c == '(' || c == ')' ||
            c == '*' || c == '/' ||
            c == '+' || c == '-' ||
            c == LOWEST_PRIO_OP );

    int prio = 0;
    switch (c) {
    case '(': prio = 4;
        break;
    case '*':
    case '/': prio = 3;
        break;
    case '+':
    case '-': prio = 2;
        break;
    case ')': prio = 1;
        break;
    case LOWEST_PRIO_OP:
        prio = 0;
        break;
    default:
        prio = 0;
        break;
    }
    return prio;
}

bool ArithmeticExpression::handleAction(ActionType type)
{
    bool rc = false;
    switch (type) {
    case ACT_NON:
        rc = handleNone();
        break;
    case ACT_OPR:
        rc = handleOperator();
        break;
    case ACT_OPD:
        rc = handleOperand();
        break;
    case ACT_LPS:
        rc = handleLeftParenthesis();
        break;
    case ACT_RPS:
        rc = handleRightParenthesis();
        break;
    case ACT_ERR:
        rc = handleError();
        break;
    default:
        rc = false;
    }
    return rc;
}

CharType ArithmeticExpression::getCharType(int c) {
    if ( ' ' == c || '\t' == c ) {
        return CT_WHITESPACE;
    }
    if ( c <= '9' && c >= '0' ) {
        return CT_NUM;
    }
    if ( '+' == c || '-' == c || '*' == c || '/' == c ) {
        return CT_OP;
    }
    if ( '(' == c ) {
        return CT_LP;
    }
    if ( ')' == c ) {
        return CT_RP;
    }
    return CT_UPPER;
}

bool ArithmeticExpression::handle(CharType type)
{
    if ( CT_UPPER == type) {
        PR_ERROR("invalid char type, argument char=%c",
                 m_infix_expression[m_parse_pos]);
        return false;
    }

    // update state
    StateTable* table_item = &m_state_table[m_state][type];
    m_state = table_item->new_state;

    return handleAction(table_item->action_type);
}

bool ArithmeticExpression::handleNone()
{
    return true;
}

bool ArithmeticExpression::handleOperator()
{
    char& cur_opr = m_infix_expression[m_parse_pos];
    char top = m_operator_stack.top();

    while ( getPriority(top) >= getPriority(cur_opr)
            && top != '(' ) {
        m_rpn_expression.push_back(top);
        m_rpn_expression.push_back(' ');
        m_operator_stack.pop();
        // get top again
        top = m_operator_stack.top();
    }
    PR_TRACE("pushing operator:%c, rpn:%s",
             cur_opr, m_rpn_expression.c_str());
    m_operator_stack.push(cur_opr);
    return true;
}
bool ArithmeticExpression::handleError()
{
    PR_ERROR("error occurs");
    return false;               // always false
}

bool ArithmeticExpression::handleOperand()
{
    double val = 0;
    static int size = m_infix_expression.size();
    int c = m_infix_expression[m_parse_pos] - '0';
    while ( c >= 0 && c <= 9 && m_parse_pos < size ) {
        val = val * 10 + c;
        c = m_infix_expression[++m_parse_pos] - '0';
    }
    m_operand_stack.push(val);

    // update m_rpn_expression
    static char val_str[20];
    memset(val_str, sizeof val_str, 0);
    snprintf(val_str, sizeof val_str, "%g ", val);
    m_rpn_expression.append(val_str);
    PR_TRACE("val str = %s", val_str);

    PR_TRACE("digit val=%g", val);
    return true;
}

bool ArithmeticExpression::handleLeftParenthesis()
{
    char& cur_opr = m_infix_expression[m_parse_pos];
    m_operator_stack.push(cur_opr);

    ++m_lp_count;
    return true;
}

bool ArithmeticExpression::handleRightParenthesis()
{
    --m_lp_count;
    if ( m_lp_count < 0 ) {
        PR_ERROR("exists extra right parenthesis");
        return false;
    }

    char top;
    // pop till '('
    while ( ! m_operator_stack.empty()
            && (top = m_operator_stack.top()) != '(' ) {
        if ( LOWEST_PRIO_OP != top ) {
            m_rpn_expression.push_back(top);
            m_rpn_expression.push_back(' ');
        }
        // always pop
        m_operator_stack.pop();
    }
    // just pop '(', do not append to RPN
    m_operator_stack.pop();

    // DO NOT save this operator:')' into stack
    return true;
}

bool ArithmeticExpression::parse()
{
    int size = m_infix_expression.size();

    while ( m_parse_pos < size ) {
        PR_TRACE("handle \"%s\"", &m_infix_expression[m_parse_pos]);

        char c = m_infix_expression.at(m_parse_pos);
        CharType char_type = getCharType(c);
        if ( ! handle(char_type) ) {
            PR_ERROR("failed to handle for char "
                     "raw string=\"%s\", fault pos=%d, "
                     "fault char='%c'",
                     m_infix_expression.c_str(),
                     m_parse_pos,
                     m_infix_expression[m_parse_pos]);
            return false;
        }
        // only update m_parse_pos when char_type is not CT_NUM,
        // Cause the handleOperand() already update m_parse_pos
        if ( CT_NUM != char_type ) {
            ++m_parse_pos;
        }
    }
    // check program terminal state
    if ( ! isOnTerminalState() ) {
        PR_ERROR("program in not on terminal state, cur state=%d",
                 (int)m_state);
        return false;
    }
    // check whether parenthesis matches
    if ( 0 != m_lp_count ) {
        PR_ERROR("open left parenthesis exists, count=%d",
                 m_lp_count);
        return false;
    }
    // fulfill m_rpn_expression using operator stack
    while ( ! m_operator_stack.empty() ) {
        char& top = m_operator_stack.top();
        if ( LOWEST_PRIO_OP != top ) {
            m_rpn_expression.push_back(top);
            m_rpn_expression.push_back(' ');
        }
        m_operator_stack.pop();
    }

    PR_TRACE("successfully parsed");
    return true;
}

