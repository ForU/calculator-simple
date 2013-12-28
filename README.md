Simple Arithmetic Expression Calculator
==================

A simple calculator Using FSM-based programming

> **NOTE:**  This is just a scratch, so do not expect too much

```
st_ops stands for state operator, can be '+', '-', '*', '/', '^'
st_opd stands for state operand, can be 0-9 and '.'
st_lps stands for left parenthesis: '('
st_rps stands for right parentesis: ')'
st_flg stands for flags: '+' or '-'
```
![](https://raw.github.com/ForU/calculator-simple/master/state_transition.png "state transition procedure")


## example
----------
```bash
$ ./calculator "(-5 + (-00.01 - 0.09) * 10 ^ 2) / 5"
```
```bash
TRACE [main] ________________________________________________________________________
"(-5 + (-00.01 - 0.09) * 10 ^ 2) / 5 " - calculator_app.cc:14
TRACE [parse] to handle "(-5 + (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "start", coming char = '(', remaining="(-5 + (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "-5 + (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "left parenthesis", coming char = '-', remaining="-5 + (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleFlag] handing flags ... - arithmetic_expression.cc:173
TRACE [parse] to handle "5 + (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "flag", coming char = '5', remaining="5 + (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperand] rst part=-5, int part=5, dot part=0, dot occurred = no - arithmetic_expression.cc:261
TRACE [parse] to handle " + (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = ' ', remaining=" + (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "+ (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = '+', remaining="+ (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperator] pushing operator:+, rpn:-5  - arithmetic_expression.cc:198
TRACE [parse] to handle " (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = ' ', remaining=" (-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "(-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = '(', remaining="(-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "left parenthesis", coming char = '-', remaining="-00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleFlag] handing flags ... - arithmetic_expression.cc:173
TRACE [parse] to handle "00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "flag", coming char = '0', remaining="00.01 - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperand] rst part=-0.01, int part=0, dot part=0.01, dot occurred = yes - arithmetic_expression.cc:261
TRACE [parse] to handle " - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = ' ', remaining=" - 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "- 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = '-', remaining="- 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperator] pushing operator:-, rpn:-5 -0.01  - arithmetic_expression.cc:198
TRACE [parse] to handle " 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = ' ', remaining=" 0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = '0', remaining="0.09) * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperand] rst part=0.09, int part=0, dot part=0.09, dot occurred = yes - arithmetic_expression.cc:261
TRACE [parse] to handle ") * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = ')', remaining=") * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [calculate] opr=-, l_opd=-0.01, r_opd=0.09, rst=-0.1 - arithmetic_expression.cc:327
TRACE [parse] to handle " * 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "right parenthesis", coming char = ' ', remaining=" * 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "* 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "right parenthesis", coming char = '*', remaining="* 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperator] pushing operator:*, rpn:-5 -0.01 0.09 -  - arithmetic_expression.cc:198
TRACE [parse] to handle " 10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = ' ', remaining=" 10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "10 ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = '1', remaining="10 ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperand] rst part=10, int part=10, dot part=0, dot occurred = no - arithmetic_expression.cc:261
TRACE [parse] to handle " ^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = ' ', remaining=" ^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "^ 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = '^', remaining="^ 2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperator] pushing operator:^, rpn:-5 -0.01 0.09 - 10  - arithmetic_expression.cc:198
TRACE [parse] to handle " 2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = ' ', remaining=" 2) / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "2) / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = '2', remaining="2) / 5 " - arithmetic_expression.cc:155
TRACE [handleOperand] rst part=2, int part=2, dot part=0, dot occurred = no - arithmetic_expression.cc:261
TRACE [parse] to handle ") / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = ')', remaining=") / 5 " - arithmetic_expression.cc:155
TRACE [calculate] opr=^, l_opd=10, r_opd=2, rst=100 - arithmetic_expression.cc:327
TRACE [calculate] opr=*, l_opd=-0.1, r_opd=100, rst=-10 - arithmetic_expression.cc:327
TRACE [calculate] opr=+, l_opd=-5, r_opd=-10, rst=-15 - arithmetic_expression.cc:327
TRACE [parse] to handle " / 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "right parenthesis", coming char = ' ', remaining=" / 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "/ 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "right parenthesis", coming char = '/', remaining="/ 5 " - arithmetic_expression.cc:155
TRACE [handleOperator] pushing operator:/, rpn:-5 -0.01 0.09 - 10 2 ^ * +  - arithmetic_expression.cc:198
TRACE [parse] to handle " 5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = ' ', remaining=" 5 " - arithmetic_expression.cc:155
TRACE [parse] to handle "5 " - arithmetic_expression.cc:339
TRACE [handle] current state = "operator", coming char = '5', remaining="5 " - arithmetic_expression.cc:155
TRACE [handleOperand] rst part=5, int part=5, dot part=0, dot occurred = no - arithmetic_expression.cc:261
TRACE [parse] to handle " " - arithmetic_expression.cc:339
TRACE [handle] current state = "operand", coming char = ' ', remaining=" " - arithmetic_expression.cc:155
TRACE [calculate] opr=/, l_opd=-15, r_opd=5, rst=-3 - arithmetic_expression.cc:327
DEBUG [calculate] do nothing for '#' - arithmetic_expression.cc:304
TRACE [parse] arithmetic expression successfully parsed, rpn = "-5 -0.01 0.09 - 10 2 ^ * + 5 / " - arithmetic_expression.cc:384
TRACE [parse] value = -3 - arithmetic_expression.cc:388
TRACE [getExpressionValue] return value = -3 - arithmetic_expression.cc:403
TRACE [main] raw = "(-5 + (-00.01 - 0.09) * 10 ^ 2) / 5 ", rpn = "-5 -0.01 0.09 - 10 2 ^ * + 5 / ", val='-3' - calculator_app.cc:21
```
