#pragma once

#include "DBOXApp.h"
#include "DBOX3D.h"
#include "DBOX2D.h"
#include "DBOXFont.h"
#include "DBOXInput.h"
#include "DBOXTimer.h"
#include "DBOXCamera.h"
#include "DBOXMath.h"

#include "Animation2D.h"

#include <vector>

extern LPDBOX3D Engine3D;
extern LPDBOX2D Engine2D;
extern LPDBOXINPUT Input;
extern DBOXTimer Timer;

class Enitity;
class PlayerObj;
class PlatformObj;

extern std::vector<Enitity*> ObjBuff;

extern PlayerObj* pPlayer;

extern Animation2D PlayerLegsWalk,
					PlayerTorsoIdle,
					PlayerLegsIdle,
					PlatformIdle,
					BulletIdle;