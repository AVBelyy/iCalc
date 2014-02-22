#include "icalc.h"

struct LexemList *find_pos(struct LexemList *left, struct LexemList *right, char op)
{
    int balance = 0;
    struct LexemList *pos = left;
    while(pos->next != right)
    {
        struct Lexem *lex = pos->value;
        if(lex->type == T_LPARENT) balance++;
        if(lex->type == T_RPARENT) balance--;
        if(lex->type == T_OP && lex->sval[0] == op && balance == 0) return pos;
        pos = pos->next;
    }
    return NULL;
}

double complex calculate(struct LexemList *left, struct LexemList *right)
{
    if(left == right) return 0.0 / 0.0;
    struct LexemList *pos;
    // If an [left; right) sub-expression looks like ( sub-expr )
    pos = left;
    while(pos->next != right)
    {
        pos = pos->next;
        if(pos->next == right && left->value->type == T_LPARENT && pos->value->type == T_RPARENT)
        {
            double complex res = calculate(left->next, pos);
            if(!isnan(res)) return res;
        }
    }
    if((pos = find_pos(left, right, '+')) != NULL)
    {
        return calculate(left, pos) + calculate(pos->next, right);
    } else if((pos = find_pos(left, right, '-')) != NULL)
    {
        return calculate(left, pos) - calculate(pos->next, right);
    } else if((pos = find_pos(left, right, '*')) != NULL)
    {
        return calculate(left, pos) * calculate(pos->next, right);
    } else if((pos = find_pos(left, right, '/')) != NULL)
    {
        return calculate(left, pos) / calculate(pos->next, right);
    } else if((pos = find_pos(left, right, '^')) != NULL)
    {
        return cpow(calculate(left, pos), calculate(pos->next, right));
    } else if(left->next == right)
    {
        if(left->value->type == T_NUM)
        {
            return left->value->nval;
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
        return (-1) * calculate(left->next, right);
    } else if(left->value->type == T_NAME)
    {
        if(strcmp(left->value->sval, "sin") == 0)
        {
            return csin(calculate(left->next, right));
        } else if(strcmp(left->value->sval, "cos") == 0)
        {
            return ccos(calculate(left->next, right));
        } else if(strcmp(left->value->sval, "exp") == 0)
        {
            return cexp(calculate(left->next, right));
        } else if(strcmp(left->value->sval, "sqrt") == 0)
        {
            return csqrt(calculate(left->next, right));
        }
    }
    return 0.0 / 0.0;
}

int main()
{
    printf("> ");
    if(lexer_read('\n') == 0)
    {
        double complex result = calculate(lexemHead, lexemTail);
        if(isnan(result))
        {
            printf("Calculation error\n");
            return 1;
        } else {
            printf("%.5lf + %.5lfi\n", creal(result), cimag(result));
        }
        return 0;
    } else {
        return 1;
    }
}
