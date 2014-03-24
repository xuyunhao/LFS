#ifndef LFS_UTIL_DEFS_H_
#define LFS_UTIL_DEFS_H_

#define __STDC_LIMIT_MACROS

#include <assert.h>
#include <cstddef>
#include <cinttypes>
#include <string>

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

namespace lfs {

typedef std::basic_string<uint8_t> charbuf;

};//namespace lfs
#endif//LFS_UTIL_DEFS_H_
