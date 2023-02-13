// file2.c 
#include <stdio.h> 
  
int class; 
  
struct foo { 
  struct bar { 
    struct { 
      char *throw; 
      int catch; 
    }; 
  } try; 
}; 
  
int public(protected) 
  char *restrict protected; 
{ 
   extern int using(); 
   struct foo typename = { 
     .try = { .catch = class, .throw = protected } 
   }; 
  
   return using(&typename); 
} 
  
int using(namespace) 
  struct bar *namespace; 
{ 
  fwrite(namespace->throw, 1, namespace->catch, stdout); 
  return sizeof('a') - sizeof(int); 
} 
