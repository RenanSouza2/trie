#define STRUCT(NAME)                    \
    typedef struct _##NAME NAME##_t;    \
    typedef NAME##_t *NAME##_p;         \
    struct _##NAME

STRUCT(string)
{
    char len;
    char arr[0];
};

void string_display(string_p str);
int  string_cmp(string_p str1, char len, char arr[]);
