
#include "Coro.h"
#define CORO_STACK_SIZE     8192
#define CORO_DEFAULT_STACK_SIZE  (65536*4)
#define CORO_STACK_SIZE_MIN 8192

typedef void (*makecontext_func)(void);

typedef struct CallbackBlock
{
	void *context;
	CoroStartCallback *func;
} CallbackBlock;

Coro *current;

void Coro_allocStackIfNeeded(Coro *self)
{
		self->stack = (void *)calloc(1, self->requestedStackSize + 16);
	self->allocatedStackSize = self->requestedStackSize;
	printf("Coro_%p allocating stack size %i\n", (void *)self, self->requestedStackSize);
}

void yield(){
 if(current->isMain){
  printf("cann't yield from root fiber");
 }else{
  Coro * c1;
  Coro * c2;
  c1 = current;
  c2 = current->prev;
  current->prev=NULL;
  current =  c2;
  Coro_switchTo_(c1,c2);
 }
}

void resume(Coro *c){
 c->prev = current; 
 current = c;
 Coro_switchTo_(current,c);
}

Coro *Coro_new(void)
{
	Coro *self = (Coro *)calloc(1, sizeof(Coro));
	self->requestedStackSize = CORO_DEFAULT_STACK_SIZE;
	self->allocatedStackSize = 0;

	self->stack = NULL;
	return self;
}

void *Coro_stack(Coro *self)
{
	return self->stack;
}

size_t Coro_stackSize(Coro *self)
{
	return self->requestedStackSize;
}


void Coro_StartWithArg(CallbackBlock *block)
{
	(block->func)(block->context);
	printf("Scheduler error: returned from coro start function\n");
	exit(-1);
}

void Coro_initializeMainCoro(Coro *self)
{
 current = self;
	self->isMain = 1;
}

void Coro_switchTo_(Coro *self, Coro *next)
{
	swapcontext(&self->env, &next->env);
}

void Coro_setup(Coro *self, void *arg)
{
	ucontext_t *ucp = (ucontext_t *) &self->env;
	getcontext(ucp);
	ucp->uc_stack.ss_sp    = Coro_stack(self);
	ucp->uc_stack.ss_size  = Coro_stackSize(self);
	ucp->uc_stack.ss_flags = 0;
	ucp->uc_link = NULL;
	makecontext(ucp, (makecontext_func)Coro_StartWithArg, 1, arg);
}


void Coro_startCoro_(Coro *self, Coro *other, void *context, CoroStartCallback *callback)
{
	CallbackBlock block;
	block.context = context;
	block.func    = callback;
	Coro_allocStackIfNeeded(other);
	Coro_setup(other, &block);
	Coro_switchTo_(self, other);
}




Coro *firstCoro, *secondCoro;

void secondTask(void *context)
{
	int num = 0;
	
	printf("secondTask created with value %d\n", *(int *)context);
	
	while (1) 
	{
	 printf("inside coroutine 2");
//		printf("secondTask: %d %d\n", (int)Coro_bytesLeftOnStack(secondCoro), num++);
		Coro_switchTo_(secondCoro, firstCoro);
	}
}

void firstTask(void *context)
{
	int value = 2;
	int num = 0;
	
	printf("firstTask created with value %d\n", *(int *)context);
	secondCoro = Coro_new();
	Coro_startCoro_(firstCoro, secondCoro, (void *)&value, secondTask);
	
	while (1) 
	{
	printf("inside coroutine 1");
//		printf("firstTask:  %d %d\n", (int)Coro_bytesLeftOnStack(firstCoro), num++);
		Coro_switchTo_(firstCoro, secondCoro);
	}
}

int main()
{
	Coro *mainCoro = Coro_new();
	int value = 1;
	
	//Coro_initializeMainCoro(mainCoro);
	
	firstCoro = Coro_new();
	Coro_startCoro_(mainCoro, firstCoro, (void *)&value, firstTask);
	
	
}


