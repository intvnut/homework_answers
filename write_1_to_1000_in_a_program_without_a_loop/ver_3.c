extern volatile char utxreg;  // UART transmit register

static int to_drop;

static inline void tx_char(const char c) {
  if (to_drop) { to_drop--; return; }
  utxreg = c;
}

static inline void tx_num(int h, int t, int u) {
  tx_char('0');
  tx_char(h);
  tx_char(t);
  tx_char(u);
  tx_char(' ');
}

#define SEND_XX0_TO_XX9(h,t) \
  tx_num(h,t,'0'); \
  tx_num(h,t,'1'); \
  tx_num(h,t,'2'); \
  tx_num(h,t,'3'); \
  tx_num(h,t,'4'); \
  tx_num(h,t,'5'); \
  tx_num(h,t,'6'); \
  tx_num(h,t,'7'); \
  tx_num(h,t,'8'); \
  tx_num(h,t,'9');

#define SEND_X0X_TO_X9X(h) \
  SEND_XX0_TO_XX9(h,'0'); \
  SEND_XX0_TO_XX9(h,'1'); \
  SEND_XX0_TO_XX9(h,'2'); \
  SEND_XX0_TO_XX9(h,'3'); \
  SEND_XX0_TO_XX9(h,'4'); \
  SEND_XX0_TO_XX9(h,'5'); \
  SEND_XX0_TO_XX9(h,'6'); \
  SEND_XX0_TO_XX9(h,'7'); \
  SEND_XX0_TO_XX9(h,'8'); \
  SEND_XX0_TO_XX9(h,'9')


#define SEND_0XX_TO_9XX() \
  SEND_X0X_TO_X9X('0'); \
  SEND_X0X_TO_X9X('1'); \
  SEND_X0X_TO_X9X('2'); \
  SEND_X0X_TO_X9X('3'); \
  SEND_X0X_TO_X9X('4'); \
  SEND_X0X_TO_X9X('5'); \
  SEND_X0X_TO_X9X('6'); \
  SEND_X0X_TO_X9X('7'); \
  SEND_X0X_TO_X9X('8'); \
  SEND_X0X_TO_X9X('9')


int main() {
  to_drop = 5;
  SEND_0XX_TO_9XX();
  tx_char('1');
  tx_char('0');
  tx_char('0');
  tx_char('0');
}
