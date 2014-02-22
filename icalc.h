#include <complex.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

#define LEX_NAME_LENGTH 16
#define LEX_BUF_LENGTH 256

struct LexemList
{
    struct Lexem *value;
    struct LexemList *next;
};

struct Lexem
{
    enum {
        T_OP,
        T_NUM,
        T_NAME,
        T_LPARENT,
        T_RPARENT,
        T_STOP
    } type;
    char sval[LEX_NAME_LENGTH];
    double nval;
};

struct LexemList *lexemHead, *lexemTail;

int in(int, const char*);
int lexer_read(int);
void lexer_dbg_output();
