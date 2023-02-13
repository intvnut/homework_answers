#include<stdio.h>
int s[82];
int solve(unsigned i) {
 unsigned ok=0x3FE;
 while(s[i])i++;
 if(i==81)return 1;
 for(int j=0,r=i-i%9,b=((i/3)%3)*3+(i/27)*27;j<9;j++)
  ok&=~((1u<<s[(i+j*9)%81])|(1u<<s[r+j])|(1u<<s[b+(j%3)+9*(j/3)]));
 for(int j=1;j<=9;j++)
  if(ok&(1u<<j)){
   s[i]=j;
   if(solve(i+1))
    return 1;
  }
 s[i]=0;
 return 0;
}

int main() {
 int i=0,c;
 while ((c=getchar())!=EOF&&i<81) {
  if (c>='0'&&c<='9') s[i++]=c-'0';
  if (c=='.') s[i++]=0;
 }
 if (solve(0))
  for (i=0;i<81;i++)
   printf("%2d%c",s[i],i%9==8?'\n':' ');
 else
  puts("No solution");
}
