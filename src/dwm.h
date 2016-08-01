//---------------------------------------------------------------------------
#ifndef DWM_H
#define DWM_H

#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include <dwmapi.h>
#include <uxtheme.h>

// version 2016.06.10

//---------------------------------------------------------------------------
#define DWM_LIBRARY "dwmapi.dll"
#define USER_LIBRARY "user32.dll"
#define DWM_DEF_WINDOWPROC "DwmDefWindowProc"
#define DWM_ENABLE_BLUR_BEHIND_WINDOW "DwmEnableBlurBehindWindow"
#define DWM_ENABLE_COMPOSITION "DwmEnableComposition"
#define DWM_EXTEND_FRAME_INTO_CLIENT_AREA "DwmExtendFrameIntoClientArea"
#define DWM_GET_COLORIZATION_COLOR "DwmGetColorizationColor"
#define DWM_GET_WINDOW_ATTRIBUTE "DwmGetWindowAttribute"
#define DWM_IS_COMPOSITION_ENABLED "DwmIsCompositionEnabled"
#define DWM_SET_WINDOW_ATTRIBUTE "DwmSetWindowAttribute"
#define DWM_SET_WINDOW_THEME_NON_CLIENT_ATTRIBUTES "SetWindowThemeNonClientAttributes"
#define DWM_SET_WINDOW_COMPOSITION_ATTRIBUTE "SetWindowCompositionAttribute"

#define WTNCA_NODRAWCAPTION 0x00000001
#define WTNCA_NODRAWICON    0x00000002
#define WTNCA_NOSYSMENU     0x00000004
#define WTNCA_NOMIRRORHELP  0x00000008
#define WTNCA_VALIDBITS     0x0000000f

/*#define DWM_BB_ENABLE 0x00000001
#define DWM_BB_BLURREGION 0x00000002
#define DWM_BB_TRANSITIONONMAXIMIZED 0x00000004

#define DWM_EC_DISABLECOMPOSITION 0x00000000
#define DWM_EC_ENABLECOMPOSITION 0x00000001*/

//---------------------------------------------------------------------------
namespace NDwm
{
  enum EWinCompAttr
  {
    WCA_UNDEFINED = 0,
    WCA_NCRENDERING_ENABLED,
    WCA_NCRENDERING_POLICY,
    WCA_TRANSITIONS_FORCEDISABLED,
    WCA_ALLOW_NCPAINT,
    WCA_CAPTION_BUTTON_BOUNDS,
    WCA_NONCLIENT_RTL_LAYOUT,
    WCA_FORCE_ICONIC_REPRESENTATION,
    WCA_FLIP3D_POLICY,
    WCA_EXTENDED_FRAME_BOUNDS,
    WCA_HAS_ICONIC_BITMAP,
    WCA_THEME_ATTRIBUTES,
    WCA_NCRENDERING_EXILED,
    WCA_NCADORNMENTINFO,
    WCA_EXCLUDED_FROM_LIVEPREVIEW,
    WCA_VIDEO_OVERLAY_ACTIVE,
    WCA_FORCE_ACTIVEWINDOW_APPEARANCE,
    WCA_DISALLOW_PEEK,
    WCA_CLOAK,
    WCA_CLOAKED,
    WCA_ACCENT_POLICY,
    WCA_ACCENT_TRANSFORM,
    WCA_LAST
  };

  enum EAccentState
  {
    ACCENT_DISABLED = 0,
    ACCENT_ENABLE_GRADIENT,
    ACCENT_ENABLE_TRANSPARENTGRADIENT,
    ACCENT_ENABLE_BLURBEHIND,
    ACCENT_INVALID_STATE
  };
}

struct SDwmMargins
{
  int cxLeftWidth;
  int cxRightWidth;
  int cyTopHeight;
  int cyBottomHeight;

  inline SDwmMargins() : cxLeftWidth(0), cxRightWidth(0), cyTopHeight(0), cyBottomHeight(0) {}
  inline SDwmMargins(int cxLeftWidth, int cxRightWidth, int cyTopHeight, int cyBottomHeight) : cxLeftWidth(cxLeftWidth), cxRightWidth(cxRightWidth), cyTopHeight(cyTopHeight), cyBottomHeight(cyBottomHeight) {}
};

struct SDwmBlurBehind
{
  DWORD dwFlags;
  BOOL fEnable;
  HRGN hRgnBlur;
  BOOL fTransitionOnMaximized;

  inline SDwmBlurBehind() : dwFlags(0), fEnable(0), hRgnBlur(0), fTransitionOnMaximized(0) {}
  inline SDwmBlurBehind(DWORD dwFlags, BOOL fEnable, HRGN hRgnBlur, BOOL fTransitionOnMaximized) : dwFlags(dwFlags), fEnable(fEnable), hRgnBlur(hRgnBlur), fTransitionOnMaximized(fTransitionOnMaximized) {}
};

