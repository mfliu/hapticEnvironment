#include "controller.h"

/**
 * @file controller.h
 * @file controller.cpp 
 * @brief This file contains the main function that runs the haptic, graphics, and message listening
 * loops.
 *
 * This file is in the global namespace. It accesses haptic data and graphics data through external
 * structs, and keeps track of all the messaging sockets and Chai3D threads through the ControlData
 * extern, which is accessible in other files. 
 */


extern HapticData hapticsData;
extern GraphicsData graphicsData;
ControlData controlData;

int main(int argc, char* argv[])
{
  cout << endl;
  cout << "-----------------------------------" << endl;
  cout << "CHAI3D" << endl;
  cout << "-----------------------------------" << endl << endl << endl;
  cout << "Keyboard Options:" << endl << endl;
  cout << "[f] - Enable/Disable full screen mode" << endl;
  cout << "[q] - Exit application" << endl;
  cout << endl << endl;
  
  controlData.simulationRunning = false;
  controlData.simulationFinished = true;
  controlData.hapticsUp = false;
  controlData.listenerUp = false;
  controlData.streamerUp = false;
  controlData.loggingData = false;

  // TODO: Set these IP addresses from a config file
  //controlData.LISTENER_IP = "127.0.0.1";
  //controlData.LISTENER_PORT = 7000;
  controlData.MODULE_NUM = 1;
  controlData.IPADDR = "127.0.0.1";
  controlData.PORT = 7000;
  controlData.client = new rpc::client("127.0.0.1", 8080);
  //controlData.SENDER_IPS.push_back("127.0.0.1");
  //controlData.SENDER_IPS.push_back("127.0.0.1");
  //controlData.SENDER_PORTS.push_back(9000);
  //controlData.SENDER_PORTS.push_back(10000);
  controlData.hapticsOnly = false;
  
  if (controlData.hapticsOnly == false) {
    initDisplay();
    initScene();
  }
  initHaptics();
  startHapticsThread(); 
  atexit(close);
  resizeWindowCallback(graphicsData.window, graphicsData.width, graphicsData.height);
  sleep(2); 
  openMessagingSocket();
  int addSuccess = addMessageHandlerModule();
  if (addSuccess == 0) {
    cout << "Module addition failed" << endl;
    close();
    exit(1);
  }
  sleep(1);
  int subscribeSuccess = subscribeToTrialControl();
  if (subscribeSuccess == 0) {
    cout << "Subcribe to Trial Control failed" << endl;
    close();
    exit(1);
  }
  sleep(2);
  startStreamer(); 
  startListener();
  cout << "streamer and listener started" << endl;
  
  while (!glfwWindowShouldClose(graphicsData.window)) {
    glfwGetWindowSize(graphicsData.window, &graphicsData.width, &graphicsData.height);
    graphicsData.graphicsClock = clock();
    updateGraphics();
    glfwSwapBuffers(graphicsData.window);
    glfwPollEvents();
    graphicsData.freqCounterGraphics.signal(1);
  }
  glfwDestroyWindow(graphicsData.window);
  glfwTerminate();
  return(0);
}

/**
 * Checks if the haptics and messaging threads have exited yet. The graphics loop is in main, and
 * exits when all other threads are down, so it is not checked here.
 */
bool allThreadsDown()
{
  return (controlData.hapticsUp && controlData.listenerUp && controlData.streamerUp);  
}

/**
 * Ends the program. This method does so by setting the "simulationRunning" boolean to false. When
 * false, other threads will exit. To exit gracefully, this method waits until all threads have
 * returned before stopping the haptic tool and exiting the graphic interface.
 */
void close()
{
  controlData.simulationRunning = false;
  while (!controlData.simulationFinished) {
    controlData.simulationFinished = allThreadsDown();
    cSleepMs(100);
  }
  hapticsData.tool->stop();
  cout << "Haptic tool stopped" << endl;
  delete hapticsData.hapticsThread;
  cout << "Deleted haptics thread" << endl;
  delete graphicsData.world;
  cout << "Deleted world" << endl;
  delete hapticsData.handler;
  cout << "Deleted handler" << endl;
  closeMessagingSocket();
}

/**
 * This function receives packets from the listener threads and updates the haptic environment
 * variables accordingly.
 * @param packet is a pointer to a char array of bytes
 */
