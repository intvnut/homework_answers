#include <stdio.h>
#include <math.h>

static long double sum_floats_body(const float lo,
                                   const float hi);

long double sum_floats(const float a, const float b) {
  float lo = fminf(a, b);
  float hi = fmaxf(a, b);

  // If both are exactly equal, return lo, as our range has
  // precisely one element because lo and hi are coincident.
  if (lo == hi) return lo;

  // If either argument is NaN, return NaN.
  if (isnan(lo) || isnan(hi)) return NAN;

  // If both arguments are Infinities, return NaN.  Why NaN?
  // If they had had the same sign, we would have already
  // returned Infinity above.  They have different signs, and
  // Infinity - Infinity == NaN.
  if (isinf(lo) && isinf(hi)) return NAN;

  // If only one argument is an Infinity, return the
  // appropriately signed Infinity.
  if (isinf(lo)) return lo;
  if (isinf(hi)) return hi;

  // Eliminate 0 from the interval by moving one ulp toward
  // the other end of the interval. 0 doesn't contribute to
  // the sum, which makes it safe to remove.  It is also
  // undefined for frexpf.  (Similarly, log(0) is undefined.)
  if (lo == 0.f) lo = nexttowardf(0.f, hi);
  if (hi == 0.f) hi = nexttowardf(0.f, lo);

  // If the signs differ, we can take advantage of the fact
  // that many of the terms may cancel, and only compute the
  // portion of the interval that has surviving terms.
  if (lo < 0.f && hi > 0.f)
    return -lo == hi ? 0.L  // Exact cancellation!
         : -lo >  hi ? -sum_floats_body( hi, -lo)
         :              sum_floats_body(-lo,  hi);

  // If both are -ve, flip to +ve, compute, and negate the result.
  if (hi < 0.f)
    return -sum_floats_body(-hi, -lo);

  // Otherwise, just sum up and return.
  return sum_floats_body(lo, hi);
}

#include <stdint.h>

// Compute the integer sum for [lo, hi] using Gauss' formula.
static uint64_t sum_int_range(const uint64_t lo,
                              const uint64_t hi) {
  return (hi + lo) * (hi - lo + 1) / 2;
}

// Some useful constants.
#define FL_MIN_FRAC_NORM  (1ull << 23)
#define FL_MAX_FRAC_NORM  ((1ull << 24) - 1)
#define FL_MIN_FRAC_SUBN  (1)
#define FL_MAX_FRAC_SUBN  ((1ull << 23) - 1)
#define FL_FRAC_MULT      (1ull << 24)

// "Integerized" floating point number.
typedef struct {
  long     exp;   // Exponent
  uint64_t frac;  // Fraction
} ifloat;

static ifloat float_to_ifloat(const float f) {
  int exp = 0;
  float ff = frexpf(f, &exp);

  // Apply 2^24 bias to frac, and remove that bias from exp.
  ifloat ifl = { exp - 24, FL_FRAC_MULT * ff };

  // Special case:  If f was subnormal, then we over-biased
  // ifl.frac.  Remove that extra bias.  Furthermore, our
  // computation of ifl.exp was incorrect because frexpf()
  // normalized the fraction.
  if (ifl.exp < -149) {
    ifl.frac >>= -125 - exp;
    ifl.exp = -149;
  }

  return ifl;
}

static long double ifloat_to_long_double(ifloat ifl) {
  return scalblnl((long double)ifl.frac, ifl.exp);
}

static int ifloat_is_normal(ifloat ifl) {
  return ifl.exp > -149 || ifl.frac >= FL_MIN_FRAC_NORM;
}

static long double sum_floats_body(const float lo,
                                   const float hi) {
  const ifloat if_lo = float_to_ifloat(lo);
  const ifloat if_hi = float_to_ifloat(hi);

  // Straightforward case:  Both have the same exponent.
  if (if_lo.exp == if_hi.exp) {
    const ifloat rslt = {
        if_lo.exp,
        sum_int_range(if_lo.frac, if_hi.frac)
    };
    return ifloat_to_long_double(rslt);
  }

  // Trickier case:  Both have different exponents:
  // 1. Compute from lo up to the upper boundary of its exponent.
  // 2. Compute from hi down to the lower boundary of its exponent.
  // 3. Compute the mess for the exponents in-between.
  const ifloat lo_sum = {
    if_lo.exp,
    ifloat_is_normal(if_lo) ? sum_int_range(if_lo.frac, FL_MAX_FRAC_NORM)
                            : sum_int_range(if_lo.frac, FL_MAX_FRAC_SUBN)
  };
  const ifloat hi_sum = {
    if_hi.exp,
    ifloat_is_normal(if_hi) ? sum_int_range(FL_MIN_FRAC_NORM, if_hi.frac)
                            : sum_int_range(FL_MIN_FRAC_SUBN, if_hi.frac)
  };

  // Step 3 is interesting.  Each exponent range between if_lo.exp
  // and if_hi.exp will have the same sum, just with a different
  // exponent scaling.  We don't need to iterate through all of
  // these.  We can make use of this fact:
  //   2^(lo+1) + 2^(lo+2) + ... 2^(hi-1) == 2^hi - 2^(lo+1).
  // Note: Use 'lo' instead of 'lo+1' for subnormals, because we've
  // de-normalized them.
  const ifloat ml_sum = {
    ifloat_is_normal(if_lo) + if_lo.exp,
    sum_int_range(FL_MIN_FRAC_NORM, FL_MAX_FRAC_NORM)
  };
  const ifloat mh_sum = {
    if_hi.exp, ml_sum.frac
  };

  // Now let's compute the final sum, starting at the bottom and
  // working up.  Note that ld_mh_sum will likely be larger than
  // ld_hi_sum, so add it last.  Numeric optimization:  Omit
  // the mid-sums entirely if they're equal since they'll cancel.
  const long double ld_lo_sum = ifloat_to_long_double(lo_sum);
  const long double ld_hi_sum = ifloat_to_long_double(hi_sum);
  if (ml_sum.exp != mh_sum.exp) {
    const long double ld_ml_sum = ifloat_to_long_double(ml_sum);
    const long double ld_mh_sum = ifloat_to_long_double(mh_sum);

    return (((ld_lo_sum - ld_ml_sum) + ld_hi_sum) + ld_mh_sum);
  } else {
    return ld_lo_sum + ld_hi_sum;
  }
}

// Jerry Coffin's version:
#include <tgmath.h>
long double jerry_sum_floats(const float lower, const float upper) {
  long double total = 0.0;
  lower = nextafter(lower, upper);
  for (float f = nextafter(lower, upper); f<upper; f=nextafter(f, upper))
    total += f;

  return total;
}

int main() {
//printf("jz: %Lg\n", sum_floats(1.0f, 2.0f));
//printf("jc: %Lg\n", jerry_sum_floats(1.0f, 2.0f));

  float smallnorm = 1.1754943508e-38f;
  //float lo = smallnorm / 3.f, hi = smallnorm / 2.f;
  //float lo = -101.f, hi = 100.f;
  float lo = 1.0f, hi = 100.0f;
  long double jz = sum_floats(lo, hi);
  long double jc = jerry_sum_floats(lo, hi);
  printf("  jz: %Lg (%La)\n  jc: %Lg (%La)\ndiff: %Lg (%La)\n",
          jz, jz, jc, jc, jz - jc, jz - jc);
}
