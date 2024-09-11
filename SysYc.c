/// SysYc
/// Author: TobyShi
/// Date: 2024-09-11

#include <stdio.h>
#include <stdlib.h>

// errors
const int eof = -1, oom_error = -2, syntax_error = -3, semantic_error = -4;

// source pool
char source[1024 * 256]; // 256 kb
int source_index, source_line = 1;

int next_char() {
    if (source_index < 0) return source_index;
    if (source_index == 1024 * 256) {
        source_index = oom_error;
        return oom_error;
    }
    if (source[source_index]) {
        if (source[source_index] == '\n') source_line++;
        return source[source_index++];
    }
    int ch = getchar();
    if (ch == eof) {
        source_index = -1;
        return eof;
    }
    source[source_index] = ch;
    if (ch == '\n') source_line++;
    return source[source_index++];
}

void unget_char() {
    if (source[--source_index] == '\n') source_line--;
}

// enum for token type
const int const_tk = 0, int_tk = 1, char_tk = 2, void_tk = 3, break_tk = 4,
          continue_tk = 5, if_tk = 6, else_tk = 7, while_tk = 8, for_tk = 9,
          return_tk = 10, not_tk = 11, and_tk = 12, or_tk = 13, plus_tk = 14,
          minus_tk = 15, mul_tk = 16, div_tk = 17, mod_tk = 18, inc_tk = 19,
          dec_tk = 20, shl_tk = 21, shr_tk = 22, eq_tk = 23, neq_tk = 24,
          lss_tk = 25, leq_tk = 26, gre_tk = 27, geq_tk = 28, bitand_tk = 29,
          bitxor_tk = 30, bitor_tk = 31, assign_tk = 32, semicn_tk = 33,
          comma_tk = 34, lparent_tk = 35, rparent_tk = 36, lbrack_tk = 37,
          rbrack_tk = 38, lbrace_tk = 39, rbrace_tk = 40, intcon_tk = 41,
          charcon_tk = 42, strcon_tk = 43, ident_tk = 44;

int lex_token_idx, lex_token_line;

void token2str(int tk) {
    if (tk == const_tk) printf("CONSTTK");
    else if (tk == int_tk) printf("INTTK");
    else if (tk == char_tk) printf("CHARTK");
    else if (tk == void_tk) printf("VOIDTK");
    else if (tk == break_tk) printf("BREAKTK");
    else if (tk == continue_tk) printf("CONTINUETK");
    else if (tk == if_tk) printf("IFTK");
    else if (tk == else_tk) printf("ELSETK");
    else if (tk == while_tk) printf("WHILETK");
    else if (tk == for_tk) printf("FORTK");
    else if (tk == return_tk) printf("RETURNTK");
    else if (tk == not_tk) printf("NOTTK");
    else if (tk == and_tk) printf("ANDTK");
    else if (tk == or_tk) printf("ORTK");
    else if (tk == plus_tk) printf("PLUSTK");
    else if (tk == minus_tk) printf("MINUSTK");
    else if (tk == mul_tk) printf("MULTK");
    else if (tk == div_tk) printf("DIVTK");
    else if (tk == mod_tk) printf("MODTK");
    else if (tk == inc_tk) printf("INCTK");
    else if (tk == dec_tk) printf("DECTK");
    else if (tk == shl_tk) printf("SHLTK");
    else if (tk == shr_tk) printf("SHRTK");
    else if (tk == eq_tk) printf("EQTK");
    else if (tk == neq_tk) printf("NEQTK");
    else if (tk == lss_tk) printf("LSSTK");
    else if (tk == leq_tk) printf("LEQTK");
    else if (tk == gre_tk) printf("GRETk");
    else if (tk == geq_tk) printf("GEQTK");
    else if (tk == bitand_tk) printf("BITANDTK");
    else if (tk == bitxor_tk) printf("BITXORTK");
    else if (tk == bitor_tk) printf("BITORTK");
    else if (tk == assign_tk) printf("ASSIGNTK");
    else if (tk == semicn_tk) printf("SEMICNTK");
    else if (tk == comma_tk) printf("COMMATK");
    else if (tk == lparent_tk) printf("LPARENTTK");
    else if (tk == rparent_tk) printf("RPARENTTK");
    else if (tk == lbrack_tk) printf("LBRACKTK");
    else if (tk == rbrack_tk) printf("RBRACKTK");
    else if (tk == lbrace_tk) printf("LBRACETK");
    else if (tk == rbrace_tk) printf("RBRACETK");
    else if (tk == intcon_tk) printf("INTCONTK");
    else if (tk == charcon_tk) printf("CHARCONTK");
    else if (tk == strcon_tk) printf("STRCONTK");
    else if (tk == ident_tk) printf("IDENTTK");
    else printf("ERROR");
}

