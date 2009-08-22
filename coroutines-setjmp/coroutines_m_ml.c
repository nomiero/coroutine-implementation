#include <setjmp.h>
#include <stdio.h> 
 jmp_buf buf1;
  jmp_buf sched;
  jmp_buf main_buf;
  init=0;
 int is_resume=0;
 struct {
   jmp_buf buf;
 //  coroutine prev;
  // void *(func());
 } coroutine;
 
void coroutine_init(void* (func()) ){
 // coroutine c;
 // c->func = func;
 // return c;
}

#define resume(func)({                    \
       is_resume=1;                       \   
       setjmp(main_buf);                  \
       if(!init){                         \   
         init=1;                          \
         printf("%d\n",func());                          \
       }                                  \
       else if (is_resume){               \                       
        longjmp(buf1,1);                  \
       }                                  \
      })                                   
  
#define  yield()({                    \
 is_resume=0;                         \ 
 setjmp(buf1);                        \ 
 printf("yielding\n");                \
 if(!is_resume){                      \
  longjmp(main_buf,1);                \   
 }                                    \
})                                     

 
int  myfunc(){
 printf("starting func\n");
 yield();
 printf("ending func\n");
 return 5;
}

int main (void)
{
   int first=0;
   int second=0;
   resume(myfunc);
   resume(myfunc);
   return 0;
}

