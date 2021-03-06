#include "icalc.h"

// Lexems
const char *t_whitespace = " \t\n";
const char *t_op = "+-*/^";
const char *t_parent = "()";

int cur;

struct LexemList *lexemHead, *lexemTail;

void get()
{
    cur = getchar();
}

int in(int c, const char *s)
{
    int i, len = strlen(s);
    for(i = 0; i < len; i++)
    {
        if(s[i] == c)
        {
            return TRUE;
        }
    }
    return FALSE;
}

int lex_error()
{
    printf("Wrong expression format\n");
    return 1;
}

void lexem_push(struct Lexem *val)
{
    struct LexemList *item = (struct LexemList *)malloc(sizeof(struct LexemList));
    if(lexemTail != NULL)
    {
        lexemTail->next = item;
        item->prev = lexemTail;
    } else {
        lexemHead = item;
        item->prev = NULL;
    }
    item->value = val;
    item->next = NULL;
    lexemTail = item;
}

void lexer_dbg_output(void)
{
    struct LexemList *item = lexemHead;
    while(item != NULL)
    {
        switch(item->value->type)
        {
            case T_OP: printf("T_OP"); break;
            case T_NUM: printf("T_NUM"); break;
            case T_NAME: printf("T_NAME"); break;
            case T_LPARENT: printf("T_LPARENT"); break;
            case T_RPARENT: printf("T_RPARENT"); break;
            case T_STOP: printf("T_STOP"); break;
        }
        printf(" | sval=\"%s\" | nval=%lf\n", item->value->sval, item->value->nval);
        item = item->next;
    }
}

int lexer_read(int stop)
{
    int prev, buflen;
    char buf[LEX_BUF_LENGTH];
    get();
    while(cur != stop && cur != EOF)
    {
        if(in(cur, t_whitespace))
        {
            // Ignore whitespaces
            get();
        } else if(in(cur, t_op)) {
            struct Lexem *lex = (struct Lexem *)malloc(sizeof(struct Lexem));
            lex->type = T_OP;
            lex->sval[0] = cur;
            lex->sval[1] = '\0';
            prev = cur;
            get();
            cur = tolower(cur);
            if(prev == '-')
            {
                // Maybe, it's an unary minus
                if((in(cur, t_parent) || (cur >= '0' && cur <= '9') || (cur == '.') || (cur >= 'a' && cur <= 'z')) && (!lexemTail || lexemTail->value->type == T_OP || lexemTail->value->type == T_LPARENT))
                {
                    lex->sval[0] = '~';
                }
            }
            lexem_push(lex);
        } else if(cur == t_parent[0]) {
            struct Lexem *lex = (struct Lexem *)malloc(sizeof(struct Lexem));
            lex->type = T_LPARENT;
            lexem_push(lex);
            get();
        } else if(cur == t_parent[1]) {
            struct Lexem *lex = (struct Lexem *)malloc(sizeof(struct Lexem));
            lex->type = T_RPARENT;
            lexem_push(lex);
            get();
        } else {
            cur = tolower(cur);
            buflen = 0;
            while(cur >= 'a' && cur <= 'z')
            {
                buf[buflen++] = cur;
                get();
                cur = tolower(cur);
                if(!((cur >= 'a' && cur <= 'z') || in(cur, t_whitespace) || in(cur, t_op) || in(cur, t_parent))) return lex_error();
            }
            if(buflen != 0)
            {
                struct Lexem *lex = (struct Lexem *)malloc(sizeof(struct Lexem));
                lex->type = T_NAME;
                strncpy(lex->sval, buf, buflen);
                lex->sval[buflen] = '\0';
                lexem_push(lex);
            } else {
                buflen = 0;
                while((cur >= '0' && cur <= '9') || (cur == '.'))
                {
                    buf[buflen++] = cur;
                    get();
                    if(!((cur >= '0' && cur <= '9') || (cur == '.') || in(cur, t_whitespace) || in(cur, t_op) || in(cur, t_parent))) return lex_error();
                }
                if(buflen != 0)
                {
                    struct Lexem *lex = (struct Lexem *)malloc(sizeof(struct Lexem));
                    lex->type = T_NUM;
                    buf[buflen] = '\0';
                    sscanf(buf, "%lf", &lex->nval);
                    lexem_push(lex);
                } else {
                    return lex_error();
                }
            }
        }
    }
    if(cur == EOF) return -1;
    struct Lexem *lex = (struct Lexem *)malloc(sizeof(struct Lexem));
    lex->type = T_STOP;
    lexem_push(lex);
    return 0;
}

void lexer_flush(int stop)
{
    while(cur != stop)
    {
        get();
    }
}
