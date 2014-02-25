#include "icalc.h"

struct LexemList *find_pos(struct LexemList *left, struct LexemList *right, char op)
{
    int balance = 0;
    struct LexemList *pos = right->prev, *ans = NULL;
    while(pos != left)
    {
        struct Lexem *lex = pos->value;
        if(lex->type == T_LPARENT) balance++;
        if(lex->type == T_RPARENT) balance--;
        if(lex->type == T_OP && lex->sval[0] == op && balance == 0) return pos;
        pos = pos->prev;
    }
    return NULL;
}

double complex calculate(struct LexemList *left, struct LexemList *right)
{
    if(left == right) return 0.0 / 0.0;
    struct LexemList *pos;
    // If an [left; right) sub-expression looks like ( sub-expr )
    if(left->value->type == T_LPARENT && right->prev->value->type == T_RPARENT)
    {
        double complex res = calculate(left->next, right->prev);
        if(!isnan(res)) return res + ZERO;
    }
    if((pos = find_pos(left, right, '+')) != NULL)
    {
        return calculate(left, pos) + calculate(pos->next, right) + ZERO;
    } else if((pos = find_pos(left, right, '-')) != NULL)
    {
        return calculate(left, pos) - calculate(pos->next, right) + ZERO;
    } else if((pos = find_pos(left, right, '*')) != NULL)
    {
        return calculate(left, pos) * calculate(pos->next, right) + ZERO;
    } else if((pos = find_pos(left, right, '/')) != NULL)
    {
        return calculate(left, pos) / calculate(pos->next, right) + ZERO;
    } else if((pos = find_pos(left, right, '^')) != NULL)
    {
        return cpow(calculate(left, pos), calculate(pos->next, right)) + ZERO;
    } else if(left->next == right)
    {
        if(left->value->type == T_NUM)
        {
            return left->value->nval + ZERO;
        } else if(left->value->type == T_NAME)
        {
            if(strcmp(left->value->sval, "pi") == 0)
            {
                return M_PI;
            } else if(strcmp(left->value->sval, "e") == 0)
            {
                return M_E;
            } else if(strcmp(left->value->sval, "i") == 0)
            {
                return I;
            }
        }
    } else if(left->value->type == T_OP && left->value->sval[0] == '~')
    {
        return (-1) * calculate(left->next, right) + ZERO;
    } else if(left->value->type == T_NAME)
    {
        if(strcmp(left->value->sval, "sin") == 0)
        {
            return csin(calculate(left->next, right)) + ZERO;
        } else if(strcmp(left->value->sval, "cos") == 0)
        {
            return ccos(calculate(left->next, right)) + ZERO;
        } else if(strcmp(left->value->sval, "exp") == 0)
        {
            return cexp(calculate(left->next, right)) + ZERO;
        } else if(strcmp(left->value->sval, "sqrt") == 0)
        {
            return csqrt(calculate(left->next, right)) + ZERO;
        }
    }
    return 0.0 / 0.0;
}

void calc_free()
{
    struct LexemList *next;
    while(lexemHead != NULL)
    {
        free(lexemHead->value);
        next = lexemHead->next;
        free(lexemHead);
        lexemHead = next;
    }
    lexemTail = NULL;
}

int main()
{
    while(1)
    {
        printf("> ");
        int lex_result = lexer_read('\n');
        if(lex_result == 0) {
            double complex result = calculate(lexemHead, lexemTail);
            if(isnan(result))
            {
                printf("Calculation error\n");
            } else {
                printf("%.5lf + %.5lfi\n", creal(result), cimag(result));
            }
        } else if(lex_result == -1) {
            // End of Line reached
            printf("\n");
            calc_free();
            break;
        }
        lexer_flush('\n');
        calc_free();
    }
    return 0;
}
