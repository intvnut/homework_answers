#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

int main() <%
  const char *s = "?Homework?";

  do (
    ispunct(*s++) ? putchar : abs
       )(
    getchar()
  ),

  *s || (
    exit(*s), 0
  );

  while (
    "!Homework!"
  );
%>
