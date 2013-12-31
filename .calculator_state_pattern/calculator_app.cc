#include "log.hh"
#include "arithmetic_expression.hh"

int main(int argc, char *argv[])
{
    if ( argc == 1 ) {
        PR_TRACE("Usage: %s \"infix_expression\" ...", argv[0]);
        return 0;
    }

    const char* sep = "____________________________________";
    double val=0;
    for ( int i = 1; i < argc;  ++i) {
        PR_TRACE("%s%s\n\"%s\"", sep, sep, argv[i]);

        ArithmeticExpression ari_exp(argv[i]);
        if ( ! ari_exp.parse() ) {
            PR_WARN("failed to parse \"%s\"",
                    ari_exp.getArithmeticExpression().c_str());
            continue;
        }

        ari_exp.getExpressionValue(val);
        PR_TRACE("raw = \"%s\", rpn = \"%s\", val='%g'",
                 ari_exp.getArithmeticExpression().c_str(),
                 ari_exp.getRpnExpression().c_str(),
                 val);
    }

    return 0;
}
