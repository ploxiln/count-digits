#include <time.h>
#include <stdio.h>
#include <stdint.h>

#define NUM_DIGITS (100 * 1000 * 1000)
#define INIT_RNG_STATE 123456789  // for reproducible benchmark

struct xorshift32_state {
  uint32_t a;
};

/* The state word must be initialized to non-zero */
uint32_t xorshift32(struct xorshift32_state *state)
{
	/* Algorithm "xor" from p. 4 of Marsaglia, "Xorshift RNGs" */
	uint32_t x = state->a;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5;
	return state->a = x;
}

double elapsed_sec(struct timespec start, struct timespec end)
{
	return (double)(end.tv_sec - start.tv_sec)
	    + ((double)(end.tv_nsec - start.tv_nsec) / (1000 * 1000 * 1000));
}

char digits[NUM_DIGITS];

int main() {
	struct timespec start_ts, prepped_ts, done_ts;
	struct xorshift32_state state = { INIT_RNG_STATE };

	clock_gettime(CLOCK_MONOTONIC, &start_ts);

	for (size_t i = 0; i < NUM_DIGITS; i++) {
		digits[i] = '0' + (xorshift32(&state) % 10);
	}

	clock_gettime(CLOCK_MONOTONIC, &prepped_ts);
	printf("setup time: %3.3f\n", elapsed_sec(start_ts, prepped_ts));

	// debug check consistency of C and JS
	//for (int i = 0; i < 30; i++) {
	//	printf("digits %d: %c\n", i, digits[i]);
	//}

	int digcnt[10] = {0};
	int invalid = 0;

	for (size_t i = 0; i < NUM_DIGITS; i++) {
		char d = digits[i];
		if (d >= '0' && d <= '9') {
			digcnt[d - '0'] += 1;
		} else {
			invalid++;
		}
	}
	size_t x = 0;
	for (int j = 0; j < 10; j++) {
		for (int k = 0; k < digcnt[j]; k++) {
			digits[x++] = '0' + j;
		}
	}

	clock_gettime(CLOCK_MONOTONIC, &done_ts);
	printf("sort time: %3.3f\n", elapsed_sec(prepped_ts, done_ts));

	for (int j = 0; j < 10; j++) {
		printf("digit %d count: %d\n", j, digcnt[j]);
	}
}
