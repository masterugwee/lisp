#include<stdio.h>
#include<stdlib.h>
#include "mpc.h"
#ifdef _WIN32


static char buffer[2048];

char* readline(char* prompt){
	fputs(prompt, stdout);
	fgets(buffer, 2048, stdin);
	char* cpy = malloc(strlen(buffer)+1);
	strcpy(cpy,buffer);
	cpy[strlen(cpy)-1]= '\0';
	return cpy;

}

char* add_history(char* history) {}

#elif defined(__linux__)
#include<editline/readline.h>
#endif


enum {LVAL_NUM, LVAL_ERR};
enum { LERR_DIV_ZERO, LERR_BAD_OP, LERR_BAD_NUM };

typedef struct {
	int type;
	union {
	long num;
	int err;
	} un;
} lval;

lval lval_num(long x){
	lval v;
	v.type = LVAL_NUM;
	v.un.num = x;
	return v;
}

lval lval_err(int x) {
	lval v;
	v.type = LVAL_ERR;
	v.un.err = x;
	return v;
}


void lval_print(lval v) {
	switch (v.type) {
	case LVAL_NUM: printf("%li",v.un.num); break;
	case LVAL_ERR: 
	if (v.un.err == LERR_DIV_ZERO) { printf("Error: Division by zero");	}
	if (v.un.err == LERR_BAD_OP) {printf("Error: Invalid Operator!"); }
	if (v.un.err == LERR_BAD_NUM) { printf("Error: Invalid Number!");}
	break;
}

}

void lval_println(lval v) {lval_print(v); putchar('\n');}


long power(long x, long y){
	int result = 1;
	for(int i=0;i<y;i++){
	result *= x;
	}
	return result;
}

long min(long x, long y) {
	if (x<y) {return x;}
	else {return y;}
}

long max(long x, long y) {
	if (x<y) {return y;} else {return x;}
}

lval eval_op(lval x, char* op, lval y){
	
	if (x.type == LVAL_ERR) { return x;}
	if (y.type == LVAL_ERR) { return y;}


	if (strcmp(op,"+") == 0) {return lval_num(x.un.num+y.un.num);}
	if (strcmp(op,"-") == 0) {return lval_num(x.un.num - y.un.num);}
	if (strcmp(op,"*") == 0) {return lval_num(x.un.num * y.un.num);}
	if (strcmp(op,"/") == 0) {return y.un.num == 0 ? 
					 lval_err(LERR_DIV_ZERO) :
					 lval_num(x.un.num / y.un.num);}
	if (strcmp(op,"^") == 0) {return lval_num(power(x.un.num , y.un.num));}
	if (strcmp(op,"%") == 0) {return y.un.num ==0 ? lval_err(LERR_DIV_ZERO):
							lval_num(x.un.num % y.un.num);}
	if (strcmp(op,"min") == 0) {return lval_num(min(x.un.num , y.un.num));}
	if (strcmp(op,"max") == 0) {return lval_num(max(x.un.num , y.un.num));}
	return lval_err(LERR_BAD_OP);
	
}


lval eval(mpc_ast_t* t){
	if(strstr(t->tag,"number")) {
		errno = 0;
		long x = strtol(t->contents, NULL, 10);
		return errno != ERANGE ? lval_num(x) : lval_err(LERR_BAD_NUM);
	}
	char* op = t->children[1]->contents;
	lval x = eval(t->children[2]);
	int i=3;
	if (strcmp(op,"-") == 0 && (t->children_num <= i || !strstr(t->children[i]->tag, "expr")))
		 { return lval_num(-x.un.num);}
	while(strstr(t->children[i]->tag, "expr")){
		x = eval_op(x, op, eval(t->children[i]));
		i++;
	}

	return x;
}





int main(int argc,char** argv){

	mpc_parser_t* Number = mpc_new("number");
	mpc_parser_t* Operator = mpc_new("operator");
	mpc_parser_t* Expr = mpc_new("expr");
	mpc_parser_t* Lispy = mpc_new("lispy");

	mpca_lang(MPCA_LANG_DEFAULT,
	"					\
	number	: /-?[0-9]+/;			\
	operator : '+' | '-' | '*' | '/' | '^' | '%' | /min/ | /max/;	\
	expr	: <number> | '(' <operator> <expr>+ ')'; \
	lispy	: /^/ <operator> <expr>+ /$/ ;	\
	",Number, Operator, Expr, Lispy);
	
	puts("Lispy version 0.0");
	puts("Ctrl+c to exit\n");

	while(1)
	{
		char* input = readline("lispy> ");
		add_history(input);
		//printf("No you're a %s\n", input);
		mpc_result_t r;
		if(mpc_parse("<stdin>", input, Lispy, &r)){
			lval result = eval(r.output);
			lval_println(result);
			//mpc_ast_print(r.output);
			mpc_ast_delete(r.output);
		} else {
			mpc_err_print(r.error);
			mpc_err_delete(r.error);
		}
		free(input);
	}
	
	mpc_cleanup(4, Number, Operator, Expr, Lispy);
	return 0;
}
