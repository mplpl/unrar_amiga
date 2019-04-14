#ifndef _RAR_LANG_
#define _RAR_LANG_

  #ifdef _AMIGA
    #include <loclangamiga.hpp>
  #elif defined(USE_RC)
    #include "rarres.hpp"
  #else
    #include "loclang.hpp"
  #endif

#endif
