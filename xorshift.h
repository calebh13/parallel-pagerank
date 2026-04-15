#include <inttypes.h>

typedef struct XorshiftState XorshiftState;

void seed_state(XorshiftState* state, int pid);
uint64_t xorshift128plus(XorshiftState* state[2]);
double random_double(XorshiftState* state);