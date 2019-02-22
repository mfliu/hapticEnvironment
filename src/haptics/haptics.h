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
};

void initHaptics(void);
void updateHaptics(void);


#endif
