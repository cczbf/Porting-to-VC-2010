//NTType.h : definition data type.
//Begin  on: May 24,1996
//Finish on: May 24,1996
//By Yang Zhenkun.
//

#ifndef NTType_Include
#define NTType_Include

typedef signed    _int8  sint8 ;
typedef unsigned  _int8  uint8 ;
typedef signed    _int16 sint16 ;
typedef unsigned  _int16 uint16 ;
typedef signed    _int32 sint32 ;
typedef unsigned  _int32 uint32 ;
typedef signed    _int64 sint64 ;
typedef unsigned  _int64 uint64 ;

typedef signed    _int8  SINT8 ;
//typedef unsigned  _int8  UINT8 ;
typedef signed    _int16 SINT16 ;
//typedef unsigned  _int16 UINT16 ;
typedef signed    _int32 SINT32 ;
//typedef unsigned  _int32 UINT32 ;
typedef signed    _int64 SINT64 ;
typedef unsigned  _int64 UINT64 ;

typedef sint8      far * PSINT8 ;
//typedef uint8      far * PUINT8 ;
typedef sint16     far * PSINT16 ;
//typedef uint16     far * PUINT16 ;
typedef sint32     far * PSINT32 ;
//typedef uint32     far * PUINT32 ;
typedef sint64     far * PSINT64 ;
typedef uint64     far * PUINT64 ;

typedef sint8      far * LPSINT8 ;
typedef uint8      far * LPUINT8 ;
typedef sint16     far * LPSINT16 ;
typedef uint16     far * LPUINT16 ;
typedef sint32     far * LPSINT32 ;
typedef uint32     far * LPUINT32 ;
typedef sint64     far * LPSINT64 ;
typedef uint64     far * LPUINT64 ;

const uint8  UINT8MAX  = 0xff;
const sint8  SINT8MAX  = 0x7f;
const uint16 UINT16MAX = 0xffff;
const sint16 SINT16MAX = 0x7fff;
const uint32 UINT32MAX = 0xffffffff;
const sint32 SINT32MAX = 0x7fffffff;
const uint64 UINT64MAX = 0xffffffffffffffff;
const sint64 SINT64MAX = 0x7fffffffffffffff;

const int UINT8WIDTH  = 8;
const int SINT8WIDTH  = 8;
const int UINT16WIDTH = 16;
const int SINT16WIDTH = 16;
const int UINT32WIDTH = 32;
const int SINT32WIDTH = 32;
const int UINT64WIDTH = 64;
const int SINT64WIDTH = 64;

const uint32  UNSIGNEDBMPALIGN_VALUE = 0xffffffe0;
const uint32  UNSIGNEDBMPALIGN_REST = 0x1f;
const sint32  SIGNEDBMPALIGN_VALUE = 0xffffffe0;
const sint32  SIGNEDBMPALIGN_REST = 0x1f;

const float ABS_MIN_FLT = 0.00001f;
const float PI_VALUE = 3.1415927f;
const float TWOPI_VALUE = PI_VALUE * 2;
const float ABS_MIN_ANGLE = 0.01f;
#define FloatToInt32(x) (((x) > (-ABS_MIN_FLT)) ? (sint32)(x+0.5f) : (sint32)(x-0.5f))
#define FloatToLong(x)  FloatToInt32(x)


//lhf 98.12.24 copy from halfton.h
#define  ROUND_SCREEN                   0
#define  ELLIPTICAL_SCREEN              1
#define  DIAMOND_SCREEN                 2
#define  SQUARE_SCREEN                  3
#define  DIFFUSION_SCREEN               4
#define  PUREROUND_SCREEN               5
#define  SHARPELLIPSE_SCREEN            6
#define  CROSS_SCREEN                   7
#define  FOUNDERFM_SCREEN               8
#define  RHOMB_SCREEN                   9   // YB 1998.04.08 新增加菱形网点

#endif  //NTType_Include
