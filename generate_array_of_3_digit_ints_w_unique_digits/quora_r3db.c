#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef const uint64_t cu64;

static uint64_t lfib[128];

void srand_lfib(uint64_t seed) {
  cu64 poly = 0xD6C9E91ACA649AD4;

  if (!seed) {
    seed = 1;
  }

  for (int i = 0; i < 128; ++i) {
    for (int j = 0; j < 64; ++j) {
      seed = (seed >> 1) ^ (seed & 1 ? poly : 0);
    }

    lfib[i] = seed;
  }
}

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

enum {
    m0p = 0x5555555555555555, m0n = ~m0p,
    m1p = 0x3333333333333333, m1n = ~m1p,
    m2p = 0x0F0F0F0F0F0F0F0F, m2n = ~m2p,
    m3p = 0x00FF00FF00FF00FF, m3n = ~m3p,
};

void gen_rand_3dig(uint16_t *out, int count) {
  uint16_t buf[64];
  int l = 100;

  for (int i = 0; i < count; i += 64) {
    if (l > 100) {
      update_lfib_block();
      l = 0;
    }

    cu64
    cd = lfib[l++], gb = lfib[l++], ka = lfib[l++],
    ra = lfib[l++], la = lfib[l++], pc = lfib[l++],
    fa = lfib[l++], dc = lfib[l++], oe = lfib[l++],
    bf = lfib[l++], te = lfib[l++], tc = lfib[l++],
    nc = lfib[l++], id = lfib[l++], kd = lfib[l++],
    zc = lfib[l++], le = lfib[l++], oc = lfib[l++],
    je = lfib[l++], gc = lfib[l++], pb = lfib[l++],
    kb = ~(cd | gb | ka), qb = ra & la & pc,
    ua = ~(fa | dc | oe), ic = ((kb & qb) | ~qb) & ua,
    ge = cd & ~ic, wa = gb & ~ic, ob = ka & ~ic,
    ha = ~(bf | te | tc), mb = nc & id & kd,
    oa = ~(zc | le | oc), wb = ((ha & mb) | ~mb) & oa,
    qa = bf & ~wb, ff = te & ~wb, ce = tc & ~wb,
    we = ~(ic | ge | wa | ob), se = ic | we, hc = ob | we,
    aa = wb & ~se | (wb ^ ~se) & (qa & ~ge | (qa ^ ~ge) &
         (ff & ~wa | (ff ^ ~wa) & (ce & ~hc | (ce ^ ~hc)))),
    zd = ce ^ aa, ia = ff ^ (ce & aa), xe = qa ^ (ff & ~ia),
    rb = wb ^ (qa & ~xe), ae = ~wb & (je & ~qa | (je ^ ~qa) &
         (gc & ~ff | (gc ^ ~ff) & (pb & ~ce | (pb ^ ~ce)))),
    ad = pb ^ ae, pa = gc ^ (pb & ae), ub = je ^ (gc & ~pa),
    ja = (je & ~ub), be = ja & ~se | (ja ^ ~se) & (ub & ~ge |
         (ub ^ ~ge) & (pa & ~wa | (pa ^ ~wa) & (ad & ~hc |
         (ad ^ ~hc)))), rc = ad ^ be, he = pa ^ (ad & be),
    cf = ub ^ (pa & ~he), jd = ja ^ (ub & ~cf), fc = ge ^ hc,
    qc = ge & hc, vc = wa ^ se ^ qc, gd = (wa & se) | (wa &
         qc) | (se & qc), va = ge ^ gd, uc = se ^ (ge & gd),
    tb = hc ^ ia, df = hc & ia, lb = wa ^ xe ^ df, ve = (wa &
         xe) | (wa & df) | (xe & df), ue = rb ^ fc ^ ve,
    kf = (rb & fc) | (rb & ve) | (fc & ve), lc = vc ^ kf,
    ed = va ^ (vc & ~lc), me = uc ^ (va & ~ed), gf = zd ^ lb,
    od = zd & lb, re = tb ^ ue ^ od, cb = (tb & ue) |
         (tb & od) | (ue & od), ef = lb ^ lc ^ cb,
    ze = (lb & lc) | (lb & cb) | (lc & cb), za = ue ^ ed ^ ze,
    vd = (ue & ed) | (ue & ze) | (ed & ze), wc = lc ^ me ^ vd,
    qe = (lc & me) | (lc & vd) | (me & vd), ie = ed ^ qe,
    af = me ^ (ed & qe), cc = zd ^ he, hf = zd & he,
    mc = cf ^ tb ^ hf, lf = (cf & tb) | (cf & hf) | (tb & hf),
    ne = jd ^ gf ^ lf, yb = (jd & gf) | (jd & lf) | (gf & lf),
    fb = re ^ yb, xd = ef ^ (re & yb), nd = za ^ (ef & ~xd),
    sd = wc ^ (za & ~nd), wd = ie ^ (wc & ~sd),
    qd = af ^ (ie & ~wd), jb = 0,
    ba = (cc & m0p) << 1 | rc & m0p, sb = (rc & m0n) >> 1 | cc & m0n,
    td = (ne & m0p) << 1 | mc & m0p, mf = (mc & m0n) >> 1 | ne & m0n,
    ea = (xd & m0p) << 1 | fb & m0p, hb = (fb & m0n) >> 1 | xd & m0n,
    of = (sd & m0p) << 1 | nd & m0p, ma = (nd & m0n) >> 1 | sd & m0n,
    hd = (qd & m0p) << 1 | wd & m0p, yd = (wd & m0n) >> 1 | qd & m0n,
    ta = (td & m1p) << 2 | ba & m1p, qf = (ba & m1n) >> 2 | td & m1n,
    ud = (mf & m1p) << 2 | sb & m1p, vb = (sb & m1n) >> 2 | mf & m1n,
    ga = (of & m1p) << 2 | ea & m1p, rd = (ea & m1n) >> 2 | of & m1n,
    zb = (ma & m1p) << 2 | hb & m1p, bc = (hb & m1n) >> 2 | ma & m1n,
    ec = (jb & m1p) << 2 | hd & m1p, nf = (hd & m1n) >> 2 | jb & m1n,
    xa = (jb & m1p) << 2 | yd & m1p, sc = (yd & m1n) >> 2 | jb & m1n,
    ib = (ga & m2p) << 4 | ta & m2p, ye = (ta & m2n) >> 4 | ga & m2n,
    ca = (zb & m2p) << 4 | ud & m2p, ld = (ud & m2n) >> 4 | zb & m2n,
    ke = (rd & m2p) << 4 | qf & m2p, xc = (qf & m2n) >> 4 | rd & m2n,
    na = (bc & m2p) << 4 | vb & m2p, ya = (vb & m2n) >> 4 | bc & m2n,
    db = (jb & m2p) << 4 | ec & m2p, fe = (ec & m2n) >> 4 | jb & m2n,
    pf = (jb & m2p) << 4 | xa & m2p, da = (xa & m2n) >> 4 | jb & m2n,
    de = (jb & m2p) << 4 | nf & m2p, ee = (nf & m2n) >> 4 | jb & m2n,
    fi = (jb & m2p) << 4 | sc & m2p, bb = (sc & m2n) >> 4 | jb & m2n,
    dd = (db & m3p) << 8 | ib & m3p, pe = (ib & m3n) >> 8 | db & m3n,
    nb = (pf & m3p) << 8 | ca & m3p, ab = (ca & m3n) >> 8 | pf & m3n,
    sa = (de & m3p) << 8 | ke & m3p, ac = (ke & m3n) >> 8 | de & m3n,
    kc = (fi & m3p) << 8 | na & m3p, md = (na & m3n) >> 8 | fi & m3n,
    jc = (fe & m3p) << 8 | ye & m3p, xb = (ye & m3n) >> 8 | fe & m3n,
    jf = (da & m3p) << 8 | ld & m3p, eb = (ld & m3n) >> 8 | da & m3n,
    pd = (ee & m3p) << 8 | xc & m3p, yc = (xc & m3n) >> 8 | ee & m3n,
    fd = (bb & m3p) << 8 | ya & m3p, bd = (ya & m3n) >> 8 | bb & m3n;
    buf[000] = dd >>  0; buf[001] = dd >> 16; buf[002] = dd >> 32;
    buf[003] = dd >> 48; buf[004] = nb >>  0; buf[005] = nb >> 16;
    buf[006] = nb >> 32; buf[007] = nb >> 48; buf[010] = sa >>  0;
    buf[011] = sa >> 16; buf[012] = sa >> 32; buf[013] = sa >> 48;
    buf[014] = kc >>  0; buf[015] = kc >> 16; buf[016] = kc >> 32;
    buf[017] = kc >> 48; buf[020] = jc >>  0; buf[021] = jc >> 16;
    buf[022] = jc >> 32; buf[023] = jc >> 48; buf[024] = jf >>  0;
    buf[025] = jf >> 16; buf[026] = jf >> 32; buf[027] = jf >> 48;
    buf[030] = pd >>  0; buf[031] = pd >> 16; buf[032] = pd >> 32;
    buf[033] = pd >> 48; buf[034] = fd >>  0; buf[035] = fd >> 16;
    buf[036] = fd >> 32; buf[037] = fd >> 48; buf[040] = pe >>  0;
    buf[041] = pe >> 16; buf[042] = pe >> 32; buf[043] = pe >> 48;
    buf[044] = ab >>  0; buf[045] = ab >> 16; buf[046] = ab >> 32;
    buf[047] = ab >> 48; buf[050] = ac >>  0; buf[051] = ac >> 16;
    buf[052] = ac >> 32; buf[053] = ac >> 48; buf[054] = md >>  0;
    buf[055] = md >> 16; buf[056] = md >> 32; buf[057] = md >> 48;
    buf[060] = xb >>  0; buf[061] = xb >> 16; buf[062] = xb >> 32;
    buf[063] = xb >> 48; buf[064] = eb >>  0; buf[065] = eb >> 16;
    buf[066] = eb >> 32; buf[067] = eb >> 48; buf[070] = yc >>  0;
    buf[071] = yc >> 16; buf[072] = yc >> 32; buf[073] = yc >> 48;
    buf[074] = bd >>  0; buf[075] = bd >> 16; buf[076] = bd >> 32;
    buf[077] = bd >> 48;

    memcpy(out + i, buf, sizeof(uint16_t) * (count - i < 64 ? count - i : 64));
  }
}

#define COUNT (1000000)

uint16_t buf[COUNT];

int main() {
  srand_lfib(2 /*time(0)*/);

  gen_rand_3dig(buf, COUNT);

  for (int i = 0; i < COUNT; ++i) {
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
