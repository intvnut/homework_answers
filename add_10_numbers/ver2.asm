; Add the 10 integers at data together, with result in R0 
      CLRR   R0 
      ADD3x  R0, 10,   X1 
      MVII   #data,    R4 
loop: ADD    @R4++,    R0 
      DECBNZ X0, loop 
