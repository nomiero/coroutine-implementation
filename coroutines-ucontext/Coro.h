#include <stdio.h>
#include <ucontext.h>
typedef struct Coro Coro;

struct Coro
{
	size_t requestedStackSize;
	size_t allocatedStackSize;
	void *stack;
	Coro *prev;
	ucontext_t env;
	unsigned char isMain;
	unsigned char isFirstRun;
};
 void yield();
 void resume(Coro *c);
 void *Coro_stack(Coro *self);
 size_t Coro_stackSize(Coro *self);
 void Coro_setStackSize_(Coro *self, size_t sizeInBytes);
 size_t Coro_bytesLeftOnStack(Coro *self);
 int Coro_stackSpaceAlmostGone(Coro *self);

 void Coro_initializeMainCoro(Coro *self);

typedef void (CoroStartCallback)(void *);

 void Coro_init(Coro *other, void *context, CoroStartCallback *callback);
 void Coro_switchTo_(Coro *self, Coro *next);

