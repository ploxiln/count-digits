console.log("starting");

const NUM_DIGITS = 100 * 1000 * 1000;
const INIT_RNG_STATE = 123456789;  // for reproducible benchmark
const digchars = '0123456789';

function xorshift32(s) {
	s[0] ^= s[0] << 13;
	s[0] ^= s[0] >>> 17;
	s[0] ^= s[0] << 5;
	return s[0];
}
let rng_state = Uint32Array.of(INIT_RNG_STATE);

let digits = Array(NUM_DIGITS);

let start_ts = Date.now()

for (let i = 0; i < NUM_DIGITS; i++) {
	digits[i] = digchars[xorshift32(rng_state) % 10];
}

// debug check consistency of C and JS
//for (let i = 0; i < 30; i++) {
//	console.log("digits " + i + ": " + digits[i]);
//}

let prepped_ts = Date.now()
console.log("setup time: " + ((prepped_ts - start_ts) / 1000))

