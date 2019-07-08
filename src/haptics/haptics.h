#pragma once 

#ifndef _HAPTICS_H_INCLUDED_
#define _HAPTICS_H_INCLUDED_

#include <stdio.h>
#include "chai3d.h"
#include "graphics/graphics.h"
#include "core/controller.h"

using namespace chai3d;
using namespace std;

struct HapticData
{
  cHapticDeviceHandler* handler;
  cGenericHapticDevicePtr hapticDevice;
  cHapticDeviceInfo hapticDeviceInfo;
  cToolCursor* tool;
  cThread* hapticsThread;
  cFrequencyCounter freqCounterHaptics;
  double toolRadius;
};

#define HAPTIC_TOOL_RADIUS 2

void initHaptics(void);
void startHapticsThread(void);
void updateHaptics(void);


// ---------------------------------------------------- //
// -----------Custom Haptics Functionality------------- // 
// ---------------------------------------------------- // 

#include "cBoundingPlane.h"
#include "cConstantForceFieldEffect.h"
#include "cViscosityEffect.h"
#include "cFreezeEffect.h"
#include "cPositionForceFieldEffect.h"
#endif
