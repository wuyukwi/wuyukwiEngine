/*********************************************************************************

  *FileName: defines.h
            コウ  キガク
  *Author:  Huang QiYue
  *Version:  1.0
  *Date:  2022/04/02

**********************************************************************************/


#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <windows.h>
#include <cstdio>
#include "imgui.h"
#include "backends\imgui_impl_win32.h"

//#define SAFE_DELETE( p )       { if( p ) { delete ( p );     ( p ) = NULL; } }
//#define SAFE_DELETE_ARRAY( p ) { if( p ) { delete[] ( p );   ( p ) = NULL; } }
//#define SAFE_RELEASE( p )      { if( p ) { ( p )->Release(); ( p ) = NULL; } }

#define KEYS_SIZE (256)

// Boolean values.
#define UGP_INFINITE -1
#define UGP_INVALID -1
#define UGP_OK       1
#define UGP_FAIL     0

// Vertex formats.
#define GUI_FVF      2
#define MV_FVF       3
// Types of controls we support.
#define UGP_GUI_STATICTEXT 1
#define UGP_GUI_BUTTON     2
#define UGP_GUI_BACKDROP   3

// Mouse button states.
#define UGP_BUTTON_UP      1
#define UGP_BUTTON_OVER    2
#define UGP_BUTTON_DOWN    3

#define MOUSE_LEFT_BUTTON  0
#define MOUSE_RIGHT_BUTTON 1

// ライトタイプ
#define LIGHT_POINT        1
#define LIGHT_DIRECTIONAL  2
#define LIGHT_SPOT         3

// Window handle (need new way if porting to Mac and OpenGL).
#define WinHWND HWND

// Inline関数
template <typename T>
inline void SAFE_DELETE(T& p)
{
    if (p)
    {
        delete(p);

        p = nullptr;
    }

}
template <typename T>
inline void SAFE_DELETE_ARRAY(T& p)
{
    if (p)
    {
        delete[](p);
        p = nullptr;
    }
}
template <class T>
inline void SAFE_RELEASE(T& p)
{
    if (p)
    {
        p->Release();
        p = nullptr;
    }
}
template <typename T>
inline unsigned long COLOR_ARGB(T a, T r, T g, T b)
{
    return (unsigned long)(((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((b) & 0xff) << 8) | ((b) & 0xff);
}

// Typedefs and enumerations.
typedef long VertexType;
// プリミティブタイプ
enum class PrimType
{
   NULL_TYPE,
   POINT_LIST,
   TRIANGLE_LIST,
   TRIANGLE_STRIP,
   TRIANGLE_FAN,
   LINE_LIST,
   LINE_STRIP
};

enum class RenderState
{
    CULL_NONE,
    CULL_CW,
    CULL_CCW,
    DEPTH_NONE,
    DEPTH_READONLY,
    DEPTH_READWRITE,
    SHADE_POINTS,
    SHADE_SOLIDTRI,
    SHADE_WIRETRI,
    SHADE_WIREPOLY,
    TRANSPARENCY_NONE,
    TRANSPARENCY_ENABLE
};

enum class TransperentState
{
    ZERO = 1,
    ONE,
    SRCCOLOR,
    INVSRCCOLOR,
    SRCALPHA,
    INVSRCALPHA,
    DSTALPHA,
    INVDSTALPHA,
    DSTCOLOR,
    INVDSTCOLOR,
    SRCALPHASAT,
    BOTHSRCALPHA,
    INVBOTHSRCALPHA,
    BLENDFACTOR,
    INVBLENDFACTOR
};

enum TextureState
{
    MIN_FILTER,
    MAG_FILTER,
    MIP_FILTER
};

enum FilterType
{
    POINT_TYPE,
    LINEAR_TYPE,
    ANISOTROPIC_TYPE
};

enum MS_TYPE
{
    MS_NONE,
    MS_SAMPLES_2,
    MS_SAMPLES_4,
    MS_SAMPLES_8,
    MS_SAMPLES_16
};

enum FOG_TYPE
{
    VERTEX_FOR,
    PIXEL_FOG
};
#endif