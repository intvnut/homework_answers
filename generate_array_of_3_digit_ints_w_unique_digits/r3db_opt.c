#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define ROUND_UP(x, y) ((((x - 1)) / (y) + 1) * (y))

#define LFIB_SIZE (512)
#define LFIB_DROP (23)
static uint64_t lfib[LFIB_SIZE] __attribute__((aligned(128)));

// Initializes the state for the lagged Fibonacci random number generate.
// This uses a 64-bit LFSR with a 64-bit seed.  It's not exactly awesome,
// but it's adequate.
void srand_lfib(uint64_t seed) {
  const uint64_t poly = 0xD6C9E91ACA649AD4;

  if (!seed) {
    seed = 1;
  }

  for (int i = 0; i < 512; ++i) {
    for (int j = 0; j < 64; ++j) {
      seed = (seed >> 1) ^ (seed & 1 ? poly : 0);
    }

    lfib[i] = seed;
  }
}

// Generates another full block of data in lfib[].  This is a lagged Fibonnaci
// random number generator, with lags 258 and 83.  These come from Table 1 in
// TAOCP Vol 2, section 3.2.2.
//
// The period of this generator should be 2^63 * (2^258 - 1), but that applies
// to the 64-bit outputs *as a whole*.  Bit number n has period
// 2^(n-1) * (2^258 - 1).  To get a higher quality result, whatever consumes
// this output should rotate the i^th output by i % 64, or apply some other
// transformation to mix all the bits together.
//
// But for this exercise, this is way more than adequate.
static void update_lfib_block(void) {
  // lags of 83 and 258.
  for (int i = 0; i < 83; ++i) {
    lfib[i] = lfib[i + LFIB_SIZE - 83] + lfib[i + LFIB_SIZE - 258];
  }

  for (int i = 83; i < 258; ++i) {
    lfib[i] = lfib[i - 83] + lfib[i + LFIB_SIZE - 258];
  }

  for (int i = 258; i < LFIB_SIZE; ++i) {
    lfib[i] = lfib[i - 83] + lfib[i - 258];
  }
}


// The mKp constants are what Knuth calls mu_k, where each bit position is 1
// where bit number n has a 1 if n has a 0 in bit k.  They also happen to be
// the binary fractions 1 / (2^(n+2) - 1).
//
// The mKn constants are just the inverse of the corresponding mKp constants.
enum {
    m0p = 0x5555555555555555, m0n = ~m0p,
    m1p = 0x3333333333333333, m1n = ~m1p,
    m2p = 0x0F0F0F0F0F0F0F0F, m2n = ~m2p,
    m3p = 0x00FF00FF00FF00FF, m3n = ~m3p,
};

