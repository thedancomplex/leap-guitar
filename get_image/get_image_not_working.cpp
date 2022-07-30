#include <iostream>
#include <cstring>
#include "Leap.h"
#include "get_image.h"

using namespace Leap;

class SampleListener : public Listener {
  public:
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
    virtual void onDeviceChange(const Controller&);
    virtual void onServiceConnect(const Controller&);
    virtual void onServiceDisconnect(const Controller&);

  private:
};

const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
const std::string boneNames[] = {"Metacarpal", "Proximal", "Middle", "Distal"};
const std::string stateNames[] = {"STATE_INVALID", "STATE_START", "STATE_UPDATE", "STATE_END"};

void SampleListener::onInit(const Controller& controller) {
  std::cout << "Initialized" << std::endl;
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();

/*
  std::cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", extended fingers: " << frame.fingers().extended().count()
            << ", tools: " << frame.tools().count()
            << ", gestures: " << frame.gestures().count() << std::endl;
*/
  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
    // Get the first hand
    const Hand hand = *hl;
    std::string handType = hand.isLeft() ? "Left hand" : "Right hand";

//    std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
//              << ", palm position: " << hand.palmPosition() << std::endl;
    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

/*
    // Calculate the hand's pitch, roll, and yaw angles
    std::cout << std::string(2, ' ') <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
              << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
              << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;

    // Get the Arm bone
    Arm arm = hand.arm();
    std::cout << std::string(2, ' ') <<  "Arm direction: " << arm.direction()
              << " wrist position: " << arm.wristPosition()
              << " elbow position: " << arm.elbowPosition() << std::endl;

*/
    // Get fingers
    double xyz_index[]  = {0.0, 0.0, 0.0};
    double xyz_middle[] = {0.0, 0.0, 0.0};
    double xyz_ring[]   = {0.0, 0.0, 0.0};
    double xyz_pinky[]  = {0.0, 0.0, 0.0};
    double xyz_thumb[]  = {0.0, 0.0, 0.0};
    double tip_dist_thumb_index  = 0.0;
    double tip_dist_thumb_middle = 0.0;
    double tip_dist_thumb_ring   = 0.0;
    double tip_dist_thumb_pinky  = 0.0;
//const std::string fingerNames[] = {"Thumb", "Index", "Middle", "Ring", "Pinky"};
    const FingerList fingers = hand.fingers();
    display_image();
    for (FingerList::const_iterator fl = fingers.begin(); fl != fingers.end(); ++fl) {
      const Finger finger = *fl;
 
/*
      std::cout << std::string(4, ' ') <<  fingerNames[finger.type()]
                << " finger, id: " << finger.id()
                << ", length: " << finger.length()
                << "mm, width: " << finger.width() << std::endl;
*/
      // Get finger bones
      for (int b = 0; b < 4; ++b) {
        Bone::Type boneType = static_cast<Bone::Type>(b);
        Bone bone = finger.bone(boneType);
	if ( 
	    /* Gets the end of the finger */
	    (boneNames[boneType].compare("Distal") == 0) 
	   ) {

            if (
	    /* Gets the pinky finger */
	    (fingerNames[finger.type()].compare("Pinky") == 0)
	    ) {  
            xyz_pinky[0] = bone.nextJoint()[0];
            xyz_pinky[1] = bone.nextJoint()[1];
            xyz_pinky[2] = bone.nextJoint()[2];
	    }

            if (
	    /* Gets the ring finger */
	    (fingerNames[finger.type()].compare("Ring") == 0)
	    ) {  
            xyz_ring[0] = bone.nextJoint()[0];
            xyz_ring[1] = bone.nextJoint()[1];
            xyz_ring[2] = bone.nextJoint()[2];
	    }


            if (
	    /* Gets the middle finger */
	    (fingerNames[finger.type()].compare("Middle") == 0)
	    ) {  
            xyz_middle[0] = bone.nextJoint()[0];
            xyz_middle[1] = bone.nextJoint()[1];
            xyz_middle[2] = bone.nextJoint()[2];
	    }

            if (
	    /* Gets the index finger */
	    (fingerNames[finger.type()].compare("Index") == 0)
	    ) {  
            xyz_index[0] = bone.nextJoint()[0];
            xyz_index[1] = bone.nextJoint()[1];
            xyz_index[2] = bone.nextJoint()[2];
	    }

	    if (
	    (fingerNames[finger.type()].compare("Thumb") == 0)
	    ) {  
            xyz_thumb[0] = bone.nextJoint()[0];
            xyz_thumb[1] = bone.nextJoint()[1];
            xyz_thumb[2] = bone.nextJoint()[2];
	    }
	}


      }
    }
	for (int i = 0; i < 3; i++) tip_dist_thumb_index  += (xyz_index[i]  - xyz_thumb[i]) * (xyz_index[i]  - xyz_thumb[i]);
	for (int i = 0; i < 3; i++) tip_dist_thumb_middle += (xyz_middle[i] - xyz_thumb[i]) * (xyz_middle[i] - xyz_thumb[i]);
	for (int i = 0; i < 3; i++) tip_dist_thumb_ring   += (xyz_ring[i]   - xyz_thumb[i]) * (xyz_ring[i]   - xyz_thumb[i]);
	for (int i = 0; i < 3; i++) tip_dist_thumb_pinky  += (xyz_pinky[i]  - xyz_thumb[i]) * (xyz_pinky[i]  - xyz_thumb[i]);


	tip_dist_thumb_index  = sqrt(tip_dist_thumb_index);
	tip_dist_thumb_middle = sqrt(tip_dist_thumb_middle);
	tip_dist_thumb_ring   = sqrt(tip_dist_thumb_ring);
	tip_dist_thumb_pinky  = sqrt(tip_dist_thumb_pinky);
    std::cout << tip_dist_thumb_index  << ", "
	      << tip_dist_thumb_middle << ", "
	      << tip_dist_thumb_ring   << ", "
	      << tip_dist_thumb_pinky
	      << std::endl;
  }
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}

void SampleListener::onDeviceChange(const Controller& controller) {
  std::cout << "Device Changed" << std::endl;
  const DeviceList devices = controller.devices();

  for (int i = 0; i < devices.count(); ++i) {
    std::cout << "id: " << devices[i].toString() << std::endl;
    std::cout << "  isStreaming: " << (devices[i].isStreaming() ? "true" : "false") << std::endl;
  }
}

void SampleListener::onServiceConnect(const Controller& controller) {
  std::cout << "Service Connected" << std::endl;
}

void SampleListener::onServiceDisconnect(const Controller& controller) {
  std::cout << "Service Disconnected" << std::endl;
}

int main(int argc, char** argv) {
  // Create a sample listener and controller
  SampleListener listener;
  Controller controller;

  // Have the sample listener receive events from the controller
  controller.addListener(listener);

  if (argc > 1 && strcmp(argv[1], "--bg") == 0)
    controller.setPolicy(Leap::Controller::POLICY_BACKGROUND_FRAMES);

  // Keep this process running until Enter is pressed
  std::cout << "Press Enter to quit..." << std::endl;
  std::cin.get();

  // Remove the sample listener when done
  controller.removeListener(listener);

  return 0;
}
