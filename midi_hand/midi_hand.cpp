#include <iostream>
#include <cstring>
#include "Leap.h"
//#include "midi_hand.h"


int midi_send();
int midi_init();
int midi_cleanup();
int midi_volume(double val);
int midi_note_cont(double val);
int midi_pitch_bend(double val);

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
  midi_init();
}

void SampleListener::onConnect(const Controller& controller) {
  std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
  //midi_init();
  std::cout << "done midi init" << std::endl;
}

void SampleListener::onDisconnect(const Controller& controller) {
  // Note: not dispatched when running in a debugger.
  std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {

  double deg = 0.0;
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();
  std::cout << "Frame id: " << frame.id()
            << ", timestamp: " << frame.timestamp()
            << ", hands: " << frame.hands().count()
            << ", extended fingers: " << frame.fingers().extended().count()
            << ", tools: " << frame.tools().count()
            << ", gestures: " << frame.gestures().count() << std::endl;

  HandList hands = frame.hands();
  for (HandList::const_iterator hl = hands.begin(); hl != hands.end(); ++hl) {
    // Get the first hand
    const Hand hand = *hl;
    std::string handType = hand.isLeft() ? "Left hand" : "Right hand";
    std::cout << std::string(2, ' ') << handType << ", id: " << hand.id()
              << ", palm position: " 
	      << " p[0] = " << hand.palmPosition()[0]
	      << " p[1] = " << hand.palmPosition()[1]
	      << " p[2] = " << hand.palmPosition()[2]
	      << std::endl;
              // << ", palm position: " << hand.palmPosition() << std::endl;
    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();
    deg = direction.pitch() * RAD_TO_DEG;

    // Calculate the hand's pitch, roll, and yaw angles
    std::cout << std::string(2, ' ') 
	      <<  "pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
              << "roll: "   << normal.roll()     * RAD_TO_DEG << " degrees, "
              << "yaw: "    << direction.yaw()   * RAD_TO_DEG << " degrees" 
	      << std::endl;
    }
  double val = deg / 180.0;
  midi_pitch_bend(val*2);
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
// midiout.cpp
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "RtMidi.h"
RtMidiOut *midiout = new RtMidiOut();

int midi_init()
{
  // Check available ports.
  unsigned int nPorts = midiout->getPortCount();
  if ( nPorts == 0 ) {
    std::cout << "No ports available!\n";
    midi_cleanup();
  }
  // Open first available port.
  midiout->openPort( 0 );
  return 0;
}

int midi_note_cont(double val)
{
  std::vector<unsigned char> message;
  // Send out a series of MIDI messages.
  if( val < 0.0 ) val = 0.0;
  else if( val > 1.0 ) val = 1.0;
  int vali =(int)(val * 127.0);
  if( vali < 0 ) vali = 0;
  else if( vali > 127 ) vali = 127;
  message.push_back(0x90);
  message.push_back(vali);
  message.push_back( 100 );
  midiout->sendMessage( &message );
  message.push_back(0x80);
  message.push_back(vali);
  message.push_back( 100 );
  midiout->sendMessage( &message );
  return 0;
}

#define MIDI_PITCH_BEND_MAX 16384
#define MIDI_PITCH_BEND_MIN 0
#define MIDI_PITCH_BEND_CENTER 8192

int midi_pitch_bend(double val)
{
  std::vector<unsigned char> message;
  // Send out a series of MIDI messages.
  double val2 = val;
  if( val < -1.0 ) val = -1.0;
  else if( val > 1.0 ) val = 1.0;
  val = (val + 1.0) / 2.0;

  int vali =(int)(val * MIDI_PITCH_BEND_MAX);
  if( vali < MIDI_PITCH_BEND_MIN ) vali = MIDI_PITCH_BEND_MIN;
  else if( vali > MIDI_PITCH_BEND_MAX ) vali = MIDI_PITCH_BEND_MAX;

  int mLSB = vali & 0x7F;
  int mMSB = (vali >> 7) & 0x7F;
  int vali2 = 128*mMSB + mLSB;

  std::cout << val2 << " - " << vali << " - " << vali2 << " -- " << mLSB << " " << mMSB << std::endl;

  message.push_back(0xE0);
  message.push_back(mLSB);
  message.push_back(mMSB );
  midiout->sendMessage( &message );
  return 0;
}

int midi_volume(double val)
{
  std::vector<unsigned char> message;
  // Send out a series of MIDI messages.
  // Control Change: 176, 7, 100 (volume)
  if( val < 0.0 ) val = 0.0;
  else if( val > 1.0 ) val = 1.0;
  int vali =(int)(val * 100.0);
  if( vali < 0 ) vali = 0;
  else if( vali > 100 ) vali = 100;
  message.push_back(176);
  message.push_back(1);
  message.push_back( vali );
  midiout->sendMessage( &message );
  return 0;
}

int midi_send()
{
  std::vector<unsigned char> message;
  // Send out a series of MIDI messages.
  // Program change: 192, 5
  message.push_back( 192 );
  message.push_back( 5 );
  midiout->sendMessage( &message );
  // Control Change: 176, 7, 100 (volume)
  message[0] = 176;
  message[1] = 7;
  message.push_back( 100 );
  midiout->sendMessage( &message );
  // Note On: 144, 64, 90
  message[0] = 144;
  message[1] = 64;
  message[2] = 90;
  midiout->sendMessage( &message );
  usleep( 500000 ); // Platform-dependent ... see example in tests directory.
  // Note Off: 128, 64, 40
  message[0] = 128;
  message[1] = 64;
  message[2] = 40;
  midiout->sendMessage( &message );
  // Clean up
  return 0;
}

int midi_cleanup()
{
  delete midiout;
  return 0;
}
