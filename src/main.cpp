#include <Arduino.h>

#include "PCF8575.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "FS.h"
#include "SPIFFS.h"
#include <vector>


const char *PinDescriptions[]={
  "pendant stop","35",// 0
  "pendant start","35",//1
  "pendant jog","35",//2
  "pendant red","35",//3
  "pendant green","35",//4
  "pendant alarm","35",//5
  "chassis ground","35",//6
  "interlock stop +24","35",//7
  "interlock stop gnd","35",//8
  "n.c.","35",//9
  "clutch switched gnd","35",//10
  "pendant +24v","35",//11
  "pendant gnd","35",//12
  "clutch power","35",//13
  "n.c.","35",//14
  "missing flat/egg +24","35",//15
  "missing flat/egg gnd","35",//16
  "cycle stop","35",//17
  "missing flat","35",//18
  "missing egg","35",//19
  "missing egg extra","35",//20
  "n.c.","35",//21
  "n.c.","35",//22
  "n.c.","35",//23
  "stacker int","35",//24
  "stacker E-stop","35",//25
  "n.c.","35",//26
  "n.c.","35",//27
  "n.c.","35",//28
  "n.c.","35",//29
  "n.c.","35",//30
  "n.c.","35",//31
  // outputs 
  "pendant red","purple",//32
  "pendant GND","purple",//33
  "pendant start","purple",//34
  "pendant job","purple",//35
  "pendant green","purple",//36
  "pendant alarm","purple",//37
  "pendant +24V","purple",//38
  "pendant stop","purple",//39
  "n.c.","",//40
  "n.c.","",//41
  "n.c.","",//42
  "n.c.","",//43
  "clutch power","red",//44
  "n.c.","",//45
  "n.c.","",//46
  "clutch switched GND","red",//47
  "interlock stop +24V","orange",//48
  "n.c","",//49
  "missing flat +24V","orange",//50
  "missing flat GND","orange",//51
  "interlock cycle stop GND","blue",//52
  "missing egg extra","blue",//53
  "missing egg GND","blue",//54
  "missing egg","blue",//55
  "stacker ","black",//56
  "missing egg +24V","black",//57
  "stacker int","black",//58
  "interlock stop +24V","black",//59
  "missing flat","yellow",//60
  "missing flat egg GND","yellow",//61
  "n.c.","",//62
  "interlock cycle stop GND","yellow",//63
  "Sixty four error","error"
};

int KnownGood[]={
  0,39,-1,-1,
  1,34,-1,-1,
  2,35,-1,-1,
  3,32,-1,-1,
  4,36,-1,-1,
  5,37,-1,-1,
  6,40,-1,-1,
  7,48,59,-1,
  8,52,63,-1,
  9,-1,-1,-1,
  10,47,-1,-1,
  11,38,-1,-1,
  12,33,-1,-1,
  13,44,-1,-1,
  14,-1,-1,-1,
  15,50,57,-1,
  16,54,61,-1,
  17,51,-1,-1,
  18,60,-1,-1,
  19,55,-1,-1,
  20,53,-1,-1,
  21,-1,-1,-1,
  22,-1,-1,-1,
  23,-1,-1,-1,
  24,58,-1,-1,
  25,56,-1,-1,
  26,-1,-1,-1,
  27,-1,-1,-1,
  28,-1,-1,-1,
  29,-1,-1,-1,
  30,-1,-1,-1,
  31,-1,-1,-1,
  32, 3,-1,-1,
  33,12,-1,-1,
  34, 1,-1,-1,
  35, 2,-1,-1,
  36, 4,-1,-1,
  37, 5,-1,-1,
  38,11,-1,-1,
  39, 0,-1,-1,
  40, 6,-1,-1,
  41,-1,-1,-1,
  42,-1,-1,-1,
  43,-1,-1,-1,
  44,13,-1,-1,
  45,-1,-1,-1,
  46,-1,-1,-1,
  47,10,-1,-1,
  48, 7,59,-1,
  49,-1,-1,-1,
  50,15,57,-1,
  51,17,-1,-1,
  52, 8,63,-1,
  53,20,-1,-1,
  54,16,61,-1,
  55,19,-1,-1,
  56,25,-1,-1,
  57,15,50,-1,
  58,24,-1,-1,
  59, 7,48,-1,
  60,18,-1,-1,
  61,16,54,-1,
  62,-1,-1,-1,
  63, 8,52,-1
};



//  adjust addresses if needed
#include "PCF8575.h"

// these should be an array or vector. maybe next version

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

  //Serial.printf("Read device %d pin %d as %d\r\n",device,ppin,val);

  return val;
}

void PairScan(void) {
  // set all pins high
  for(int index=0;index<64;index++) {
    WritePCF(index,1);
  }

  for(int outpin=0;outpin<64;outpin++) {
    WritePCF(outpin,0);
    Serial.printf("%02d",outpin);
    for(int inpin=0;inpin<64;inpin++)
    {
      if(inpin!=outpin)
        if(ReadPCF(inpin)==0) Serial.printf(",%02d",inpin);
    }
    Serial.println();
    WritePCF(outpin,1);
  }
}

int ComparisonScan(void) {

  int comparison[4],cdex,errorcount=0;
  // set all pins high
  for(int index=0;index<64;index++) {
    WritePCF(index,1);
  }


  for(int outpin=0;outpin<64;outpin++) {
    WritePCF(outpin,0);
    for(cdex=0;cdex<4;cdex++) comparison[cdex]=-1;
    cdex=0;
    comparison[cdex++]=outpin;
    for(int inpin=0;inpin<64;inpin++)
    {
      if(inpin!=outpin)
        if(ReadPCF(inpin)==0) {
          comparison[cdex++]=inpin;
        }
    }
    int bad=0;
    for(cdex=0;cdex<4;cdex++) {
      if(KnownGood[outpin*4+cdex]!=comparison[cdex]) bad++;
      }
      {
    if(bad) { 
      Serial.printf("%02d,%02d,%02d,%02d %-32s %s\r\n",
        comparison[0],comparison[1],comparison[2],comparison[3],
        PinDescriptions[comparison[0]*2],PinDescriptions[comparison[0]*2+1]);
      errorcount++;
    }
    }
    WritePCF(outpin,1);
  }
  return errorcount;
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

  //Serial.println("Pair Scan");
  //PairScan();

  Serial.println("Comparison Scan");
  int compareresult=ComparisonScan();
  Serial.printf("Scan complete, %d errors\r\n",compareresult);
}

int last=0;

void loop() {
  int thiss,tthat;
  server.handleClient();        // handle any pending HTTP requests     
  return;
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