struct SWinCompAttrData
{
  NDwm::EWinCompAttr attribute;
  PVOID pData;
  ULONG dataSize;

  inline SWinCompAttrData() : attribute(NDwm::WCA_UNDEFINED), pData(NULL), dataSize(0) {}
  inline SWinCompAttrData(NDwm::EWinCompAttr attribute, PVOID pData, ULONG dataSize) : attribute(attribute), pData(pData), dataSize(dataSize) {}
};

struct SAccentPolicy
{
  NDwm::EAccentState accentState;
  int accentFlags;
  int gradientColor;
  int animationId;

  inline SAccentPolicy() : accentState(NDwm::ACCENT_DISABLED), accentFlags(0), gradientColor(0), animationId(0) {}
  inline SAccentPolicy(NDwm::EAccentState accentState, int accentFlags, int gradientColor, int animationId) : accentState(accentState), accentFlags(accentFlags), gradientColor(gradientColor), animationId(animationId) {}
};

/*enum EDwmWindowAttribute
{
  DWMWA_NCRENDERING_ENABLED = 1,
  DWMWA_NCRENDERING_POLICY,
  DWMWA_TRANSITIONS_FORCEDISABLED,
  DWMWA_ALLOW_NCPAINT,
  DWMWA_CAPTION_BUTTON_BOUNDS,
  DWMWA_NONCLIENT_RTL_LAYOUT,
  DWMWA_FORCE_ICONIC_REPRESENTATION,
  DWMWA_FLIP3D_POLICY,
  DWMWA_EXTENDED_FRAME_BOUNDS,
  DWMWA_HAS_ICONIC_BITMAP,
  DWMWA_DISALLOW_PEEK,
  DWMWA_EXCLUDED_FROM_PEEK,
  DWMWA_CLOAK,
  DWMWA_CLOAKED,
  DWMWA_FREEZE_REPRESENTATION,
  DWMWA_LAST
};*/
//---------------------------------------------------------------------------
class CDwm
{
  private:
    HWND hWnd;

