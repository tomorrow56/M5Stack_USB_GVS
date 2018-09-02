 /*************************************************************
  This sketch is cutomized for M5Stack + USB Host Shield + FTDI.
  USB_Host_Shield_2.0: https://github.com/felis/USB_Host_Shield_2.0
 *************************************************************/

#include <M5Stack.h>
#include "M5StackUpdater.h"

#include "cdcftdimod.h"
#include <usbhub.h>

#include "pgmstrings.h"

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

class FTDIMODAsync : public FTDIMODAsyncOper
{
public:
    uint8_t OnInit(FTDIMOD *pftdi);
};

uint8_t FTDIMODAsync::OnInit(FTDIMOD *pftdi)
{
    uint8_t rcode = 0;

//    rcode = pftdi->SetBaudRate(115200);
    rcode = pftdi->SetBaudRate(9600);

    if (rcode)
    {
        ErrorMessage<uint8_t>(PSTR("SetBaudRate"), rcode);
        return rcode;
    }
    rcode = pftdi->SetFlowControl(FTDI_SIO_DISABLE_FLOW_CTRL);

    if (rcode)
        ErrorMessage<uint8_t>(PSTR("SetFlowControl"), rcode);

    return rcode;
}

USB              Usb;
//USBHub         Hub(&Usb);
FTDIMODAsync        FtdiAsync;
FTDIMOD             Ftdi(&Usb, &FtdiAsync);

// A few test variables used during debugging
boolean change_colour = 1;
boolean selected = 1;

// We have to blank the top line each time the display is scrolled, but this takes up to 13 milliseconds
// for a full width line, meanwhile the serial buffer may be filling... and overflowing
// We can speed up scrolling of short text lines by just blanking the character we drew
int blank[19]; // We keep all the strings pixel lengths to optimise the speed of the top line blanking

// GVS Logo
extern const unsigned char gImage_logoGVS[];

boolean InitFlag = false;

// for GVS

  char buffer[10];
  char ID = 1;
  int Dat = 0;
  char pole = 0;
  char A = '0';

void setup()
{
  // Setup the TFT display
  M5.begin();

  if(digitalRead(BUTTON_A_PIN) == 0) {
    Serial.println("Will Load menu binary");
    updateFromFS(SD);
    ESP.restart();
  }

  // Use GVS Logo
  M5.Lcd.pushImage(0, 0, 320, 240, (uint16_t *)gImage_logoGVS);

//  Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
  Serial.println("Start");

/*
  drawString(const char *string, int poX, int poY, int font),
  drawCentreString(const char *string, int dX, int poY, int font), // Deprecated, use setTextDatum() and drawString()
  drawRightString(const char *string, int dX, int poY, int font),  // Deprecated, use setTextDatum() and drawString()
 */

  if (Usb.Init() == -1){
    Serial.println("OSC did not start.");
  }

  // Change colour for scrolling zone text
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}

void loop()
{
    Usb.Task();

    if( Usb.getUsbTaskState() == USB_STATE_RUNNING ){
      uint8_t  rcode;

      if(InitFlag == false){
/*
        char strbuf[] = "R";
        //char strbuf[] = "The quick brown fox jumps over the lazy dog";
        //char strbuf[] = "This string contains 61 character to demonstrate FTDI buffers"; //add one symbol to it to see some garbage
        rcode = Ftdi.SndData(strlen(strbuf), (uint8_t*)strbuf);
        if (rcode){
          ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
        }
*/
        InitFlag = true;
        delay(50);
      }

      if(M5.BtnA.wasPressed()){
        M5.Lcd.fillRect(0,0,320,240, TFT_RED);
        M5.Lcd.setTextColor(TFT_BLUE, TFT_RED);
        if(A == '0'){
          A = '5';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 2) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '5'){
          A = '4';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 4) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '4'){
          A = '3';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 6) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '3'){
          A = '2';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 8) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '2'){
          A = '1';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 10) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '1'){
          A = '5';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 2) * PI) * 120), TFT_GREEN);
          }        
        }
        M5.Lcd.drawCentreString("Mode" + (String)A, 320/2, 240/2, 4);
      }

      if(M5.BtnB.wasPressed()){
        M5.Lcd.fillRect(0,0,320,240, TFT_RED);
        M5.Lcd.setTextColor(TFT_BLUE, TFT_RED);
        if(A == '0'){
          A = '1';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 10) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '1'){
          A = '2';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 8) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '2'){
          A = '3';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 6) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '3'){
          A = '4';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 4) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '4'){
          A = '5';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 2) * PI) * 120), TFT_GREEN);
          }        
        }else if(A == '5'){
          A = '1';
          for (int16_t i = 0; i < 320; i++){
            M5.Lcd.drawPixel(i, (240 / 2) - (int16_t)(sin((double)i / (320 / 10) * PI) * 120), TFT_GREEN);
          }        
        }
        M5.Lcd.drawCentreString("Mode" + (String)A, 320/2, 240/2, 4);
      }

      if(M5.BtnC.wasPressed()){
        A = '0';
        Serial.println("Stopped");
        M5.Lcd.fillRect(0,0,320,240, TFT_GREEN);
        M5.Lcd.setTextColor(TFT_BLUE, TFT_GREEN);
        M5.Lcd.drawCentreString("Stopped", 320/2, 240/2, 4);
        delay(1500);
        M5.Lcd.fillRect(0,0,320,240, TFT_BLACK);
        // Use GVS Logo
        M5.Lcd.pushImage(0, 0, 320, 240, (uint16_t *)gImage_logoGVS);
      }

      uint8_t  buf[64];
      for (uint8_t i=0; i<64; i++){
        buf[i] = 0;
     }

      uint16_t rcvd = 64;

      rcode = Ftdi.RcvData(&rcvd, buf);

      if (rcode && rcode != hrNAK){
        ErrorMessage<uint8_t>(PSTR("Ret"), rcode);
      }

      if( rcvd > 2 ) { //more than 2 bytes received
        for(uint16_t i = 2; i < rcvd; i++ ) {
          Serial.print((char)buf[i]);
          M5.Lcd.print((char)buf[i]);
        }
      }
      GVStest(A); // 前庭電気刺激(Galvanic Vestibular Stimulation)
    }
    M5.update();
}

