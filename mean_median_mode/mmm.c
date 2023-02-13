// Finds mean, median, mode of an array of integers.
#include <stdlib.h>

typedef const void cv;
typedef const int ci;
typedef double d;
typedef struct { d m[3] ;} mmm;

int l(cv*a,cv*b) {; return
(*(ci*)a<*(ci*)b)-(*(ci*)a>*(ci*)b)

;}

mmm hmmm(int*const a, ci ml) {;
  mmm m={}; if (ml < 1) return m;
  if (ml ==!! ml) return (mmm)

  {{*a,*a,*a}};

  qsort(a, ml, sizeof(*a), l); for

  (d mi = !ml, ma = *a, mb = !mi,
  mc = mb, md, mn = a[ml/2]/2.+
  a[~-ml/2]/2., mg = ma, mm = mg ;(
  md =* ((ci)++(mi)+a), mi )< ml

  ;)

  mc += md - ma ? 1 - mc : 1,
  mb = mb <= mc ? mc : mb,
  mm = mb <= mc ? md : mm,
  mg += ma = md,
  m = (mmm){{mg/ml,mn,mm}};
  return m

;}


// Example test program:
#include <stdio.h>

int main() {
    int a[] = {
        8, 6, 7, 5, 3, 0, 9,
        7, 7, 3, 3, 2, 2, 2
    };

    mmm m = hmmm(a, sizeof(a)/sizeof(*a));

    printf("%f %f %f\n",
           m.m[0], m.m[1], m.m[2]);
}
