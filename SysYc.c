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
          dec_tk = 20, eq_tk = 21, neq_tk = 22, lss_tk = 23, leq_tk = 24,
          gre_tk = 25, geq_tk = 26, assign_tk = 27, semicn_tk = 28,
          comma_tk = 29, lparent_tk = 30, rparent_tk = 31, lbrack_tk = 32,
          rbrack_tk = 33, lbrace_tk = 34, rbrace_tk = 35, intcon_tk = 36,
          charcon_tk = 37, strcon_tk = 38, ident_tk = 39;

int lex_token_idx, lex_token_line;

void token2str(int tk) {
    if (tk == 0) printf("CONSTTK");
    else if (tk == 1) printf("INTTK");
    else if (tk == 2) printf("CHARTK");
    else if (tk == 3) printf("VOIDTK");
    else if (tk == 4) printf("BREAKTK");
    else if (tk == 5) printf("CONTINUETK");
    else if (tk == 6) printf("IFTK");
    else if (tk == 7) printf("ELSETK");
    else if (tk == 8) printf("WHILETK");
    else if (tk == 9) printf("FORTK");
    else if (tk == 10) printf("RETURNTK");
    else if (tk == 11) printf("NOTTK");
    else if (tk == 12) printf("ANDTK");
    else if (tk == 13) printf("ORTK");
    else if (tk == 14) printf("PLUSTK");
    else if (tk == 15) printf("MINUSTK");
    else if (tk == 16) printf("MULTK");
    else if (tk == 17) printf("DIVTK");
    else if (tk == 18) printf("MODTK");
    else if (tk == 19) printf("INCTK");
    else if (tk == 20) printf("DECTK");
    else if (tk == 21) printf("EQTK");
    else if (tk == 22) printf("NEQTK");
    else if (tk == 23) printf("LSSTK");
    else if (tk == 24) printf("LEQTK");
    else if (tk == 25) printf("GRTK");
    else if (tk == 26) printf("GEQTK");
    else if (tk == 27) printf("ASSIGNTK");
    else if (tk == 28) printf("SEMICNTK");
    else if (tk == 29) printf("COMMATK");
    else if (tk == 30) printf("LPARENTTK");
    else if (tk == 31) printf("RPARENTTK");
    else if (tk == 32) printf("LBRACKTK");
    else if (tk == 33) printf("RBRACKTK");
    else if (tk == 34) printf("LBRACETK");
    else if (tk == 35) printf("RBRACETK");
    else if (tk == 36) printf("INTCON");
    else if (tk == 37) printf("CHARCON");
    else if (tk == 38) printf("STRCON");
    else if (tk == 39) printf("IDENTTK");
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
        else {
            unget_char();
            return lss_tk;
        }
    } else if (ch == '>') {
        ch = next_char();
        if (ch == '=') return geq_tk;
        else {
            unget_char();
            return gre_tk;
        }
    } else if (ch == '&') {
        ch = next_char();
        if (ch == '&') return and_tk;
        else {
            unget_char();
            return syntax_error;
        }
    } else if (ch == '|') {
        ch = next_char();
        if (ch == '|') return or_tk;
        else {
            unget_char();
            return syntax_error;
        }
    } else if (ch == '*') {
        return mul_tk;
    } else if (ch == '%') {
        return mod_tk;
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