// Generates random 3-digit decimal numbers with the following constraints:
//
//  -- The leading digit is non-zero.
//  -- The three digits are all different.
//
// This uses a bitslice implementation.  Bitslice implementations compute the
// output as a series of boolean computations on individual bits of
// intermediate values and the final result.  That's right, it's a bit-serial
// implementation of the algorithm.
//
// Or, if you prefer, it's how you would implement it with 1-input and 2-input
// logic gates.  So, you will see me refer to "gate counts" below.  Gate counts
// correspond to instruction counts.
//
// Where it gains its speed is from applying that bit-serial algorithm to all
// of the bits in a machine word in parallel.  With a 64-bit machine word, we
// can calculate 64 results as fast as we can compute a single result.
//
// This optimized version goes further.  It requires the output buffer to be
// a multiple of 64 entries on a 64-bit boundary.
void gen_rand_3dig(uint16_t *restrict out, size_t count) {
  // Generate batches of 64 with a bit-slice implementation.
  enum {
    RBITS_ITER = 21,  // How many bits we consume per iteration.
    REFILL_IVAL = (LFIB_SIZE - LFIB_DROP) / RBITS_ITER
  };

  // As I warned above, the output buffer must be a multiple of 64 entries...
  count = ROUND_UP(count, 64);

  uint64_t *restrict o64 = (uint64_t *)out;
  for (size_t j = 0; j < count; j += 64 * REFILL_IVAL) {
    // Generate the next block of LFIB_SIZE random 64-bit values.
    update_lfib_block();

    const uint64_t *restrict lf = lfib;
    const size_t rem = count - j;
    const size_t iter = rem < 64 * REFILL_IVAL ? rem : 64 * REFILL_IVAL;
    for (size_t i = 0; i < iter; i += 64) {
      // Extract a random 9-bit value.  That literally just requires pulling 9
      // values from the lfib[] array.  Recall my comment above about the
      // period of each bit, and how bits further to the left have longer
      // periods.  This is where rotating each value by an increasing amount
      // would spread that around all the bit lanes.
      const uint64_t b00 = *lf++;
      const uint64_t b01 = *lf++;
      const uint64_t b02 = *lf++;
      const uint64_t b03 = *lf++;
      const uint64_t b04 = *lf++;
      const uint64_t b05 = *lf++;
      const uint64_t b06 = *lf++;
      const uint64_t b07 = *lf++;
      const uint64_t b08 = *lf++;

      // A quick aside at how I generate a random number in the range 0..8 from
      // a 9-bit value with minimal bias and minimal gate count.
      //
      // It turns out values of the form 2^(3k) mod 9 are +/-1, for k > 0.  Why
      // is that important?  That means that 2^(3k) is very nearly a multiple of
      // 9.  Thus, over the range [0, 2^(3k) - 1], all of the values mod 9
      // occur the same number of times except one.  The oddball either is
      // missing an occurrence, or gets an extra occurrence.
      //
      // What I do below is get a 9 bit number (0..511).  Each of the possible
      // outcomes 0..8 occurs 57 times, except for 0, which occurs 56 times.
      //
      // I do that as follows:
      //
      //    if (r <= 56)
      //        output 8
      //    else
      //        output r mod 8
      //
      // This allocates 0..56 to the value 8, and spreads the remaining values
      // evenly amoung 0..7.  Because 56 mod 8 == 0, that means 0 gets short
      // shrift.
      //
      // The value 56 equals 0b000111000 in binary.  So, the code below
      // implements an optimized bitwise comparison for r <= 56.

      // In the code below, if you see a sequence in brackets such as
      // [b02, b01, b00], that's meant to logically concatenate individual bits
      // into a single quantity, from MSB to LSB.

      // Check for <= 56 (0b000111000).
      // Input is [b08, b07, b06, b05, b04, b03, b02, b01, b00].
      const uint64_t b09 = ~(b00 | b01 | b02);
      const uint64_t b10 = b03 & b04 & b05;
      const uint64_t b11 = ~(b06 | b07 | b08);

      // The logic expression for b12 has been minimized, so it's perhaps a bit
      // hard to follow.  Read it from right to left.
      //
      //    If the upper 3 bits are 0, and
      //      the middle 3 bits are 1 and the bottom 3 bits are 0, or
      //      the middle 3 bits are not all 1
      //    then the number is less than or equal to 56.
      //
      // When b12 is 1, then the output is 1000 exactly, by forcing the other
      // three bits to 0.  Otherwise, we just take bits 0, 1, and 2 from the
      // random input.
      //
      // The output a is in the range 0..8.  Below, I set a = 9 if a == 0, so
      // that a is in the range 1..9.
      //
      // Output: a = [b12, b13, b14, b15].
      const uint64_t b12 = ((b09 & b10) | ~b10) & b11;
      const uint64_t b13 = b02 & ~b12;
      const uint64_t b14 = b01 & ~b12;
      const uint64_t b15 = b00 & ~b12;

      // Second random 9-bit value.  I do the same thing with this one as I do
      // above, except that I don't map the value 0 to the value 9.
      const uint64_t b16 = *lf++;
      const uint64_t b17 = *lf++;
      const uint64_t b18 = *lf++;
      const uint64_t b19 = *lf++;
      const uint64_t b20 = *lf++;
      const uint64_t b21 = *lf++;
      const uint64_t b22 = *lf++;
      const uint64_t b23 = *lf++;
      const uint64_t b24 = *lf++;

      // Check for <= 56 (0b000111000).
      // Input is [b24, b23, b22, b21, b20, b19, b18, b17, b16].
      const uint64_t b25 = ~(b16 | b17 | b18);
      const uint64_t b26 = b19 & b20 & b21;
      const uint64_t b27 = ~(b22 | b23 | b24);

      // This is the same computation as above to extract 0..8.
      // Output: b = [b28, b29, b30, b31].
      const uint64_t b28 = ((b25 & b26) | ~b26) & b27;
      const uint64_t b29 = b18 & ~b28;
      const uint64_t b30 = b17 & ~b28;
      const uint64_t b31 = b16 & ~b28;

      // This extracts a 3-bit random value 0..7.
      // Output: c = [b32, b33, b34].
      const uint64_t b32 = *lf++;
      const uint64_t b33 = *lf++;
      const uint64_t b34 = *lf++;

      // This program disallows 0 as a leading digit for the 3-digit result.
      // To minimize gate count, I merely map 0 to 9, rather than adding 1 to
      // the random number.  It seemed simpler.  If all four bits are 0, force
      // the output to 0b1001.
      //
      // In C:  if (a == 0) a = 9;
      //
      // Output: a' = [b36, b13, b14, b37]
      const uint64_t b35 = ~(b12 | b13 | b14 | b15);
      const uint64_t b36 = b12 | b35;
      const uint64_t b37 = b15 | b35;

      // For 'b', add 1 if it's greater or equal to 'a'.  b38 is just a
      // classical lexicographical comparison, just performed bitwise:
      //   (a0 > b0) or ((a0 == b0) and ((a1 > b1) or ...))
      //
      // Output: b' = [b42, b41, b40, b39].
      const uint64_t b38 = b28 & ~b36 | (b28 ^ ~b36) &
                          (b29 & ~b13 | (b29 ^ ~b13) &
                          (b30 & ~b14 | (b30 ^ ~b14) &
                          (b31 & ~b37 | (b31 ^ ~b37))));
      const uint64_t b39 = b31 ^ b38;
      const uint64_t b40 = b30 ^ (b31 & b38);
      const uint64_t b41 = b29 ^ (b30 & ~b40);
      const uint64_t b42 = b28 ^ (b29 & ~b41);

      // For c, add 1 if it's greater or equal to pre-increment b, not the
      // post-increment b'.  We compare against b before it was incremented,
      // as that avoids a range of ambiguities.  Work a few examples on paper.
      //
      // This performs the same sort of lexicographic compare as before,
      // adjusting for the fact that c is only a 3-bit value currently that's
      // about to become 4-bit.
      //
      // Output: c' = [b52, b51, b50, b49].
      const uint64_t b48 =                     ~b28  &
                          (b32 & ~b29 | (b32 ^ ~b29) &
                          (b33 & ~b30 | (b33 ^ ~b30) &
                          (b34 & ~b31 | (b34 ^ ~b31))));
      const uint64_t b49 = b34 ^ b48;
      const uint64_t b50 = b33 ^ (b34 & b48);
      const uint64_t b51 = b32 ^ (b33 & ~b50);
      const uint64_t b52 = (b32 & ~b51);

      // For c', add 1 if it's greater or equal to a'.
      // Output: c'' = [b57, b56, b55, b54].
      const uint64_t b53 = b52 & ~b36 | (b52 ^ ~b36) &
                          (b51 & ~b13 | (b51 ^ ~b13) &
                          (b50 & ~b14 | (b50 ^ ~b14) &
                          (b49 & ~b37 | (b49 ^ ~b37))));
      const uint64_t b54 = b49 ^ b53;
      const uint64_t b55 = b50 ^ (b49 & b53);
      const uint64_t b56 = b51 ^ (b50 & ~b55);
      const uint64_t b57 = b52 ^ (b51 & ~b56);

      // A note on the diagrams below:  I've written the input bit numbers for
      // each of these operations vertically over two rows.  If you see __,
      // that means a zero is present there.  A __ on the output means there's
      // an implied 0 in the output.

      // w = 10 * a'.
      // Output: w = [b71, b70, b69, b67, b65, b64, b39].
      //
      //          3 1 1 3 _
      //          6 3 4 7 _
      //      3 1 1 3 _   
      //    + 6 3 4 7 _   
      //    ---------------
      //      6 6 6 5 1 3 _
      //      3 2 0 8 4 7 _
      const uint64_t b58 = b13 ^ b37;
      const uint64_t b59 = b13 & b37;
      const uint64_t b60 = b14 ^ b36 ^ b59;
      const uint64_t b61 = (b14 & b36) | (b14 & b59) | (b36 & b59);
      const uint64_t b62 = b13 ^ b61;
      const uint64_t b63 = b36 ^ (b13 & b61);

      // x = w + b'.
      // Output: x = [b71, b70, b69, b67, b65, b64, b39].
      //
      //     6 6 6 5 1 3 _
      //     3 2 0 8 4 7 _
      //           4 4 4 3
      //   +       2 1 0 9
      //   ---------------
      //     7 7 6 6 6 6 3
      //     1 0 9 7 5 4 9
      const uint64_t b64 = b37 ^ b40;
      const uint64_t b6_ = b37 & b40;
      const uint64_t b65 = b14 ^ b41 ^ b6_;
      const uint64_t b66 = (b14 & b41) | (b14 & b6_) | (b41 & b6_);
      const uint64_t b67 = b42 ^ b58 ^ b66;
      const uint64_t b68 = (b42 & b58) | (b42 & b66) | (b58 & b66);
      const uint64_t b69 = b60 ^ b68;
      const uint64_t b70 = b62 ^ (b60 & ~b69);
      const uint64_t b71 = b63 ^ (b62 & ~b70);

      // y = 10 * x.
      // Output: y = [b83, b82, b80, b78, b76, b74, b72, b64, b39, __].
      //
      //       7 7 6 6 6 6 3 _
      //       1 0 9 7 5 4 9 _
      //   7 7 6 6 6 6 3 _   
      // + 1 0 9 7 5 4 9 _   
      // ---------------------
      //   8 8 8 7 7 7 7 6 3 _
      //   3 2 0 8 6 4 2 4 9 _
      const uint64_t b72 = b39 ^ b65;
      const uint64_t b73 = b39 & b65;
      const uint64_t b74 = b64 ^ b67 ^ b73;
      const uint64_t b75 = (b64 & b67) | (b64 & b73) | (b67 & b73);
      const uint64_t b76 = b65 ^ b69 ^ b75;
      const uint64_t b77 = (b65 & b69) | (b65 & b75) | (b69 & b75);
      const uint64_t b78 = b67 ^ b70 ^ b77;
      const uint64_t b79 = (b67 & b70) | (b67 & b77) | (b70 & b77);
      const uint64_t b80 = b69 ^ b71 ^ b79;
      const uint64_t b81 = (b69 & b71) | (b69 & b79) | (b71 & b79);
      const uint64_t b82 = b70 ^ b81;
      const uint64_t b83 = b71 ^ (b70 & b81);

      // z = y + c''.
      // Output: z = [b95, b94, b93, b92, b91, b90, b88, b86, b84, b54].
      //
      //   8 8 8 7 7 7 7 6 3 _
      //   3 2 0 8 6 4 2 4 9 _
      //               5 5 5 5
      // +             7 6 5 4
      // ---------------------
      //   9 9 9 9 9 9 8 8 8 5
      //   5 4 3 2 1 0 8 6 4 4
      const uint64_t b84 = b39 ^ b55;
      const uint64_t b85 = b39 & b55;
      const uint64_t b86 = b56 ^ b64 ^ b85;
      const uint64_t b87 = (b56 & b64) | (b56 & b85) | (b64 & b85);
      const uint64_t b88 = b57 ^ b72 ^ b87;
      const uint64_t b89 = (b57 & b72) | (b57 & b87) | (b72 & b87);
      const uint64_t b90 = b74 ^ b89;
      const uint64_t b91 = b76 ^ (b74 & b89);
      const uint64_t b92 = b78 ^ (b76 & ~b91);
      const uint64_t b93 = b80 ^ (b78 & ~b92);
      const uint64_t b94 = b82 ^ (b80 & ~b93);
      const uint64_t b95 = b83 ^ (b82 & ~b94);
      const uint64_t b96 = 0;

      // Now the fun begins.  We have all 64 results packed into a set of 10
      // 64-bit variables.  We need to _transpose_ this matrix of bits, at
      // least at the 16-bit boundary level.
      //
      // Imagine an extra 6 rows of zeros to pad our 10-bit values to 16 bits.
      // We need to transpose the four 16 x 16 bit matrices inside this 16 x 64
      // bits.
      //
      // I just decomposed the transpose into a hierarchy:
      //  -- Transpose 2x2 1-bit values.
      //  -- Transpose 2x2 blocks of 2x2 bit matrices.
      //  -- Transpose 2x2 blocks of 4x4 bit matrices.
      //  -- Transpose 2x2 blocks of 8x8 bit matrices.
      //
      // I use b96 for all virtual zero rows where needed as an input.  I
      // figure the compiler can optimize that back out.  It's worth the visual
      // symmetry to leave it in my source code.

      // ... transpose and output.
      // 2x2 transpose on pairs of outputs.
      //    dcba     CcAa
      //    DCBA ... DdBb
      //    zyxw     YyWw
      //    ZYXW     ZzXx
      const uint64_t t00 = (b84 & m0p) << 1 | b54 & m0p;
      const uint64_t t01 = (b54 & m0n) >> 1 | b84 & m0n;
      const uint64_t t02 = (b88 & m0p) << 1 | b86 & m0p;
      const uint64_t t03 = (b86 & m0n) >> 1 | b88 & m0n;
      const uint64_t t04 = (b91 & m0p) << 1 | b90 & m0p;
      const uint64_t t05 = (b90 & m0n) >> 1 | b91 & m0n;
      const uint64_t t06 = (b93 & m0p) << 1 | b92 & m0p;
      const uint64_t t07 = (b92 & m0n) >> 1 | b93 & m0n;
      const uint64_t t08 = (b95 & m0p) << 1 | b94 & m0p;
      const uint64_t t09 = (b94 & m0n) >> 1 | b95 & m0n;

      // 4x4 transpose on pairs of transposed 2x2s.
      //    CcAa     WwAa
      //    DdBb ... XxBb
      //    YyWw     YyCc
      //    ZzXx     ZzDd
      const uint64_t t10 = (t02 & m1p) << 2 | t00 & m1p;
      const uint64_t t11 = (t03 & m1p) << 2 | t01 & m1p;
      const uint64_t t12 = (t00 & m1n) >> 2 | t02 & m1n;
      const uint64_t t13 = (t01 & m1n) >> 2 | t03 & m1n;
      const uint64_t t14 = (t06 & m1p) << 2 | t04 & m1p;
      const uint64_t t15 = (t07 & m1p) << 2 | t05 & m1p;
      const uint64_t t16 = (t04 & m1n) >> 2 | t06 & m1n;
      const uint64_t t17 = (t05 & m1n) >> 2 | t07 & m1n;
      const uint64_t t18 = (b96 & m1p) << 2 | t08 & m1p;
      const uint64_t t19 = (b96 & m1p) << 2 | t09 & m1p;
      const uint64_t t1a = (t08 & m1n) >> 2 | b96 & m1n;
      const uint64_t t1b = (t09 & m1n) >> 2 | b96 & m1n;

      // 8x8 transpose on pairs of transposed 4x4s.
      const uint64_t t20 = (t14 & m2p) << 4 | t10 & m2p;
      const uint64_t t21 = (t15 & m2p) << 4 | t11 & m2p;
      const uint64_t t22 = (t16 & m2p) << 4 | t12 & m2p;
      const uint64_t t23 = (t17 & m2p) << 4 | t13 & m2p;
      const uint64_t t24 = (t10 & m2n) >> 4 | t14 & m2n;
      const uint64_t t25 = (t11 & m2n) >> 4 | t15 & m2n;
      const uint64_t t26 = (t12 & m2n) >> 4 | t16 & m2n;
      const uint64_t t27 = (t13 & m2n) >> 4 | t17 & m2n;
      const uint64_t t28 = (b96 & m2p) << 4 | t18 & m2p;
      const uint64_t t29 = (b96 & m2p) << 4 | t19 & m2p;
      const uint64_t t2a = (b96 & m2p) << 4 | t1a & m2p;
      const uint64_t t2b = (b96 & m2p) << 4 | t1b & m2p;
      const uint64_t t2c = (t18 & m2n) >> 4 | b96 & m2n;
      const uint64_t t2d = (t19 & m2n) >> 4 | b96 & m2n;
      const uint64_t t2e = (t1a & m2n) >> 4 | b96 & m2n;
      const uint64_t t2f = (t1b & m2n) >> 4 | b96 & m2n;

      // 16x16 transpose on pairs of transposed 8x8s.
      const uint64_t t30 = (t28 & m3p) << 8 | t20 & m3p;
      const uint64_t t31 = (t29 & m3p) << 8 | t21 & m3p;
      const uint64_t t32 = (t2a & m3p) << 8 | t22 & m3p;
      const uint64_t t33 = (t2b & m3p) << 8 | t23 & m3p;
      const uint64_t t34 = (t2c & m3p) << 8 | t24 & m3p;
      const uint64_t t35 = (t2d & m3p) << 8 | t25 & m3p;
      const uint64_t t36 = (t2e & m3p) << 8 | t26 & m3p;
      const uint64_t t37 = (t2f & m3p) << 8 | t27 & m3p;
      const uint64_t t38 = (t20 & m3n) >> 8 | t28 & m3n;
      const uint64_t t39 = (t21 & m3n) >> 8 | t29 & m3n;
      const uint64_t t3a = (t22 & m3n) >> 8 | t2a & m3n;
      const uint64_t t3b = (t23 & m3n) >> 8 | t2b & m3n;
      const uint64_t t3c = (t24 & m3n) >> 8 | t2c & m3n;
      const uint64_t t3d = (t25 & m3n) >> 8 | t2d & m3n;
      const uint64_t t3e = (t26 & m3n) >> 8 | t2e & m3n;
      const uint64_t t3f = (t27 & m3n) >> 8 | t2f & m3n;

      // Now each variable above has 4 x 16-bit values.  Just spool these out
      // to memory directly without bothering to unpack them.
      *o64++ = t30;
      *o64++ = t31;
      *o64++ = t32;
      *o64++ = t33;
      *o64++ = t34;
      *o64++ = t35;
      *o64++ = t36;
      *o64++ = t37;
      *o64++ = t38;
      *o64++ = t39;
      *o64++ = t3a;
      *o64++ = t3b;
      *o64++ = t3c;
      *o64++ = t3d;
      *o64++ = t3e;
      *o64++ = t3f;
    }
  }
}

static double get_time(void)
{
    struct timespec now;
    double seconds;

    clock_gettime(CLOCK_MONOTONIC, &now);

    seconds = (double)now.tv_sec + (double)now.tv_nsec * 1e-9;
    return seconds;
}

// 10 billion.
#define O_COUNT (100000)
#define I_COUNT (100000)

static uint16_t buf[ROUND_UP(I_COUNT, 64)] __attribute__((aligned(128)));

static volatile uint64_t f = 0;

int main() {
  srand_lfib(2 /*time(0)*/);

  double elapsed = 0.0;

  for (int j = 0; j < O_COUNT; ++j) {
    double start = get_time();
    gen_rand_3dig(buf, I_COUNT);
    double end = get_time();
    elapsed += end - start;
    f += buf[(buf[0] * 0xAAAA5555ull) % I_COUNT];
    /*
    for (int i = 0; i < I_COUNT; ++i) {
      printf(" %03d", buf[i]);
      if (i % 20 == 19) {
        putchar('\n');
      }
    }
    */
  }

/*
  if (COUNT % 20) {
    putchar('\n');
  }
*/
  printf("%llX\n", f);
  printf("%f seconds\n", elapsed);
}
