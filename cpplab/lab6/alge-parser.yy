%defines
%{
#include <string>

#include "alge.hh"
#include "commands.hh"

int yylex(void);

Command *parsed_command;
%}

%union
{
  double         dval;
  std::string       *sval;
  Command       *cmd;
  AssignCommand     *assn_cmd;
  Expression        *expr_node;
};


%token      TOKEN_EOF   0   "end of file"

%token  <sval>  TOKEN_IDENTIFIER    "identifier"
%token  <dval>  TOKEN_NUMBER        "number"

%type   <cmd>       cmd         "command"
%type   <assn_cmd>  assign          "assignment"
%type   <expr_node> expr            "expression"



%%
%start cmd;

cmd:      assign    { $$ = $1 ; parsed_command = $$ ; }
    | expr      { $$ = new PrintCommand( $1 ); parsed_command = $$ ; }
    ;

assign:   TOKEN_IDENTIFIER '=' expr { $$ = new AssignCommand( new Symbol( *$1 ), $3 ); } ;

%left '+' '-';      /* Addition and subtraction */
%left '*' '/';      /* Multiplication and division */
%left '^';       /* power */
%left NEG;      /* Negation - unary minus */

expr:     '(' expr ')'      { $$ = $2; }
    | expr '+' expr     { $$ = new AddOper( $1 , $3 ); }
    | expr '-' expr     { $$ = new SubOper( $1 , $3 ); }
    | expr '*' expr     { $$ = new MulOper( $1 , $3 ); }
    | expr '/' expr     { $$ = new DivOper( $1 , $3 ); }
    | expr '^' expr     { $$ = new PowOper( $1 , $3 ); }
    | '-' expr %prec NEG    { $$ = new NegOper( $2 ); }
    | TOKEN_IDENTIFIER  { $$ = new Symbol( *$1 ); }
    | TOKEN_NUMBER      { $$ = new Value( $1 ); }
    ;

%%

