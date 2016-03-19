#ifndef __common_h__
#define __common_h__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <list>
#include <set>
#include <sstream>
#include <time.h>
#include <unordered_map>
#include <unordered_set>  
#include <iostream>
#include <deque>

#include "CUtil/inc/MsgAssert.h" 
#include "CUtil/inc/ClassTypeID.h"
#include "CUtil/inc/Platform.h"
#include "CUtil/inc/CErrno.h"
#include "CUtil/inc/BoostHelper.h"

#define TRUE    1
#define FALSE   0

#pragma warning(disable:4251)

typedef const char *     ConstCharPtr;
typedef unsigned char    UINT8; 

typedef short            INT16;
typedef unsigned short   UINT16; 

typedef ptrdiff_t        PtrDiff;

typedef int   BOOL;
typedef int   INT32;
typedef unsigned int   UINT32; 
typedef std::string    std_string;

#define MAX_NAME_LENGTH					50
#define MSG_MAX_PARAMETER_NUMBER		10 

#define SAFE_DELETE(x) if(x){ delete x; x = NULL;}
#define SAFE_DELETE_ARRAY(x) if(x){ delete []x; x = NULL;}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(x[0]))

#endif