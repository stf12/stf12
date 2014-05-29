//
// This file is part of the µOS++ III distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

// These functions are redefined locally, to avoid references to some
// heavy implementations in the standard C++ library.

// ----------------------------------------------------------------------------

#include <cstdlib>
#include <sys/types.h>
#include "diag/Trace.h"

// ----------------------------------------------------------------------------

namespace __gnu_cxx
{
  void
  __attribute__((noreturn))
  __verbose_terminate_handler();

  void
  __verbose_terminate_handler()
  {
    trace_puts(__func__);
    abort();
  }
}

// ----------------------------------------------------------------------------

extern "C"
{
  void
  __attribute__((noreturn))
  __cxa_pure_virtual();

  int
  __aeabi_atexit(void *object, void (*destructor)(void *), void *dso_handle);

  void
  __cxa_pure_virtual()
  {
    trace_puts(__func__);
    abort();
  }

  /**
   * Called by runtime when static or global object are destroyed. This happen when main() exits.
   * Do nothing to prevent the destructors for global an static objects to be called.
   */
  int
  __aeabi_atexit(void *object, void (*destructor)(void *), void *dso_handle)
  {
    return 0;
  }
}

// ----------------------------------------------------------------------------

