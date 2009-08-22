#include <setjmp.h>
 
 jmp_buf buf1;

 void f2(){
    printf("inside f2\n");
    longjmp(buf1,1);
    printf("end of f2\n");
 }
 
 void f1(){
   int end=0;
   setjmp(buf1);
   if(!end){
    end =1;
    printf("inside f1\n");
    f2();
   }
   printf("end of f1\n");
 }
int main (void)
{
   f1(); 
   return 0;
}

