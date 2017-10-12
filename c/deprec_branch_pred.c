#ifndef DEBUGPRED
# define unlikely(expr) __builtin_expect (!!(expr), 0)
# define likely(expr) __builtin_expect (!!(expr), 1)
#else
asm (".section predict_data, \"aw\"; .previous\n"
".section predict_line, \"a\"; .previous\n"
".section predict_file, \"a\"; .previous");
# ifdef __x86_64__
# define debugpred__(e, E) \
({ long int _e = !!(e); \
asm volatile (".pushsection predict_data\n" \
"..predictcnt%=: .quad 0; .quad 0\n" \
".section predict_line; .quad %c1\n" \
".section predict_file; .quad %c2; .popsection\n" \
"addq $1,..predictcnt%=(,%0,8)" \
: : "r" (_e == E), "i" (__LINE__), "i" (__FILE__)); \
__builtin_expect (_e, E); \
})
# elif defined __i386__
# define debugpred__(e, E) \
({ long int _e = !!(e); \
asm volatile (".pushsection predict_data\n" \
"..predictcnt%=: .long 0; .long 0\n" \
".section predict_line; .long %c1\n" \
".section predict_file; .long %c2; .popsection\n" \
"incl ..predictcnt%=(,%0,4)" \
: : "r" (_e == E), "i" (__LINE__), "i" (__FILE__)); \
__builtin_expect (_e, E); \
})
# else
# error "debugpred__ definition missing"
# endif
# define unlikely(expt) debugpred__ ((expr), 0)
# define likely(expr) debugpred__ ((expr), 1)
#endif

