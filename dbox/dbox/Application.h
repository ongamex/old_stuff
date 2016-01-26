#pragma once

#define safe_release(p) if(p) { p->Release() ; p = NULL; }

#include "DBOXApp.h"
#include "DBOX3D.h"
#include "DBOX2D.h"
#include "DBOXFont.h"
#include "DBOXInput.h"
#include "DBOXTimer.h"
#include "DBOXCamera.h"
#include "DBOXMath.h"

#include <vector>

extern LPDBOX3D Engine3D;
extern LPDBOX2D Engine2D;
extern LPDBOXINPUT Input;


