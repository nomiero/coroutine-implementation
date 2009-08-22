#include <setjmp.h>
#include <stdio.h> 
 jmp_buf buf1;
  jmp_buf sched;
  jmp_buf main_buf;
  init=0;
 int is_resume=0;
 
void resume(void *(func())){                    
       is_resume=1;                          
       setjmp(main_buf);                  
       if(!init){                            
         init=1;                          
         func();                          
       }                                  
       else if (is_resume){                                      
        longjmp(buf1,1);                  
       }                                  
      }                                   
  
void  yield(){                    
 is_resume=0;                          
 setjmp(buf1);                         
 printf("yielding\n");                
 if(!is_resume){                      
  longjmp(main_buf,1);                   
 }                                    
}                                     

 
void myfunc(){
 printf("starting func\n");
 yield();
 printf("ending func\n");
}
int main (void)
{
   int first=0;
   int second=0;
   resume(myfunc);
   resume(myfunc);
   return 0;
}

