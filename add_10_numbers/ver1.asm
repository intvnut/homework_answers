; Add the 10 integers at data together, with result in R0 
      CLRR   R0 
      ADD3x  R0, data, X0 
      ADD3x  R0, 10,   X1 
loop: ADD    @X0++(1), R0 
      DECBNZ X1, loop 
