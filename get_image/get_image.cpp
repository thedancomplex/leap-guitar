1 #undef __cplusplus
 2
 3 #include <stdio.h>
 4 #include <stdlib.h>
 5 #include "LeapC.h"
 6 #include "ExampleConnection.h"
 7
 8 /** Callback for when the connection opens. */
 9 static void OnConnect(){
10   printf("Connected.\n");
11 }
12
13 /** Callback for when a device is found. */
14 static void OnDevice(const LEAP_DEVICE_INFO *props){
15   printf("Found device %s.\n", props->serial);
16 }
17
18 /** Callback for when a frame of tracking data is available. */
19 static void OnFrame(const LEAP_TRACKING_EVENT *frame){
20   printf("Frame %lli with %i hands.\n", (long long int)frame->info.frame_id, frame->nHands);
21   for(uint32_t h = 0; h < frame->nHands; h++){
22     LEAP_HAND* hand = &frame->pHands[h];
23     printf("    Hand id %i is a %s hand with position (%f, %f, %f).\n",
24                 hand->id,
25                 (hand->type == eLeapHandType_Left ? "left" : "right"),
26                 hand->palm.position.x,
27                 hand->palm.position.y,
28                 hand->palm.position.z);
29   }
30 }
31
32 /** Callback for when an image is available. */
33 static void OnImage(const LEAP_IMAGE_EVENT *imageEvent){
34     printf("Received image set for frame %lli with size %lli.\n",
35            (long long int)imageEvent->info.frame_id,
36            (long long int)imageEvent->image[0].properties.width*
37            (long long int)imageEvent->image[0].properties.height*2);
38 }
39
40 int main(int argc, char** argv) {
41   //Set callback function pointers
42   ConnectionCallbacks.on_connection          = &OnConnect;
43   ConnectionCallbacks.on_device_found        = &OnDevice;
44   ConnectionCallbacks.on_frame               = &OnFrame;
45   ConnectionCallbacks.on_image               = &OnImage;
46
47   LEAP_CONNECTION *connection = OpenConnection();
48   LeapSetPolicyFlags(*connection, eLeapPolicyFlag_Images, 0);
49
50   printf("Press Enter to exit program.\n");
51   getchar();
52   return 0;
53 }
54 //End-of-Sample.c
