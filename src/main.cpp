#include <Arduino.h>

#include "PCF8575.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "FS.h"
#include "SPIFFS.h"
#include <vector>


//  adjust addresses if needed
#include "PCF8575.h"

// these should be an array or vector

PCF8575 PCF;
PCF8575 PCF0(0x20);
PCF8575 PCF1(0x21);
PCF8575 PCF2(0x22);
PCF8575 PCF3(0x23);
PCF8575 PCF4(0x24);
PCF8575 PCF5(0x25);
PCF8575 PCF6(0x26);
PCF8575 PCF7(0x27);

//PCF8575 blort= new PCF8575(0x20);

WebServer server(80);
extern void InitServer(void);


char HOSTNAME[]="htester";
char ssid[]="TELUS0609";
char password[]="t6z7gmkhd5";

void InitWiFi(void){
  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HOSTNAME);
  WiFi.begin(ssid,password);
  //WiFi.begin("telus0609", "t6z7gmkhd5");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void InitFileSystem(void){
   #define FORMAT_FS_IF_FAILED true

    if(!SPIFFS.begin(FORMAT_FS_IF_FAILED)){
        Serial.println("FS Mount Failed");
    }
    else
    {
       Serial.println("FS Mounted");
    }
}

void InitMDNS(void) {
  if (MDNS.begin(HOSTNAME)) {
    MDNS.addService("telnet", "tcp", 23);
    Serial.printf("MDNS %s.local\r\n",HOSTNAME);
  }
  else
    Serial.println("MDNS failed to start");
}

void InitPCF(PCF8575 PCF,int msg) {
if (!PCF.begin())
  {
    Serial.print(msg);
    Serial.println(" => not initialized");
  }
  else
  if (!PCF.isConnected())
  {
    Serial.print(msg);
    Serial.println(" => not connected");
  }
  else
  {
    Serial.print(msg);
    Serial.println(" => connected!!");
  }

}

void InitPCFS(void) {
  InitPCF(PCF0,0);
  InitPCF(PCF1,1);
  InitPCF(PCF2,2);
  InitPCF(PCF3,3);
  InitPCF(PCF4,4);
  InitPCF(PCF5,5);
  InitPCF(PCF6,6);
  InitPCF(PCF7,7);

}

void WritePCF(int pin,int val) {

  int device,ppin;

  device=pin/16;
  ppin=pin%16;

  if(device==0) PCF0.write(ppin,val);
  if(device==1) PCF1.write(ppin,val);
  if(device==2) PCF2.write(ppin,val);
  if(device==3) PCF3.write(ppin,val);
  if(device==4) PCF4.write(ppin,val);
  if(device==5) PCF5.write(ppin,val);
  if(device==6) PCF6.write(ppin,val);
  if(device==7) PCF7.write(ppin,val);


}

int ReadPCF(int pin) {
  int device,ppin,val;

  device=pin/16;
  ppin=pin%16;

  if(device==0) val=PCF0.read(ppin);
  if(device==1) val=PCF1.read(ppin);
  if(device==2) val=PCF2.read(ppin);
  if(device==3) val=PCF3.read(ppin);
  if(device==4) val=PCF4.read(ppin);
  if(device==5) val=PCF5.read(ppin);
  if(device==6) val=PCF6.read(ppin);
  if(device==7) val=PCF7.read(ppin);

  Serial.printf("Read device %d pin %d as %d\r\n",device,ppin,val);

  return val;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("It's.... alive! ");

Serial.print("PCF8575_LIB_VERSION:\t");
  Serial.println(PCF8575_LIB_VERSION);


  Wire.begin();

  InitPCFS();

  InitWiFi();
  InitMDNS();
  InitFileSystem();
  //httpUpdater.setup(&server);

  InitServer();
  
  MDNS.addService("http", "tcp", 80);
  Serial.println("HTTP service added");


  WritePCF(64+5,1);
  WritePCF(64+6,0);
  WritePCF(64+7,0);

}

int last=0;

void loop() {
  return;
  int thiss,tthat;
  server.handleClient();        // handle any pending HTTP requests     
  thiss=ReadPCF(64+11);
  if(thiss!=last) {
    last=thiss;
    Serial.printf("This: %d\r\n",thiss);
    WritePCF(64+6,thiss);
  }
  tthat=ReadPCF(64+8);
 // Serial.printf("That: %d\r\n",tthat); 
  if(tthat) WritePCF(64+5,1); else WritePCF(64+5,0);
}