int lex_strcmp(char tk[]) {
    unget_char();
    int ch, i;
    for (i = 0; tk[i]; i++) {
        ch = next_char();
        if (ch != tk[i]) break;
    }
    if (tk[i] == '\0') return 1;
    for (; i > 0; i--) unget_char();
    return 0;
}

int next_token() {
    int ch = next_char();
    while (ch == ' ' || ch == '\r' || ch == '\n' || ch == '\t')
        ch = next_char();
    lex_token_idx = source_index - 1;
    lex_token_line = source_line;
    if (ch < 0) {
        return ch;
    } else if (ch == '#') {
        while (ch != '\n') ch = next_char();
        return next_token();
    } else if (ch == '/') {
        ch = next_char();
        if (ch == '/') {
            while (ch != '\n') ch = next_char();
            return next_token();
        } else if (ch == '*') {
            ch = next_char();
            while (ch != '*' || next_char() != '/') ch = next_char();
            return next_token();
        } else {
            unget_char();
            return div_tk;
        }
    } else if (ch == '+') {
        ch = next_char();
        if (ch == '+') return inc_tk;
        else {
            unget_char();
            return plus_tk;
        }
    } else if (ch == '-') {
        ch = next_char();
        if (ch == '-') return dec_tk;
        else {
            unget_char();
            return minus_tk;
        }
    } else if (ch == '=') {
        ch = next_char();
        if (ch == '=') return eq_tk;
        else {
            unget_char();
            return assign_tk;
        }
    } else if (ch == '!') {
        ch = next_char();
        if (ch == '=') return neq_tk;
        else {
            unget_char();
            return not_tk;
        }
    } else if (ch == '<') {
        ch = next_char();
        if (ch == '=') return leq_tk;
        else if (ch == '<') return shl_tk;
        else {
            unget_char();
            return lss_tk;
        }
    } else if (ch == '>') {
        ch = next_char();
        if (ch == '=') return geq_tk;
        else if (ch == '>') return shr_tk;
        else {
            unget_char();
            return gre_tk;
        }
    } else if (ch == '&') {
        ch = next_char();
        if (ch == '&') return and_tk;
        else {
            unget_char();
            return bitand_tk;
        }
    } else if (ch == '|') {
        ch = next_char();
        if (ch == '|') return or_tk;
        else {
            unget_char();
            return bitor_tk;
        }
    } else if (ch == '*') {
        return mul_tk;
    } else if (ch == '%') {
        return mod_tk;
    } else if (ch == '^') {
        return bitxor_tk;
    } else if (ch == ';') {
        return semicn_tk;
    } else if (ch == ',') {
        return comma_tk;
    } else if (ch == '(') {
        return lparent_tk;
    } else if (ch == ')') {
        return rparent_tk;
    } else if (ch == '[') {
        return lbrack_tk;
    } else if (ch == ']') {
        return rbrack_tk;
    } else if (ch == '{') {
        return lbrace_tk;
    } else if (ch == '}') {
        return rbrace_tk;
    } else if (lex_strcmp("const")) {
        return const_tk;
    } else if (lex_strcmp("int")) {
        return int_tk;
    } else if (lex_strcmp("char")) {
        return char_tk;
    } else if (lex_strcmp("void")) {
        return void_tk;
    } else if (lex_strcmp("break")) {
        return break_tk;
    } else if (lex_strcmp("continue")) {
        return continue_tk;
    } else if (lex_strcmp("if")) {
        return if_tk;
    } else if (lex_strcmp("else")) {
        return else_tk;
    } else if (lex_strcmp("while")) {
        return while_tk;
    } else if (lex_strcmp("for")) {
        return for_tk;
    } else if (lex_strcmp("return")) {
        return return_tk;
    } else if (ch == '\'') {
        ch = next_char();
        if (ch == '\\') ch = next_char();
        if (next_char() != '\'') return syntax_error;
        return charcon_tk;
    } else if (ch == '"') {
        while (next_char() != '"')
            if (ch == '\\') ch = next_char();
        return strcon_tk;
    } else if (ch >= '0' && ch <= '9') {
        while (ch >= '0' && ch <= '9') ch = next_char();
        unget_char();
        return intcon_tk;
    } else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
               ch == '_') {
        while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') ||
               (ch >= '0' && ch <= '9') || ch == '_')
            ch = next_char();
        unget_char();
        return ident_tk;
    } else {
        return syntax_error;
    }
}

int print_tokens() {
    int last_line = 0;
    while (1) {
        int tk = next_token();
        if (tk < -1) {
            printf("\nError at line %d\n", lex_token_line);
            return -1;
        }
        if (tk < 0) break;
        if (last_line != lex_token_line) printf("\n%d ", lex_token_line);
        else printf(" ");
        token2str(tk);
        last_line = lex_token_line;
    }
    putchar(10);
    return 0;
}

int main() {
    if (print_tokens() < 0) return -1;
    return 0;
}