// ##############################################################################################
// GVS Control
// ##############################################################################################
void GVStest(char Mode){
  uint8_t  rcode;
  int i;

  if (Mode == '1'){
   if (pole == 1){
      pole = 0;
    }
    else{
      pole = 1;
    }
    for (i = 0; i < 256; i++){
      Dat = (int)(sin((double)i / 256 * PI) * 4095);
//      M5.lcd.printf("D = %d,%d\n", Dat,pole);
      Serial.printf("D = %d,%d\n", Dat,pole);
      ID = 1;
      buffer[0] = 'G';
      buffer[1] = (ID << 5) + (pole << 4) + (Dat >> 8);
      buffer[2] = (char)Dat;
//      hCom1->transmit((BYTE*)buffer, 3);
      rcode = Ftdi.SndData(strlen(buffer), (uint8_t*)buffer);
      if (rcode){
        ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
      }
      delay(1);
    }
  }
  else if (Mode== '2'){
    if (pole == 1){
      pole = 0;
    }
    else{
      pole = 1;
    }
    for (i = 0; i < 128; i++){
      Dat = (int)(sin((double)i / 128 * PI) * 4095);
//      M5.lcd.printf("D = %d,%d\n", Dat,pole);
      Serial.printf("D = %d,%d\n", Dat,pole);
      ID = 1;
      buffer[0] = 'G';
      buffer[1] = (ID << 5) + (pole << 4) + (Dat >> 8);
      buffer[2] = (char)Dat;
//      hCom1->transmit((BYTE*)buffer, 3);
      rcode = Ftdi.SndData(strlen(buffer), (uint8_t*)buffer);
      if (rcode){
        ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
      }
      delay(1);
    }
  }
  else if (Mode == '3'){
    if (pole == 1){
      pole = 0;
      }
      else{
        pole = 1;
      }

      for (i = 0; i < 64; i++) {
        Dat = (int)(sin((double)i / 64 * PI) * 4095);
//      M5.lcd.printf("D = %d,%d\n", Dat,pole);
      Serial.printf("D = %d,%d\n", Dat,pole);
      ID = 1;
      buffer[0] = 'G';
      buffer[1] = (ID << 5) + (pole << 4) + (Dat >> 8);
      buffer[2] = (char)Dat;
//      hCom1->transmit((BYTE*)buffer, 3);
      rcode = Ftdi.SndData(strlen(buffer), (uint8_t*)buffer);
      if (rcode){
        ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
      }
      delay(1);
    }
  }
  else if (Mode == '4'){
    if (pole == 1){
      pole = 0;
    }
    else{
      pole = 1;
    }
    for (i = 0; i < 32; i++) {
      Dat = (int)(sin((double)i / 32 * PI) * 4095);
//      M5.lcd.printf("D = %d,%d\n", Dat,pole);
      Serial.printf("D = %d,%d\n", Dat,pole);
      ID = 1;
      buffer[0] = 'G';
      buffer[1] = (ID << 5) + (pole << 4) + (Dat >> 8);
      buffer[2] = (char)Dat;
//      hCom1->transmit((BYTE*)buffer, 3);
      rcode = Ftdi.SndData(strlen(buffer), (uint8_t*)buffer);
      if (rcode){
        ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
      }
      delay(1);
    }
  }
  else if (Mode == '5') {
    if (pole == 1) {
      pole = 0;
    }
    else{
      pole = 1;
    }
    for (i = 0; i < 16; i++) {
      Dat = (int)(sin((double)i / 16 * PI) * 4095);
//      M5.lcd.printf("D = %d,%d\n", Dat,pole);
      Serial.printf("D = %d,%d\n", Dat,pole);
      ID = 1;
      buffer[0] = 'G';
      buffer[1] = (ID << 5) + (pole << 4) + (Dat >> 8);
      buffer[2] = (char)Dat;
//      hCom1->transmit((BYTE*)buffer, 3);
      rcode = Ftdi.SndData(strlen(buffer), (uint8_t*)buffer);
      if (rcode){
        ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
      }
      delay(1);
    }
  }
/*  else{
    //'G'を送るとデータを受け取る MSBで16ビットのうち 先頭3ビットがチャンネル 4ビット目が極(+か-の)，
    //のこり12bitがデータというフォーマットです．
    for (i = 0; i < 256; i++) {
      ID = 1;
      Dat = i * 16 * 2;
      buffer[0] = 'G';
      buffer[1] = (ID << 5) + (pole << 4) + (Dat >> 8);
      buffer[2] = (char)Dat;
//      hCom1->transmit((BYTE*)buffer, 3);
      rcode = Ftdi.SndData(strlen(buffer), (uint8_t*)buffer);
      if (rcode){
        ErrorMessage<uint8_t>(PSTR("SndData"), rcode);
      }
      Serial.printf("D = %d,%d\n", Dat,pole);
      delay(20);
    }
  }
 */
}
