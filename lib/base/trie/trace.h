#ifndef __TRIRE_TRACE_H__
#define __TRIRE_TRACE_H__

#ifdef DEBUG

#define DEF_I(TYPE) \
    int TYPE##_created; \
    int TYPE##_freed;

#define DEF_E(TYPE) \
    extern int TYPE##_created; \
    extern int TYPE##_freed;

void assert_memory();

#define decrease(created, freed) 

#define INC(INT) INT##_created++
#define DEC(INT) \
    {   \
        assert(INT##_created > INT##_freed); \
        INT##_freed++; \
    }
#else
#define INC(INT)
#define DEC(INT)
#endif

#endif