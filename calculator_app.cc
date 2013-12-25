#include "log.hh"
#include "calculator_core.hh"

int main(int argc, char *argv[])
{
    if ( argc == 1 ) {
        PR_TRACE("Usage: %s \"infix_expression\" ...", argv[0]);
        return 0;
    }

    const char* sep = "________________________________________________________________";
    for ( int i = 1; i < argc;  ++i) {
        PR_TRACE("%s\"%s\"", sep, argv[i]);
        ArithmeticExpression ari_exp(argv[i]);
        ari_exp.parse();
        PR_TRACE("rpn = \"%s\"", ari_exp.getRpnExpression().c_str());
    }

    return 0;
}
