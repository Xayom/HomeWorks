#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctype.h>
int tok;
double tokval;

int next() {
	for (;;) {
		int c = getchar();
		if (c == EOF || strchr("+%-*/()\n", c) != NULL) return tok = c;
		if (isspace(c)) continue;
		if (isdigit(c) || c == '.') {
			ungetc(c, stdin);
			scanf(" %lf", &tokval);
			return tok = 'n';
		}
		fprintf(stderr, "Bad character: %c\n", c); exit(0);
	}
}

void skip(char t)
{
   // printf("%c %c\n", tok, t);
    if(tok != t){
        printf("Incorrect input");
        exit(0);
    }
    next();
}

double expr();

// numpar ::= number | '(' expr ')'
int numpar() {
	if (tok == 'n') { double x = tokval; skip('n'); return x; }
	skip('('); double x = expr(); skip(')'); return x;
}

// term ::= numpar | term '*' numpar | term '/' numpar
double term() {
	int x = numpar();
    //printf("%c/n", tok);
	for (;;) {
		if (tok == '*') {
		    skip('*');
            x *= numpar();
        }
		else
            if (tok == '/') {
                skip('/');
                int k = numpar();
                if (k == 0)
                {
                    printf("Division by zero");
                    exit(0);
                }
                x /= k;
            }
		else if (tok == '%') { skip('%'); x %= numpar(); }
		else
             if (tok == '(')
             {
                printf("Incorrect input");
                exit(0);
             }
            else
                return x;
	}
}

// expr ::= term | expr '+' term | expr '-' term
double expr() {
	double x = term();
	for (;;) {
		if (tok == '+') { skip('+'); x += term(); }
		else if (tok == '-') { skip('-'); x -= term(); }
		else return x;
	}
}

int main()
{
	next();
    if (tok == '\n') skip('\n');
    printf("%.9g\n", expr());
	return 0;
}
