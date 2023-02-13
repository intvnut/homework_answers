void sort2(uint32_t *pa, uint32_t *pb) {
  const uint32_t a = *pa;
  const uint32_t b = *pb;
  const uint32_t c = a ^ b;
  const uint32_t d = c & b;
  const uint32_t e = d | (d / 16);
  const uint32_t f = e | (e / 65536);
  const uint32_t g = f | (f / 2);
  const uint32_t h = g | (g / 256);
  const uint32_t i = h | (h / 4);
  const uint32_t j = c & -!(c & ~i);
  const uint32_t k = a ^ j;
  const uint32_t l = b ^ j;
  *pa = k;
  *pb = l;
}
