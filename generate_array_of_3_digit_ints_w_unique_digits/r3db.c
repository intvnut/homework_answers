#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static uint64_t lfib[128];

// Initializes the state for the lagged Fibonacci random number generate.
// This uses a 64-bit LFSR with a 64-bit seed.  It's not exactly awesome,
// but it's adequate.
static void srand_lfib(uint64_t seed) {
  const uint64_t poly = 0xD6C9E91ACA649AD4;

  if (!seed) {
    seed = 1;
  }

  for (int i = 0; i < 128; ++i) {
    // Advance the LFSR by the full width of the seed.
    for (int j = 0; j < 64; ++j) {
      seed = (seed >> 1) ^ (seed & 1 ? poly : 0);
    }

    lfib[i] = seed;
  }
}

// Generates another full block of data in lfib[].  This is a lagged Fibonnaci
// random number generator, with lags 127 and 30.  These come from Table 1 in
// TAOCP Vol 2, section 3.2.2.
//
// The period of this generator should be 2^63 * (2^127 - 1), but that applies
// to the 64-bit outputs *as a whole*.  Bit number n has period
// 2^(n-1) * (2^127 - 1).  To get a higher quality result, whatever consumes
// this output should rotate the i^th output by i % 64, or apply some other
// transformation to mix all the bits together.
//
// But for this exercise, this is way more than adequate.
static void update_lfib_block(void) {
  // lags of 30 and 127.
  for (int i = 0; i < 31; ++i) {
    lfib[i] = lfib[i + 1] + lfib[i + 127 - 30];
  }

  for (int i = 31; i < 127; ++i) {
    lfib[i] = lfib[i + 1] + lfib[i - 31];
  }

  lfib[127] = lfib[0] + lfib[127 - 30];
}

