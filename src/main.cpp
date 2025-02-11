#include <Arduino.h>

#include "PCF8575.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "FS.h"
#include "SPIFFS.h"



//  adjust addresses if needed
#include "PCF8575.h"
PCF8575 PCF(0x20);
WebServer server(80);
extern void InitServer(void);

// put function declarations here:
int myFunction(int, int);

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

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  Serial.begin(115200);
  Serial.println("It's.... alive! ");

Serial.print("PCF8575_LIB_VERSION:\t");
  Serial.println(PCF8575_LIB_VERSION);

  Wire.begin();

  if (!PCF.begin())
  {
    Serial.println("could not initialize...");
  }
  if (!PCF.isConnected())
  {
    Serial.println("=> not connected");
  }
  else
  {
    Serial.println("=> connected!!");
  }

  InitWiFi();
  InitMDNS();
  InitFileSystem();
  //httpUpdater.setup(&server);

  InitServer();
  
  MDNS.addService("http", "tcp", 80);
  Serial.println("HTTP service added");

//  PCF.write(0,0);
//  PCF.write(1,0);
//  PCF.write(2,0);
//  PCF.write(3,0);
//  PCF.write(4,0);
//  PCF.write(5,0);
  PCF.write(6,1);
  PCF.write(7,0);

  Serial.printf("Read:  8 %d\r\n",PCF.read(8));
  Serial.printf("Read: 11 %d\r\n",PCF.read(11));

}

int last=0;

void loop() {
  int thiss;
  server.handleClient();        // handle any pending HTTP requests     
  thiss=PCF.read(11);
  if(thiss!=last) {
    last=thiss;
    Serial.printf("This: %d\r\n",thiss);
    PCF.write(6,thiss);
  }
  if(PCF.read(8)) PCF.write(5,1); else PCF.write(5,0); 
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}