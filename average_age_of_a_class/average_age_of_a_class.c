#include <fenv.h> 
#include <math.h> 
#include <stdbool.h> 
#include <stdio.h> 
#include <stdlib.h> 
 
#define CLASS_SIZE 20 
 
double return_average(double the_average_value) { 
  return the_average_value; 
} 
 
int main() { 
  fesetround(FE_UPWARD); 
  char array[CLASS_SIZE]; 
  double students = sizeof(array); 
  double inverse_students = 1. / students; 
  double average_age = 0.; 
  double percentage_measured = 0.; 
  double student_age; 
  while (percentage_measured < 100.) { 
    do { 
      printf("Age? "); 
      switch (scanf("%lf", &student_age)) { 
        case EOF: exit(1); 
        case true: goto good; 
        default: puts("Enter a number."); 
      } 
    } while (NAN); 
good:  
    average_age += student_age * inverse_students; 
    percentage_measured += 100. * inverse_students; 
  } 
  printf("Average age: %f\n",  
         return_average(average_age)); 
} 
