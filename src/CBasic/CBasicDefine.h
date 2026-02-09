/***************************

@File: CBasicDefine.h
@Time: 2021/4/26 8:15 下午
@Desc:
***************************/

#ifndef DAOGRAPH_CBASICDEFINE_H
#define DAOGRAPH_CBASICDEFINE_H

#include <cstddef>

#define DAOGRAPH_NAMESPACE_BEGIN                                          \
namespace DaoGraph {                                                      \

#define DAOGRAPH_NAMESPACE_END                                            \
} /* end of namespace DaoGraph */                                         \

DAOGRAPH_NAMESPACE_BEGIN

#define DAOGRAPH_INTERNAL_NAMESPACE_BEGIN                                 \
namespace internal {                                                    \

#define DAOGRAPH_INTERNAL_NAMESPACE_END                                   \
} /* end of namespace internal */                                       \

DAOGRAPH_INTERNAL_NAMESPACE_BEGIN

using CCHAR = char;
using CUCHAR = unsigned char;
using CUINT = unsigned int;
using CVOID = void;
using CINT = int;
using CLONG = long;
using CULONG = unsigned long;
using CBOOL = bool;
using CBIGBOOL = int;
using CFLOAT = float;
using CDOUBLE = double;
using CCONSTR = const char*;
using CSIZE = size_t;

DAOGRAPH_INTERNAL_NAMESPACE_END
DAOGRAPH_NAMESPACE_END

#endif //DAOGRAPH_CBASICDEFINE_H