    typedef BOOL    (WINAPI *t_DwmDefWindowProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);
    typedef HRESULT (WINAPI *t_DwmEnableBlurBehindWindow)(HWND hWnd, const SDwmBlurBehind *pBlurBehind);
    typedef HRESULT (WINAPI *t_DwmEnableComposition)(UINT uCompositionAction);
    typedef HRESULT (WINAPI *t_DwmExtendFrameIntoClientArea)(HWND hwnd, const SDwmMargins *margins);
    typedef HRESULT (WINAPI *t_DwmGetColorizationColor)(DWORD *pcrColorization, BOOL *pfOpaqueBlend);
    typedef HRESULT (WINAPI *t_DwmGetWindowAttribute)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
    typedef HRESULT (WINAPI *t_DwmIsCompositionEnabled)(BOOL *pfEnabled);
    typedef HRESULT (WINAPI *t_DwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
    typedef HRESULT (WINAPI *t_DwmSetWindowThemeNonClientAttributes)(HWND hwnd, DWORD dwMask, DWORD dwAttributes);
    typedef BOOL    (WINAPI *t_SetWindowCompositionAttribute)(HWND hwnd, const SWinCompAttrData *pAttrData);

    t_DwmDefWindowProc dwmDefWindowProc;
    t_DwmEnableBlurBehindWindow dwmEnableBlurBehindWindow;
    t_DwmEnableComposition dwmEnableComposition;
    t_DwmExtendFrameIntoClientArea dwmExtendFrameIntoClientArea;
    t_DwmGetColorizationColor dwmGetColorizationColor;
    t_DwmGetWindowAttribute dwmGetWindowAttribute;
    t_DwmIsCompositionEnabled dwmIsCompositionEnabled;
    t_DwmSetWindowAttribute dwmSetWindowAttribute;
    t_DwmSetWindowThemeNonClientAttributes dwmSetWindowThemeNonClientAttributes;
    t_SetWindowCompositionAttribute tSetWindowCompositionAttribute;

  public:
    inline CDwm(HWND handle = NULL, unsigned char loadSymbols = 0) : hWnd(NULL), dwmDefWindowProc(NULL), dwmEnableBlurBehindWindow(NULL), dwmEnableComposition(NULL), dwmExtendFrameIntoClientArea(NULL), dwmGetColorizationColor(NULL), dwmGetWindowAttribute(NULL), dwmIsCompositionEnabled(NULL), dwmSetWindowAttribute(NULL) {if(loadSymbols) load(handle);}
    inline ~CDwm() {}
    void load(HWND handle);

    inline BOOL defWindowProc(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)         {if((hWnd) && (dwmDefWindowProc)) return dwmDefWindowProc(hWnd, msg, wParam, lParam, plResult); return 0;}
    inline HRESULT enableBlurBehindWindow(const SDwmBlurBehind &pBlurBehind)                     {if((hWnd) && (dwmEnableBlurBehindWindow)) return dwmEnableBlurBehindWindow(hWnd, &pBlurBehind); return 0;}
    inline HRESULT enableComposition(UINT uCompositionAction)                                    {if((hWnd) && (dwmEnableComposition)) return dwmEnableComposition(uCompositionAction); return 0;}
    inline HRESULT extendFrameIntoClientArea(const SDwmMargins &pMarInset)                       {if((hWnd) && (dwmExtendFrameIntoClientArea)) return dwmExtendFrameIntoClientArea(hWnd, &pMarInset); return 0;}
    inline HRESULT getColorizationColor(DWORD *pcrColorization, BOOL *pfOpaqueBlend)             {if((hWnd) && (dwmGetColorizationColor)) return dwmGetColorizationColor(pcrColorization, pfOpaqueBlend); return 0;}
    inline HRESULT getWindowAttribute(DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute)   {if((hWnd) && (dwmGetWindowAttribute)) return dwmGetWindowAttribute(hWnd, dwAttribute, pvAttribute, cbAttribute); return 0;}
    inline HRESULT isCompositionEnabled(BOOL *pfEnabled)                                         {if((hWnd) && (dwmIsCompositionEnabled)) return dwmIsCompositionEnabled(pfEnabled); return 0;}
    inline HRESULT setWindowAttribute(DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute) {if((hWnd) && (dwmSetWindowAttribute)) return dwmSetWindowAttribute(hWnd, dwAttribute, pvAttribute, cbAttribute); return 0;}
    inline HRESULT setWindowThemeNonClientAttributes(DWORD dwMask, DWORD dwAttributes)           {if((hWnd) && (dwmSetWindowThemeNonClientAttributes)) return dwmSetWindowThemeNonClientAttributes(hWnd, dwMask, dwAttributes); return 0;}
    inline BOOL    setWindowCompositionAttribute(const SWinCompAttrData *pAttrData)              {if((hWnd) && (tSetWindowCompositionAttribute)) return tSetWindowCompositionAttribute(hWnd, pAttrData); return 0;}
};
//---------------------------------------------------------------------------
inline void CDwm::load(HWND handle)
{
  hWnd = handle;
  HMODULE module;

  if(!(module = LoadLibraryA(DWM_LIBRARY)))
    return;
  dwmDefWindowProc = reinterpret_cast<t_DwmDefWindowProc>(GetProcAddress (module, DWM_DEF_WINDOWPROC));
  dwmEnableBlurBehindWindow = reinterpret_cast<t_DwmEnableBlurBehindWindow>(GetProcAddress (module, DWM_ENABLE_BLUR_BEHIND_WINDOW));
  dwmEnableComposition = reinterpret_cast<t_DwmEnableComposition>(GetProcAddress (module, DWM_ENABLE_COMPOSITION));
  dwmExtendFrameIntoClientArea = reinterpret_cast<t_DwmExtendFrameIntoClientArea>(GetProcAddress (module, DWM_EXTEND_FRAME_INTO_CLIENT_AREA));
  dwmGetColorizationColor = reinterpret_cast<t_DwmGetColorizationColor>(GetProcAddress (module, DWM_GET_COLORIZATION_COLOR));
  dwmGetWindowAttribute = reinterpret_cast<t_DwmGetWindowAttribute>(GetProcAddress (module, DWM_GET_WINDOW_ATTRIBUTE));
  dwmIsCompositionEnabled = reinterpret_cast<t_DwmIsCompositionEnabled>(GetProcAddress (module, DWM_IS_COMPOSITION_ENABLED));
  dwmSetWindowAttribute = reinterpret_cast<t_DwmSetWindowAttribute>(GetProcAddress (module, DWM_SET_WINDOW_ATTRIBUTE));
  dwmSetWindowThemeNonClientAttributes = reinterpret_cast<t_DwmSetWindowThemeNonClientAttributes>(GetProcAddress (module, DWM_SET_WINDOW_ATTRIBUTE));
  FreeLibrary(module);

  if(!(module = LoadLibraryA(USER_LIBRARY)))
    return;
  tSetWindowCompositionAttribute = reinterpret_cast<t_SetWindowCompositionAttribute>(GetProcAddress (module, DWM_SET_WINDOW_COMPOSITION_ATTRIBUTE));
  FreeLibrary(module);
}
//---------------------------------------------------------------------------
#endif // DWM_H
