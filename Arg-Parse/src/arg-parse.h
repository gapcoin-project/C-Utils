/**
 *  simple Argumnet parsing functions
 */
#ifndef __ARG_PARSE_H__
#define __ARG_PARSE_H__

/**
 * Returns if the given args conatinig the given arg
 */
char has_arg(int argc, 
             char *argv[], 
             const char *short_arg, 
             const char *long_arg);

/**
 * Returns the given argument arg
 */
char *get_arg(int argc, 
              char *argv[], 
              const char *short_arg, 
              const char *long_arg);

/**
 * shorter macros to use in main
 */
#define has_arg1(long_arg) has_arg(argc, argv, NULL, long_arg)
#define has_arg2(short_arg, long_arg) has_arg(argc, argv, short_arg, long_arg)
#define has_arg4(argc, argv, short_arg, long_arg) \
  has_arg(argc, argv, short_arg, long_arg)
#define has_argx(X, T1, T2, T3, T4, FUNC, ...) FUNC
#define has_arg(...) has_argx(, ##__VA_ARGS__,            \
                                has_arg4(__VA_ARGS__),,   \
                                has_arg2(__VA_ARGS__),    \
                                has_arg1(__VA_ARGS__))

#define get_arg1(long_arg) get_arg(argc, argv, NULL, long_arg)
#define get_arg2(short_arg, long_arg) get_arg(argc, argv, short_arg, long_arg)
#define get_arg4(argc, argv, short_arg, long_arg) \
  get_arg(argc, argv, short_arg, long_arg)
#define get_argx(X, T1, T2, T3, T4, FUNC, ...) FUNC
#define get_arg(...) get_argx(, ##__VA_ARGS__,            \
                                get_arg4(__VA_ARGS__),,   \
                                get_arg2(__VA_ARGS__),    \
                                get_arg1(__VA_ARGS__))


#endif /* __ARG_PARSE_H__ */
