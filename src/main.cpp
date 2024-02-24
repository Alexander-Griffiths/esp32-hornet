/* major librarys */
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <stdint.h>

/* additional librarys */
#include <SparkFun_Qwiic_OLED.h>

/* serial communications */
#define DEBUG_SERIAL Serial
#define BAUD_RATE 115200

QwiicMicroOLED myOLED;
const char * deviceName = "Micro OLED";


int yoffset;

void setup()
{
    delay(500);   //Give display time to power on
    Serial.begin(115200);

    Serial.println("\n\r-----------------------------------");

    Serial.print("Running Test #5 on: ");
    Serial.println(String(deviceName));

    if(!myOLED.begin()){

        Serial.println("- Device Begin Failed");
        while(1);
    }

    yoffset = (myOLED.getHeight() - myOLED.getFont()->height)/2;

    delay(1000);
}

// Our testing functions

void scroll_right(void){

    myOLED.scrollStop();
    myOLED.scrollRight(0, 7, SCROLL_INTERVAL_2_FRAMES); 
}

void scroll_right_vert(void){
    myOLED.scrollStop();    
    myOLED.scrollVertRight(0, 7, SCROLL_INTERVAL_3_FRAMES); 
}

void scroll_left(void){
    myOLED.scrollStop();    
    myOLED.scrollLeft(0, 7, SCROLL_INTERVAL_4_FRAMES);
}

void scroll_left_vert(void){
    myOLED.scrollStop();    
    myOLED.scrollVertLeft(0, 7, SCROLL_INTERVAL_5_FRAMES);
}

void scroll_stop(void){
    myOLED.scrollStop();
}

void flip_horz(void){

    for(int i=0; i < 6; i++){
        myOLED.flipHorizontal(!(i & 0x01));
        delay(800);
    }
}

void flip_vert(void){
    for(int i=0; i < 6; i++){
        myOLED.flipVertical(!(i & 0x01));
        delay(800);
    }
}

void invert(void){
    for(int i=0; i < 6; i++){
        myOLED.invert(!(i & 0x01));
        delay(800);
    }    
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Use an array of testing functions, with a title, to run the tests

typedef void (*testFn)(void);
typedef struct _testRoutines{
    void (*testFn)(void);
    const char *title;
}testRoutine;

static const testRoutine testFunctions[] = {
    {scroll_right, "Right>"},
    {scroll_right_vert, "^Right-Up>"},
    {scroll_left, "<Left"},
    {scroll_left_vert, "<Left-Up^"},
    {scroll_stop, "<STOP>"},
    {flip_horz, "-Flip-Horz-"},    
    {flip_vert, "|Flip-Vert|"},    
    {invert, "**INVERT**"}        
}; 

////////////////////////////////////////////////////////////////////////////////////////////////
// loop()
// 
// Standard Arduino loop routine
void loop()
{

    // loop over the test table entries, write title and run test.
    for(uint8_t i=0; i < sizeof(testFunctions)/sizeof(testFunctions[0]); i++){


        if(testFunctions[i].title){
            myOLED.erase();
            myOLED.text(3, yoffset, testFunctions[i].title);
            myOLED.display();
        }
        testFunctions[i].testFn();  // run test

        delay(3000);
    }
  
}