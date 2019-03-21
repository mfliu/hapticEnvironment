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

#define HAPTIC_DEVICE_ANGLE_DEG 45.0
#define GAIN_HAPTIC_TO_RIG 1000.0
#define HAPTIC_WORKSPACE_ANGLE_DEG 0.00
#define WORKSPACE_WIDTH 250
#define WORKSPACE_HEIGHT 250 
#define DRAG_COEFF .02
#define HAPTIC_TOOL_RADIUS 0.04

void initHaptics(void);
void startHapticsThread(void);
void updateHaptics(void);


// ---------------------------------------------------- //
// -----------Custom Haptics Functionality------------- // 
// ---------------------------------------------------- // 

#include "cBoundingPlane.h"
#include "cConstantForceFieldEffect.h"
#endif