void parsePacket(char* packet)
{
  MSG_HEADER header;
  memcpy(&header, packet, sizeof(header));
  int msgType = header.msg_type;
  switch (msgType)
  {
    case SESSION_START:
    {
      cout << "Received SESSION_START Message" << endl;
      break;
    }

    case SESSION_END:
    {
      cout << "Received SESSION_END Message" << endl;
      controlData.simulationRunning = false;
      close();
      break;
    }

    case TRIAL_START:
    {
      cout << "Received TRIAL_START Message" << endl;
      break;
    }

    case TRIAL_END:
    {
      cout << "Received TRIAL_END Message" << endl;
      break;
    }

    case START_RECORDING:
    {
      cout << "Received START_RECORDING Message" << endl;
      M_START_RECORDING recInfo;
      memcpy(&recInfo, packet, sizeof(recInfo));
      char* fileName;
      fileName = recInfo.filename;
      controlData.dataFile.open(fileName, ofstream::binary);
      controlData.dataFile.flush();
      controlData.loggingData = true;
      break; 
    }

    case STOP_RECORDING:
    {
      cout << "Received STOP_RECORDING Message" << endl;
      controlData.dataFile.close();
      controlData.loggingData = false;
      break;
    }
    
    case REMOVE_OBJECT:
    {
      cout << "Received REMOVE_OBJECT Message" << endl;
      M_REMOVE_OBJECT rmObj;
      memcpy(&rmObj, packet, sizeof(rmObj));
      cGenericObject* objPtr = controlData.objectMap[rmObj.objectName];
      graphicsData.world->deleteChild(objPtr);
      controlData.objectMap.erase(rmObj.objectName);
      break;
    }
    case CST_CREATE:
    {
      cout << "Received CST_CREATE Message" << endl;
      M_CST_CREATE cstObj;
      memcpy(&cstObj, packet, sizeof(cstObj));
      cCST* cst = new cCST(graphicsData.world, cstObj.lambdaVal, 
          cstObj.forceMagnitude, cstObj.visionEnabled, cstObj.hapticEnabled);
      char* cstName = cstObj.cstName;
      controlData.objectMap[cstName] = cst;
      graphicsData.movingObjects.push_back(cst);
      graphicsData.world->addEffect(cst);
      controlData.worldEffects[cstName] = cst;
      break;
    }
    case CST_DESTRUCT:
    {
      cout << "Received CST_DESTRUCT Message" << endl;
      M_CST_DESTRUCT cstObj;
      memcpy(&cstObj, packet, sizeof(cstObj));
      if (controlData.objectMap.find(cstObj.cstName) == controlData.objectMap.end()) {
        cout << cstObj.cstName << " not found" << endl;
      }
      else {
        cCST* cst = dynamic_cast<cCST*>(controlData.objectMap[cstObj.cstName]);
        cst->destructCST();
        remove(graphicsData.movingObjects.begin(), graphicsData.movingObjects.end(), cst);
        controlData.objectMap.erase(cstObj.cstName);
        graphicsData.world->deleteChild(cst);
      }
      break;
    }
    case CST_START:
    {
      cout << "Received CST_START Message" << endl;
      M_CST_START cstObj;
      memcpy(&cstObj, packet, sizeof(cstObj));
      cCST* cst = dynamic_cast<cCST*>(controlData.objectMap[cstObj.cstName]);
      hapticsData.tool->setShowEnabled(false);
      cst->startCST();
      break;
    }
    case CST_STOP:
    {
      cout << "Received CST_STOP Message" << endl;
      M_CST_STOP cstObj;
      memcpy(&cstObj, packet, sizeof(cstObj));
      cCST* cst = dynamic_cast<cCST*>(controlData.objectMap[cstObj.cstName]);
      cst->stopCST();
      hapticsData.tool->setShowEnabled(true);
      break;
    }
    case CST_SET_VISUAL:
    {
      cout << "Received CST_SET_VISUAL Message" << endl;
      M_CST_SET_VISUAL cstObj;
      memcpy(&cstObj, packet, sizeof(cstObj));
      bool visual = cstObj.visionEnabled;
      cCST* cst = dynamic_cast<cCST*>(controlData.objectMap[cstObj.cstName]);
      cst->setVisionEnabled(visual);
      break;
    }
    case CST_SET_HAPTIC:
    {
      cout << "Received CST_SET_HAPTIC Message" << endl;
      M_CST_SET_HAPTIC cstObj;
      memcpy(&cstObj, packet, sizeof(cstObj));
      bool haptic = cstObj.hapticEnabled;
      cCST* cst = dynamic_cast<cCST*>(controlData.objectMap[cstObj.cstName]);
      cst->setHapticEnabled(haptic);
      break;
    }
    case CST_SET_LAMBDA:
    {
      cout << "Received CST_SET_LAMBDA Message" << endl;
      M_CST_SET_LAMBDA cstObj;
      memcpy(&cstObj, packet, sizeof(cstObj));
      double lambda = cstObj.lambdaVal;
      cCST* cst = dynamic_cast<cCST*>(controlData.objectMap[cstObj.cstName]);
      cst->setLambda(lambda);
      break;
    }
    case HAPTICS_SET_ENABLED:
    {
      cout << "Received HAPTICS_SET_ENABLED Message" << endl;
      M_HAPTICS_SET_ENABLED hapticsEnabled;
      memcpy(&hapticsEnabled, packet, sizeof(hapticsEnabled));
      char* objectName;
      objectName = hapticsEnabled.objectName;
      if (controlData.objectMap.find(objectName) == controlData.objectMap.end()) {
        cout << objectName << " not found" << endl;
      }
      else {
        if (hapticsEnabled.enabled == 1) {
          controlData.objectMap[objectName]->setHapticEnabled(true);
        }
        else if (hapticsEnabled.enabled == 0) {
          controlData.objectMap[objectName]->setHapticEnabled(false);
        }
      }
      break;
    }
    case HAPTICS_SET_STIFFNESS:
    {
      cout << "Received HAPTICS_SET_STIFFNESS Message" << endl;
      M_HAPTICS_SET_STIFFNESS stiffness;
      memcpy(&stiffness, packet, sizeof(stiffness));
      char* objectName;
      objectName = stiffness.objectName;
      if (controlData.objectMap.find(objectName) == controlData.objectMap.end()) {
        cout << objectName << " not found" << endl;
      }
      else {
        controlData.objectMap[objectName]->m_material->setStiffness(stiffness.stiffness);
      }
      break;
    }

    case HAPTICS_BOUNDING_PLANE:
    {
      cout << "Received HAPTICS_BOUNDING_PLANE Message" << endl;
      M_HAPTICS_BOUNDING_PLANE bpMsg;
      memcpy(&bpMsg, packet, sizeof(bpMsg));
      double bWidth = bpMsg.bWidth;
      double bHeight = bpMsg.bHeight;
      int stiffness = hapticsData.hapticDeviceInfo.m_maxLinearStiffness;
      double toolRadius = hapticsData.toolRadius;
      cBoundingPlane* bp = new cBoundingPlane(stiffness, toolRadius, bWidth, bHeight);
      graphicsData.world->addChild(bp->getLowerBoundingPlane());
      graphicsData.world->addChild(bp->getUpperBoundingPlane());
      graphicsData.world->addChild(bp->getTopBoundingPlane());
      graphicsData.world->addChild(bp->getBottomBoundingPlane());
      graphicsData.world->addChild(bp->getLeftBoundingPlane());
      graphicsData.world->addChild(bp->getRightBoundingPlane());
      controlData.objectMap["boundingPlane"] = bp;
      break;
    }
    
    case HAPTICS_CONSTANT_FORCE_FIELD:
    {
      cout << "Received HAPTICS_CONSTANT_FORCE_FIELD Message" << endl;
      M_HAPTICS_CONSTANT_FORCE_FIELD cffInfo;
      memcpy(&cffInfo, packet, sizeof(cffInfo));
      double d = cffInfo.direction;
      double m = cffInfo.magnitude;
      cConstantForceFieldEffect* cFF = new cConstantForceFieldEffect(graphicsData.world, d, m);
      graphicsData.world->addEffect(cFF);
      controlData.worldEffects[cffInfo.effectName] = cFF;
      break;
    }

    case HAPTICS_VISCOSITY_FIELD:
    {
      cout << "Received HAPTICS_VISCOSITY_FIELD Message" << endl;
      M_HAPTICS_VISCOSITY_FIELD vF;
      memcpy(&vF, packet, sizeof(vF));
      cMatrix3d* B = new cMatrix3d(vF.viscosityMatrix[0], vF.viscosityMatrix[1], vF.viscosityMatrix[2],
                                   vF.viscosityMatrix[3], vF.viscosityMatrix[4], vF.viscosityMatrix[5],
                                   vF.viscosityMatrix[6], vF.viscosityMatrix[7], vF.viscosityMatrix[8]);
      cViscosityEffect* vFF = new cViscosityEffect(graphicsData.world, B);
      graphicsData.world->addEffect(vFF);
      controlData.worldEffects[vF.effectName] = vFF;
      break;
    }
    
    case HAPTICS_FREEZE_EFFECT:
    {
      cout << "Received HAPTICS_FREEZE_EFFECT Message" << endl;
      M_HAPTICS_FREEZE_EFFECT freeze;
      memcpy(&freeze, packet, sizeof(freeze));
      double workspaceScaleFactor = hapticsData.tool->getWorkspaceScaleFactor();
      double maxStiffness = 1.5*hapticsData.hapticDeviceInfo.m_maxLinearStiffness/workspaceScaleFactor;
      cVector3d currentPos = hapticsData.tool->getDeviceGlobalPos();
      cFreezeEffect* freezeEff = new cFreezeEffect(graphicsData.world, maxStiffness, currentPos);
      graphicsData.world->addEffect(freezeEff);
      controlData.worldEffects[freeze.effectName] = freezeEff;
      break;  
    }

    case HAPTICS_REMOVE_WORLD_EFFECT:
    {
      cout << "Received HAPTICS_REMOVE_FIELD_EFFECT Message" << endl;
      M_HAPTICS_REMOVE_WORLD_EFFECT rmField;
      memcpy(&rmField, packet, sizeof(rmField));
      cGenericEffect* fieldEffect = controlData.worldEffects[rmField.effectName];
      graphicsData.world->removeEffect(fieldEffect);
      controlData.worldEffects.erase(rmField.effectName);
      break;
    }

    case GRAPHICS_SET_ENABLED:
    {
      cout << "Received GRAPHICS_SET_ENABLED Message" << endl;
      M_GRAPHICS_SET_ENABLED graphicsEnabled;
      memcpy(&graphicsEnabled, packet, sizeof(graphicsEnabled));
      char* objectName;
      objectName = graphicsEnabled.objectName;
      int enabled = graphicsEnabled.enabled;
      if (controlData.objectMap.find(objectName) == controlData.objectMap.end()) {
        cout << objectName << " not found" << endl;
      }
      else {
        if (graphicsEnabled.enabled == 1) {
          controlData.objectMap[objectName]->setShowEnabled(true);
        }
        else if (graphicsEnabled.enabled == 0) {
          controlData.objectMap[objectName]->setShowEnabled(false);
        }
      }
      break;
    }
    
    case GRAPHICS_CHANGE_BG_COLOR:
    {
      cout << "Received GRAPHICS_CHANGE_BG_COLOR Message" << endl;
      M_GRAPHICS_CHANGE_BG_COLOR bgColor;
      memcpy(&bgColor, packet, sizeof(bgColor));
      float red = bgColor.color[0]/250.0;
      float green = bgColor.color[1]/250.0;
      float blue = bgColor.color[2]/250.0;
      graphicsData.world->setBackgroundColor(red, green, blue);
      break;
    }
    
    case GRAPHICS_PIPE:
    {
      cout << "Received GRAPHICS_PIPE Message" << endl;
      M_GRAPHICS_PIPE pipe;
      memcpy(&pipe, packet, sizeof(pipe));
      cVector3d* position = new cVector3d(pipe.position[0], pipe.position[1], pipe.position[2]);
      cMatrix3d* rotation = new cMatrix3d(pipe.rotation[0], pipe.rotation[1], pipe.rotation[2],
                                          pipe.rotation[3], pipe.rotation[4], pipe.rotation[5],
                                          pipe.rotation[6], pipe.rotation[7], pipe.rotation[8]);
      cColorf* color = new cColorf(pipe.color[0], pipe.color[1], pipe.color[2], pipe.color[3]);
      cPipe* myPipe = new cPipe(pipe.height, pipe.innerRadius, pipe.outerRadius, pipe.numSides, 
                                pipe.numHeightSegments, position, rotation, color);
      controlData.objectMap[pipe.objectName] = myPipe->getPipeObj();
      graphicsData.world->addChild(myPipe->getPipeObj());
      break;
    }

    case GRAPHICS_ARROW:
    {
      cout << "Received GRAPHICS_ARROW Message" << endl;
      M_GRAPHICS_ARROW arrow;
      memcpy(&arrow, packet, sizeof(arrow));
      cVector3d* direction = new cVector3d(arrow.direction[0], arrow.direction[1], arrow.direction[2]);
      cVector3d* position = new cVector3d(arrow.position[0], arrow.position[1], arrow.position[2]);
      cColorf* color = new cColorf(arrow.color[0], arrow.color[1], arrow.color[2], arrow.color[3]);
      cArrow* myArrow = new cArrow(arrow.aLength, arrow.shaftRadius, arrow.lengthTip, arrow.radiusTip,
                                    arrow.bidirectional, arrow.numSides, direction, position, color);
      controlData.objectMap[arrow.objectName] = myArrow->getArrowObj();
      graphicsData.world->addChild(myArrow->getArrowObj());
      break;
    }
    
    case GRAPHICS_CHANGE_OBJECT_COLOR:
    {
      cout << "Received GRAPHICS_CHANGE_OBJECT_COLOR Message" << endl;
      M_GRAPHICS_CHANGE_OBJECT_COLOR color;
      memcpy(&color, packet, sizeof(color));
      cGenericObject* obj = controlData.objectMap[color.objectName];
      obj->m_material->setColorf(color.color[0], color.color[1], color.color[2], color.color[3]);
      break;
    }
    case GRAPHICS_MOVING_DOTS:
    {
      cout << "Received GRAPHICS_MOVING_DOTS Message" << endl;
      cMultiPoint* test = new cMultiPoint();
      M_GRAPHICS_MOVING_DOTS dots;
      memcpy(&dots, packet, sizeof(dots));
      char* objectName;
      objectName = dots.objectName;
      cMovingDots* md = new cMovingDots(dots.numDots, dots.coherence, dots.direction, dots.magnitude);
      controlData.objectMap[objectName] = md;
      graphicsData.movingObjects.push_back(md);
      graphicsData.world->addChild(md->getMovingPoints());
      graphicsData.world->addChild(md->getRandomPoints());
      break;
    }
    case GRAPHICS_SHAPE_BOX:
    {
      cout << "Received GRAPHICS_SHAPE_BOX Message" << endl;
      M_GRAPHICS_SHAPE_BOX box;
      memcpy(&box, packet, sizeof(box));
      cShapeBox* boxObj = new cShapeBox(box.sizeX, box.sizeY, box.sizeZ);
      boxObj->setLocalPos(box.localPosition[0], box.localPosition[1], box.localPosition[2]);
      boxObj->m_material->setColorf(box.color[0], box.color[1], box.color[2], box.color[3]);
      controlData.objectMap[box.objectName] = boxObj;
      graphicsData.world->addChild(boxObj);
      break;
    }
    case GRAPHICS_SHAPE_SPHERE: 
    {
      cout << "Received GRAPHICS_SHAPE_SPHERE Message" << endl;
      M_GRAPHICS_SHAPE_SPHERE sphere;
      memcpy(&sphere, packet, sizeof(sphere));
      cShapeSphere* sphereObj = new cShapeSphere(sphere.radius);
      graphicsData.world->addChild(sphereObj);
      sphereObj->setLocalPos(sphere.localPosition[0], sphere.localPosition[1], sphere.localPosition[2]);
      sphereObj->m_material->setColorf(sphere.color[0], sphere.color[1], sphere.color[2], sphere.color[3]);
      cEffectSurface* sphereEffect = new cEffectSurface(sphereObj);
      sphereObj->addEffect(sphereEffect);
      controlData.objectMap[sphere.objectName] = sphereObj;
      break;
    }
    case GRAPHICS_SHAPE_TORUS:
    {
      cout << "Received GRAPHICS_SHAPE_TORUS Message" << endl;
      M_GRAPHICS_SHAPE_TORUS torus;
      memcpy(&torus, packet, sizeof(torus));
      cShapeTorus* torusObj = new cShapeTorus(torus.innerRadius, torus.outerRadius);
      graphicsData.world->addChild(torusObj);
      torusObj->setLocalPos(0.0, 0.0, 0.0);
      torusObj->m_material->setStiffness(1.0);
      torusObj->m_material->setColorf(255.0, 255.0, 255.0, 1.0);
      cEffectSurface* torusEffect = new cEffectSurface(torusObj);
      torusObj->addEffect(torusEffect);
      controlData.objectMap[torus.objectName] = torusObj;
      break; 
    }
  }
}
