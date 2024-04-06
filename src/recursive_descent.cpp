#include <stdio.h>

const char* str = NULL;
const int SYNTAX_ERROR = 0xDEAD;

int get_G (const char* String);
int get_E ();
int get_T ();
int get_P ();
int get_N ();
int syntax_error ();

int main ()
{
    printf ("\n Infix Calculator \n\n");
    const char String[] = "(2+(2*5))*3$";
    int i = 0;
    while (String[i] != '$')
    {
        printf ("%c", String[i]);
        i++;
    }
    //printf ("%s\n", String);
    int value = get_G (String);

    printf (" = %d\n\n", value);

    return 0;
}

int get_G (const char* String)
{
    //printf ("G\n");
    str = String;
    int val = get_E ();

    if (*str == '$')
        str++;
    else
        syntax_error ();

    return val;
}

int get_E ()
{
    //printf ("E\n");
    int val = get_T ();
    while (*str == '+' || *str == '-')
    {
        const char* op = str;
        str++;
        int val_2 = get_T ();
        if (*op == '+')
            val += val_2;
        else if (*op == '-')
            val -= val_2;
    }
    return val;
}

int get_T ()
{
    //printf ("T\n");
    int val = get_P ();
    while (*str == '*' || *str == '/')
    {
        const char* op = str;
        str++;
        int val_2 = get_P ();
        if (*op == '*')
            val *= val_2;
        else if (*op == '/')
            val /= val_2;
    }
    return val;
}

int get_P ()
{
    //printf ("P\n");
    int val = 0;
    if (*str == '(')
    {
        str++;
        val = get_E ();
        if (*str == ')')
        {
            str++;
            return val;
        }
    }
    else
        val = get_N ();

    return val;
}

int get_N ()
{
    //printf ("N\n");
    int val = 0;
    const char* old_str = str;
    while ('0'<= *str && *str <= '9')
    {
        val = val * 10 + (*str - '0');
        str++;
    }

    if (old_str == str)
    {
        syntax_error ();
    }

    return val;
}

int syntax_error ()
{
    printf ("syntax error\n");
    getchar ();
    return SYNTAX_ERROR;
}
