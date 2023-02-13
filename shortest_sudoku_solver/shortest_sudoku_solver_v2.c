#include<stdio.h>
int p[109],i,c;
int s(int k){
 while(p[++k]);
 int o=1,j=-1;
 while(++j<9)o|=1<<p[(k+j*9)%81]|1<<p[k-k%9+j]|1<<p[k-k%27+k%9-k%3+j%3+j/3*9];
 while(k-81&&(p[k]=j--)&&(o>>j&2||!s(k)));
 return++j;
}
int main(){
for(;i-81&&(c=getchar())-EOF;i+=c>' ')p[i]='0'<c?c-'0':0;
for(s(i=-1)||(*p=0,puts("No solution"));p[++i];printf("%d%c",p[i],i%9-8?' ':'\n'));
}
}
