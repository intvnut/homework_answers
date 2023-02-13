#include <stdio.h> 
#include <stdint.h> 
#include <sys/mman.h> 
#include <unistd.h> 
 
typedef int fxn(int); 
 
void *align_to(void *p, uintptr_t size) { 
  uintptr_t u = (uintptr_t)p; 
  return (void *)(u & -size); 
} 
 
int main() { 
  int page_size = sysconf(_SC_PAGE_SIZE); 
   
  mprotect( 
    align_to(&page_size, page_size), 
    page_size, 
    PROT_READ | PROT_WRITE | PROT_EXEC 
  ); 
 
  unsigned char add_one_obj[] = { 
    // lea 1(%rdi), %eax 
    0x8D, 0x47, 0x01, 
    // ret 
    0xC3 
  }; 
 
  fxn* add_one_fxn = (fxn*)add_one_obj; 
 
  printf("1+1=%d\n", add_one_fxn(1)); 
} 
