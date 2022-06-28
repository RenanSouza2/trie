#define STRUCT(NAME)                    \
    typedef struct _##NAME NAME##_t;    \
    typedef NAME##_t *NAME##_p;         \
    struct _##NAME
