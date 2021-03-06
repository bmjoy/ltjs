#ifndef __WON_MEDIAMETRIX_H__
#define __WON_HEDIAMETRIX_H__
#include "wonshared.h"
#include "woncommon/platform.h"
#include <string>

namespace WONAPI
{

#ifdef WIN32
void CreateMediaMetrixEditControl(HWND theParent);
void FillMediaMetrixEditControl(const char *theURL);
void ClearMediaMetrixEditControl();
void DestroyMediaMetrixEditControl();
#endif

};

#endif
