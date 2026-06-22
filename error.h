#ifndef THROW
#define THROW(...) { fprintf(stderr, "[Error in %s] ", __func__); fprintf(stderr, __VA_ARGS__); fprintf(stderr, "\n"); exit(1); }
#endif