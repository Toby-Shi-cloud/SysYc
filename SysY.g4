grammar SysY;

compUnit: (decl | funcDef)*;

decl: constDecl | varDecl;

constDecl: CONSTTK bType constDef (COMMA constDef)* SEMICN;
bType: INTTK | CHARTK;
constDef: IDENFR (LBRACK constExp RBRACK)* ASSIGN constInitVal;
constInitVal: constExp | LBRACE constInitVal (COMMA constInitVal)* RBRACE;

varDecl: bType varDef (COMMA varDef)* SEMICN;
varDef: IDENFR (LBRACK constExp RBRACK)* (ASSIGN initVal)?;
initVal: exp | LBRACE initVal (COMMA initVal)* RBRACE;

funcDef: funcType IDENFR LPARENT funcFParams? RPARENT block;
funcType: INTTK | VOIDTK;
funcFParams: funcFParam (COMMA funcFParam)*;
funcFParam: bType IDENFR (LBRACK RBRACK (LBRACK constExp RBRACK)*)?;

block: LBRACE blockItem* RBRACE;
blockItem: decl | stmt;

stmt: lVal ASSIGN exp SEMICN # assignStmt
    | exp? SEMICN # expStmt
    | block # blockStmt
    | IFTK LPARENT exp RPARENT stmt (ELSETK stmt)? # ifStmt
    | WHILETK LPARENT exp RPARENT stmt # whileStmt
    | FORTK LPARENT forStmt? SEMICN exp? SEMICN forStmt? RPARENT stmt # forLoopStmt
    | BREAKTK SEMICN # breakStmt
    | CONTINUETK SEMICN # continueStmt
    | RETURNTK exp? SEMICN # returnStmt
    ;
forStmt: lVal ASSIGN exp;
exp: lOrExp;
lVal: IDENFR (LBRACK exp RBRACK)*;
primaryExp: LPARENT exp RPARENT | literal | lVal selfOp | selfOp? lVal;
literal: INTCON | CHARCON | STRCON;
unaryExp: primaryExp | IDENFR LPARENT funcRParams? RPARENT | unaryOp unaryExp;
unaryOp: PLUS | MINU | NOT;
selfOp: INC | DEC;
funcRParams: exp (COMMA exp)*;
mulExp: unaryExp ((MULT | DIV | MOD) unaryExp)*;
addExp: mulExp ((PLUS | MINU) mulExp)*;
shiftExp: addExp ((SHL | SHR) addExp)*;
relExp: shiftExp ((LSS | LEQ | GRE | GEQ) shiftExp)*;
eqExp: relExp ((EQL | NEQ) relExp)*;
bAndExp: eqExp (BITAND eqExp)*;
bXorExp: bAndExp (BITXOR bAndExp)*;
bOrExp: bXorExp (BITOR bXorExp)*;
lAndExp: bOrExp (AND bOrExp)*;
lOrExp: lAndExp (OR lAndExp)*;
constExp: lOrExp;

// literal
INTCON: '0' | [1-9][0-9]*;
CHARCON: '\'' CHARSET '\'';
STRCON: '"' CHARSET*? '"';

// chars
CHARSET: [^\\] | TRANSCHAR;
TRANSCHAR: '\\' [0ntr'"\\];

// keywords
CONSTTK: 'const';
INTTK: 'int';
CHARTK: 'char';
VOIDTK: 'void';
BREAKTK: 'break';
CONTINUETK: 'continue';
IFTK: 'if';
ELSETK: 'else';
WHILETK: 'while';
FORTK: 'for';
RETURNTK: 'return';

// operators
NOT: '!';
AND: '&&';
OR: '||';
PLUS: '+';
MINU: '-';
MULT: '*';
DIV: '/';
MOD: '%';
INC: '++';
DEC: '--';
SHL: '<<';
SHR: '>>';
LSS: '<';
LEQ: '<=';
GRE: '>';
GEQ: '>=';
EQL: '==';
NEQ: '!=';
BITAND: '&';
BITXOR: '^';
BITOR: '|';
ASSIGN: '=';
SEMICN: ';';
COMMA: ',';

// parentheses
LPARENT: '(';
RPARENT: ')';
LBRACK: '[';
RBRACK: ']';
LBRACE: '{';
RBRACE: '}';

// identifier
IDENFR: [a-zA-Z_][a-zA-Z0-9_]*;

// whitespaces and comments
WS: [ \r\n\t]+ -> skip;
LineComment: '//' ~ [\r\n]* -> skip;
BlockCommit: '/*' .*? '*/' -> skip;
Preprocess: '#' .*? '\n' -> skip;