// The mKp constants are what Knuth calls mu_k, where each bit pattern is 1
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
// output as a series of boolean computations on individual bits of intermediate
// values and the final result.  That's right, it's a bit-serial implementation
// of the algorithm.
//
// Or, if you prefer, it's how you would implement it with 1-input and 2-input
// logic gates.  So, you will see me refer to "gate counts" below.  Gate counts
// correspond to instruction counts.
//
// Where it gains its speed is from applying that bit-serial algorithm to all of
// the bits in a machine word in parallel.  With a 64-bit machine word, we can
// calculate 64 results as fast as we can compute a single result.
void gen_rand_3dig(uint16_t *out, int count) {
  // Since I don't assume the output buffer is a multiple of 64 elements, I'll
  // just compute into a private buffer and copy the results into the output
  // buffer at the end, trimming as necessary when I over-compute.
  uint16_t buf[64];

  // This variable keeps track of how many elements we've withdrawn from the
  // lfib[] buffer.  When it gets above 100, we'll re-generate the buffer.  I
  // meant to put 101 here, not 100, to force a regeneration up front.
  // Honestly, I should have just set it to 0 and manually called the
  // update_lfib_block().  Chalk it up to not caring too much about production
  // quality code.
  int l = 100;

  for (int i = 0; i < count; i += 64) {
    // If we've consumed "enough" of the buffer, cycle it and start over.  Note
    // that Knuth actually recommends dropping blocks of samples from a lagged
    // Fibonacci generator with a block size that is coprime to the longer lag.
    // Otherwise, lagged Fibonacci generators tend to fail tests such as the
    // spectral test, if I recall correctly.  (See TAOCP Vol 2 for more info on
    // how to test PRNGs for apparent randomness.)
    if (l > 100) {
      update_lfib_block();
      l = 0;
    }

    // Extract a random 9-bit value.  That literally just requires pulling 9
    // values from the lfib[] array.  Recall my comment above about the period
    // of each bit, and how bits further to the left have longer periods.  This
    // is where rotating each value by an increasing amount would spread that
    // around all the bit lanes.
    const uint64_t b00 = lfib[l++];
    const uint64_t b01 = lfib[l++];
    const uint64_t b02 = lfib[l++];
    const uint64_t b03 = lfib[l++];
    const uint64_t b04 = lfib[l++];
    const uint64_t b05 = lfib[l++];
    const uint64_t b06 = lfib[l++];
    const uint64_t b07 = lfib[l++];
    const uint64_t b08 = lfib[l++];

    // A quick aside at how I generate a random number in the range 0..8 from a
    // 9-bit value with minimal bias and minimal gate count.
    //
    // It turns out values of the form 2^(3k) mod 9 are +/-1, for k > 0.  Why is
    // that important?  That means that 2^(3k) is very nearly a multiple of 9.
    // Thus, over the range [0, 2^(3k) - 1], all of the values mod 9 occur the
    // same number of times except one.  The oddball either is missing an
    // occurrence, or gets an extra occurrence.
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
    // The value 56 equals 0b000111000 in binary.  So, the code below implements
    // an optimized bitwise comparison for r <= 56.

    // In the code below, if you see a sequence in brackets such as [b02, b01,
    // b00], that's just concatenating individual bits into a single quantity,
    // from MSB to LSB.

    // Check for <= 56 (0b000111000).  Input is [b08, b07, b06, b05, b04, b03,
    // b00, b01, b02].
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
    // random input.  (And, I just noticed, I don't bother to preserve their
    // order, because it doesn't really matter here.  Oops, I flipped their
    // order.)
    // 
    // The output from this is [b12, b13, b14, b15], and it's in the range 0..8.
    const uint64_t b12 = ((b09 & b10) | ~b10) & b11;
    const uint64_t b13 = b00 & ~b12;
    const uint64_t b14 = b01 & ~b12;
    const uint64_t b15 = b02 & ~b12;

    // Second random 9-bit value.  I do the same thing with this one as I do
    // above, except that I don't map 0 to 8.
    const uint64_t b16 = lfib[l++];
    const uint64_t b17 = lfib[l++];
    const uint64_t b18 = lfib[l++];
    const uint64_t b19 = lfib[l++];
    const uint64_t b20 = lfib[l++];
    const uint64_t b21 = lfib[l++];
    const uint64_t b22 = lfib[l++];
    const uint64_t b23 = lfib[l++];
    const uint64_t b24 = lfib[l++];

    // Check for <= 56 (0b000111000).
    // Input is [b24, b23, b22, b21, b20, b19, b16, b17, b18].
    const uint64_t b25 = ~(b16 | b17 | b18);
    const uint64_t b26 = b19 & b20 & b21;
    const uint64_t b27 = ~(b22 | b23 | b24);

    // This is the same computation as above to extract 0..8.
    // The output is in [b28, b29, b30, b31].
    const uint64_t b28 = ((b25 & b26) | ~b26) & b27;
    const uint64_t b29 = b16 & ~b28;
    const uint64_t b30 = b17 & ~b28;
    const uint64_t b31 = b18 & ~b28;

    // This extracts a 3-bit random value 0..7 into [b32, b33, b34].
    const uint64_t b32 = lfib[l++];
    const uint64_t b33 = lfib[l++];
    const uint64_t b34 = lfib[l++];

    // This program disallows 0 as a leading digit for the 3-digit result.  To
    // minimize gate count, I merely map 0 to 9, rather than adding 1 to the
    // random number.  It seemed simpler.  If all four bits are 0, force the
    // output to 0b1001.

    // For 'a', map 0 to 9, leaving others unchanged.
    // Output in [b36, b13, b14, b37]
    const uint64_t b35 = ~(b12 | b13 | b14 | b15);
    const uint64_t b36 = b12 | b35;
    const uint64_t b37 = b15 | b35;

    // For 'b', add 1 if it's greater or equal to 'a'.  b38 is just a classical
    // lexicographical comparison, just performed bitwise:
    //
    //     (a0 > b0) or ((a0 == b0) and ((a1 > b1) or ...))
    //
    // Output in [b42, b41, b40, b39].
    const uint64_t b38 = b28 & ~b36 | (b28 ^ ~b36) &
                        (b29 & ~b13 | (b29 ^ ~b13) &
                        (b30 & ~b14 | (b30 ^ ~b14) &
                        (b31 & ~b37 | (b31 ^ ~b37))));
    const uint64_t b39 = b31 ^ b38;
    const uint64_t b40 = b30 ^ (b31 & b38);
    const uint64_t b41 = b29 ^ (b30 & ~b40);
    const uint64_t b42 = b28 ^ (b29 & ~b41);

    // For 'c', add 1 if it's greater or equal to 'b' *before* 'b' was
    // incremented.  We compare against 'b' before it was incremented, as that
    // avoids a range of ambiguities.  Work a few examples on paper.
    //
    // This performs the same sort of lexicographic compare as before, adjusting
    // for the fact that 'c' is only a 3-bit value currently that's about to
    // become 4-bit.
    //
    // Output in [b52, b51, b50, b49].
    const uint64_t b48 =                     ~b28  &
                        (b32 & ~b29 | (b32 ^ ~b29) &
                        (b33 & ~b30 | (b33 ^ ~b30) &
                        (b34 & ~b31 | (b34 ^ ~b31))));
    const uint64_t b49 = b34 ^ b48;
    const uint64_t b50 = b33 ^ (b34 & b48);
    const uint64_t b51 = b32 ^ (b33 & ~b50);
    const uint64_t b52 = (b32 & ~b51);

    // For 'c', add 1 if it's greater or equal to 'a'.
    // Output in [b57, b56, b55, b54].
    const uint64_t b53 = b52 & ~b36 | (b52 ^ ~b36) &
                        (b51 & ~b13 | (b51 ^ ~b13) &
                        (b50 & ~b14 | (b50 ^ ~b14) &
                        (b49 & ~b37 | (b49 ^ ~b37))));
    const uint64_t b54 = b49 ^ b53;
    const uint64_t b55 = b50 ^ (b49 & b53);
    const uint64_t b56 = b51 ^ (b50 & ~b55);
    const uint64_t b57 = b52 ^ (b51 & ~b56);

    // A note on the diagrams below:  I've written the input bit numbers for
    // each of these operations vertically over two rows.  If you see __, that
    // means a zero is present there.
    //
    // A __ on the output means there's an implied 0 in the output.

    // w = 10 * a.  Output in [b63, b62, b60, b58, b14, b37, __]
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

    // x = w + b.  Output in [b71, b70, b69, b67, b65, b64, b39].
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

    // y = 10 * x.  Output in [b83, b82, b80, b78, b76, b74, b72, b64, b39, __].
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

    // z = y + c.  Output in [b95, b94, b93, b92, b91, b90, b88, b86, b84, b54].
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
    // 64-bit variables. We need to _transpose_ this matrix of bits, at least
    // at the 16-bit boundary level.  Imagine an extra 6 rows of zeros to pad
    // our 10-bit values to 16 bits.  We need to transpose the four 16 x 16 bit
    // matrices inside this 16 x 64 bits.
    //
    // I just decomposed the transpose into a hierarchy:
    //  -- Transpose 2x2 1-bit values.
    //  -- Transpose 2x2 blocks of 2x2 matrices.
    //  -- Transpose 2x2 blocks of 4x4 matrices.
    //  -- Transpose 2x2 blocks of 8x8 matrices.
    //
    // I use b96 for all virtual zero rows where needed as an input.  I figure
    // the compiler can optimize that back out.  It's worth the visual symmetry
    // to leave it in my source code.


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

    // Now after all that, each 64-bit value has four 16-bit results packed into
    // it.  Unpack them and spool them to the local output buffer.
    buf[000] = t30 >>  0;
    buf[001] = t30 >> 16;
    buf[002] = t30 >> 32;
    buf[003] = t30 >> 48;
    buf[004] = t31 >>  0;
    buf[005] = t31 >> 16;
    buf[006] = t31 >> 32;
    buf[007] = t31 >> 48;
    buf[010] = t32 >>  0;
    buf[011] = t32 >> 16;
    buf[012] = t32 >> 32;
    buf[013] = t32 >> 48;
    buf[014] = t33 >>  0;
    buf[015] = t33 >> 16;
    buf[016] = t33 >> 32;
    buf[017] = t33 >> 48;
    buf[020] = t34 >>  0;
    buf[021] = t34 >> 16;
    buf[022] = t34 >> 32;
    buf[023] = t34 >> 48;
    buf[024] = t35 >>  0;
    buf[025] = t35 >> 16;
    buf[026] = t35 >> 32;
    buf[027] = t35 >> 48;
    buf[030] = t36 >>  0;
    buf[031] = t36 >> 16;
    buf[032] = t36 >> 32;
    buf[033] = t36 >> 48;
    buf[034] = t37 >>  0;
    buf[035] = t37 >> 16;
    buf[036] = t37 >> 32;
    buf[037] = t37 >> 48;
    buf[040] = t38 >>  0;
    buf[041] = t38 >> 16;
    buf[042] = t38 >> 32;
    buf[043] = t38 >> 48;
    buf[044] = t39 >>  0;
    buf[045] = t39 >> 16;
    buf[046] = t39 >> 32;
    buf[047] = t39 >> 48;
    buf[050] = t3a >>  0;
    buf[051] = t3a >> 16;
    buf[052] = t3a >> 32;
    buf[053] = t3a >> 48;
    buf[054] = t3b >>  0;
    buf[055] = t3b >> 16;
    buf[056] = t3b >> 32;
    buf[057] = t3b >> 48;
    buf[060] = t3c >>  0;
    buf[061] = t3c >> 16;
    buf[062] = t3c >> 32;
    buf[063] = t3c >> 48;
    buf[064] = t3d >>  0;
    buf[065] = t3d >> 16;
    buf[066] = t3d >> 32;
    buf[067] = t3d >> 48;
    buf[070] = t3e >>  0;
    buf[071] = t3e >> 16;
    buf[072] = t3e >> 32;
    buf[073] = t3e >> 48;
    buf[074] = t3f >>  0;
    buf[075] = t3f >> 16;
    buf[076] = t3f >> 32;
    buf[077] = t3f >> 48;

    // Now copy them to the final output buffer.  I do this step separately as
    // the final output buffer may not be a multiple of 64 elements.
    // Realistically, if you cared about maximum performance, you'd mandate that
    // the output buffer was aligned a specific way and was a multiple of
    // whatever size you needed it to be a multiple of, so that we could avoid
    // any sort of conditional code or extra copies in the main money loop.
    //
    // And, we'd probably ask for it to be aligned on a boundary that let us
    // write out the uint64_t values directly without the shifts to unpack it.
    for (int j = 0; j < 64 && j + i < count; ++j)
      out[i + j] = buf[j];
  }
}

#define COUNT (1000000)

uint16_t buf[COUNT];

volatile uint64_t f = 0;

int main() {
  srand_lfib(2 /*time(0)*/);

  gen_rand_3dig(buf, COUNT);

  for (int i = 0; i < COUNT; ++i) {
    f += buf[i];
    printf(" %03d", buf[i]);
    if (i % 20 == 19) {
      putchar('\n');
    }
  }

  if (COUNT % 20) {
    putchar('\n');
  }

  //printf("%llX\n", f);
}
