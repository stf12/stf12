/**
 * cruntime_opt.c
 *
 *  Created on: Jan 31, 2011
 *      Author: oliveris
 */

#include <stdlib.h>

/**
 * This is an error handler that is invoked by the C++ runtime when a pure virtual function is called.
 * If anywhere in the runtime of your program an object is created with a virtual function pointer not
 * filled in, and when the corresponding function is called, you will be calling a 'pure virtual function'.
 * The handler you describe should be defined in the default libraries that come with your development environment.
 */
extern "C"
void __cxa_pure_virtual() {
	while (1);
}

void *operator new(size_t size) throw() { return malloc(size); }
void operator delete(void *p) throw() { free(p); }

/**
 * Called by runtime when static or global object are destroyed. This happen when main() exits.
 * Do nothing to prevent the destructors for global an static objects to be called.
 */
extern "C"
int __aeabi_atexit(void *object, void (*destructor)(void *), void *dso_handle)
{
  return 0;
}

extern "C"
void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
{
  while(1)
  {}
}

/*
* Minimal __assert() uses __assert__func()
* */
extern "C"
void __assert(const char *file, int line, const char *failedexpr)
{
   __assert_func (file, line, NULL, failedexpr);
}
