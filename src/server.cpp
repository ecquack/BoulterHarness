#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "FS.h"
#include "SPIFFS.h"

extern WebServer server;

#define HTML_FILE       0
#define TEXT_FILE       1
#define MAX_CSTR      256
#define FILESYSTEM SPIFFS

int FileSize=0;
char *FileData=0;

void freeFile(void)
{
  if(FileData) free(FileData);
  FileData=0;
  FileSize=0;
}


int readFile(fs::FS &fs, const char * path){
  int fsize;
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return 0;
    }
    fsize=file.size();
    Serial.printf("File size: %d\r\n",(int)fsize);
    
  freeFile();

    FileSize=fsize;
    FileData=(char *)malloc(FileSize+1);

    if(FileData){
      FileData[FileSize]=0; // null terminate
      file.read((uint8_t*)FileData,FileSize);
    }
    else{
      Serial.println("File malloc failed");
      return 0;
    }
    file.close();
    return 1;
}


int serveFile(char *filename,int plaintext)
{
  Serial.printf("SERVE FILE: %s\r\n",filename);
  if(readFile(FILESYSTEM,filename))
  {
      if(plaintext==1)
          server.send_P(200,"text/plain",FileData,FileSize);
      else
          server.send(200,"text/html",FileData);
  
    freeFile();
    return 0;
  }
  else
  {
  Serial.printf("unable to read file\r\n");

  }
  return 1;
}

void handleNotFound() {  // 404
 String filename=server.uri();
  if(serveFile((char *)filename.c_str(),0)) {
    String message = "<!DOCTYPE html><html><head><title>Hot Tub</title></head><body>";
    message +="<h1>404</h1><pre>";
    message +="File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for (uint8_t i = 0; i < server.args(); i++) {
      message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    message+="</pre></body></html>";
    server.send(404, "text/html", message);
  }
}

void handleFilename(char *filename,int plaintext)
{
 if(serveFile(filename,plaintext)){
  handleNotFound();
 }
}


void InitServer(void){
  
  server.enableCORS();

  server.on("/",                  [](){ handleFilename((char *)"/index.html",HTML_FILE);   }); 
  server.on("/index.html",        [](){ handleFilename((char *)"/index.html",HTML_FILE);    });
    
// 404 code- searches for file and serves as HTML or give error page
  server.onNotFound(              handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
  
}
