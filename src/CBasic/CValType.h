/***************************

@File: CValTypes.h
@Time: 2022/2/3 12:58 下午
@Desc: 
***************************/

#ifndef DAOGRAPH_CVALTYPE_H
#define DAOGRAPH_CVALTYPE_H

#include "CBasicDefine.h"
#include "CStatus.h"
#include "CException.h"

using CChar = DaoGraph::internal::CCHAR;
using CCharPtr = DaoGraph::internal::CCHAR *;
using CUChar = DaoGraph::internal::CUCHAR;
using CUCharPtr = DaoGraph::internal::CUCHAR *;
using CUInt = DaoGraph::internal::CUINT;
using CSize = DaoGraph::internal::CSIZE;
using CVoid = DaoGraph::internal::CVOID;
using CVoidPtr = DaoGraph::internal::CVOID *;
using CInt = DaoGraph::internal::CINT;
using CLong = DaoGraph::internal::CLONG;
using CULong = DaoGraph::internal::CULONG;
using CBool = DaoGraph::internal::CBOOL;
using CIndex = DaoGraph::internal::CINT;                // 表示标识信息，可以为负数
using CFloat = DaoGraph::internal::CFLOAT;
using CDouble = DaoGraph::internal::CDOUBLE;
using CConStr = DaoGraph::internal::CCONSTR;             // 表示 const char*
using CBigBool = DaoGraph::internal::CBIGBOOL;

using CLevel = DaoGraph::internal::CINT;
using CSec = DaoGraph::internal::CLONG;                  // 表示秒信息, for second
using CMSec = DaoGraph::internal::CLONG;                 // 表示毫秒信息, for millisecond
using CFMSec = DaoGraph::internal::CDOUBLE;              // 表示毫秒信息，包含小数点信息

using CStatus = DaoGraph::internal::CSTATUS;
using CException = DaoGraph::internal::CEXCEPTION;

#endif //DAOGRAPH_CVALTYPE_H
