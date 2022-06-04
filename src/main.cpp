#include <M5Stack.h>

#include <Arduino.h>

#include <main.h>

#include <EEPROM.h>

#include <FF.h>

#include <strings.h>


#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include "WebServer.h"
#include <Preferences.h>

#include "files/files.h"


#define ma 30

#define COUNTER_ADDR 0




const IPAddress apIP(192, 168, 4, 1);
const char* apSSID = "M5STACK_SETUP";
boolean settingMode;
String ssidList;
String wifi_ssid;
String wifi_password;

// DNSServer dnsServer;
WebServer webServer(80);

// wifi config store
Preferences preferences;






static constexpr int LOOP_PERIOD = 1000; // Display updates every 1000 ms
uint32_t updateTime = 0; // time for next update
uint32_t cs = 0; // time for next update
uint32_t cm = 0; // time for next update
uint32_t ch = 0; // time for next update
uint32_t c10 = 0; // time for next update

int counter, counter_s, counter_m, counter_h, counter_10 = 0;
int status = 0;
int audio_stat = 0;
int rad = 0;
int ccc = 0;

unsigned int cc[ma];

void setup() {
  /*
    EEPROM.begin(4096);

    if(sizeof(EEPROM.read(COUNTER_ADDR))==0){
      counter = EEPROM.read(COUNTER_ADDR);
    }
  */
  m5.begin();
  M5.Power.begin();
  M5.Speaker.begin();

  updateTime = millis(); // Next update time



  preferences.begin("wifi-config");

  delay(10);
  
  if (restoreConfig()) {
    if (checkConnection()) {
      settingMode = false;
      startWebServer();
      return;
    }
  }else{

  settingMode = true;
  setupMode();
  }
}



bool flag_tick = true;
bool flag_null = true;




void loop() {


  webServer.handleClient();



  M5.update();
  if (M5.BtnA.isPressed()) {
    if (flag_tick) {
      signalHandler();

      flag_tick = false;
    }
  } else {
    flag_tick = true;
  }

if (M5.BtnB.isPressed()) {
    if (flag_null) {
      nullRad();

      flag_null = false;
    }
  } else {
    flag_null = true;
  }










  if (updateTime <= millis()) {
    updateTime = millis() + LOOP_PERIOD;

    //draw();

  }






  if (cs <= millis()) {
    cs = millis() + 1000;
    rad = MovingAverage(&cc[0]);
    cc[ccc] = counter_s;
    draw();
    counter_s = 0;
    if(ccc<(ma-1)){
      ccc++;

    }else{
      ccc=0;
    }
  }

  if (c10 <= millis()) {
    c10 = millis() + 10000;

    //rad = counter_10;
    counter_10 = 0;

  }

  if (cm <= millis()) {
    cm = millis() + (1000 * 60);

    counter_m = 0;

  }

  if (ch <= millis()) {
    ch = millis() + ((1000 * 60) * 60);

    counter_h = 0;

  }







  if (audio_stat == 0) {

  } else
  if (audio_stat == 1) {

    if (millis() % 1000 == 0) {
      M5.Speaker.tone(661, 100);
    }

  } else
  if (audio_stat == 2) {

    if (millis() % 1000 == 0) {
      M5.Speaker.tone(661, 100);
    }

    if (millis() % 1000 == 200) {
      M5.Speaker.tone(661, 100);
    }

  } else
  if (audio_stat == 3) {

    if (millis() % 3000 == 200) {
      M5.Speaker.tone(661, 100);
    }

    if (millis() % 3000 == 400) {
      M5.Speaker.tone(661, 100);
    }

    if (millis() % 3000 == 600) {
      M5.Speaker.tone(661, 100);
    }

    if (millis() % 3000 == 800) {
      M5.Speaker.tone(661, 200);
    }

    if (millis() % 3000 == 1200) {
      M5.Speaker.tone(661, 200);
    }

    if (millis() % 3000 == 1600) {
      M5.Speaker.tone(661, 200);
    }

    if (millis() % 3000 == 2000) {
      M5.Speaker.tone(661, 100);
    }

    if (millis() % 3000 == 2200) {
      M5.Speaker.tone(661, 100);
    }

    if (millis() % 3000 == 2400) {
      M5.Speaker.tone(661, 100);
    }

  }
}
int w = 320;
int h = 240;

void draw() {/*
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF24);

  int size = 72;

  char buffer[50];
  char bat[5];
  sprintf(buffer, "%d", rad);
  int glyph_w = 60;
  int pr = 10;
  M5.Lcd.setTextColor(getDanger(rad), TFT_BLACK);
  M5.Lcd.drawString(buffer, (w - pr) - (strlen(buffer) * glyph_w), (h / 2) - (size / 2), 8);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.drawString(bat, 10, 10, 1);
  */




}

void signalHandler() {
  counter++;

  counter_s++;
  counter_10++;

  counter_m++;

  counter_h++;

  draw();
  //M5.Speaker.tone(661, 500);
  M5.Speaker.tone(25, 40);
  //M5.Speaker.tone(100, 50);
}

uint16_t getDanger(int c) {
  int _yellow = 59;
  int _red = 99;
  int _dark_red = 149;
  int a_yellow = 79;
  int a_red = 99;
  int a_dark_red = 149;

  if (c > a_dark_red) {
    audio_stat = 3;
  } else

  if (c > a_red) {

    audio_stat = 2;

  } else

  if (c > a_yellow) {
    audio_stat = 1;

  } else {
    audio_stat = 0;

  }

  if (c > _dark_red) {
    status = 3;
    return ConvertToRGB565(0xFF0000);
  } else

  if (c > _red) {

    status = 2;

    return ConvertToRGB565(0xFF0000);
  } else

  if (c > _yellow) {
    status = 1;

    return ConvertToRGB565(0xFFFF00);

  } else {
    status = 0;

    return ConvertToRGB565(0x00FF00);
  }

}

uint16_t ConvertToRGB565(uint32_t color) {
  // 23     16  15      8 7       0      
  // rrrr_rrrr  gggg_gggg bbbb_bbbb
  //            15  11 10   5 4   0
  //            rrrrr  gggggg bbbbb

  return (((color & 0xF80000) >> 8) | ((color & 0xFC00) >> 5) | ((color & 0xF8) >> 3));

}


unsigned int MovingAverage(unsigned int *arr){
  unsigned int r = 0;
  for(unsigned int i = 0; i < ma; i++){
    r = r+arr[i];
  }
  return r;
}




void nullRad(){
  for(unsigned int i = 0; i < ma; i++){
    cc[i]=0;
  }
  ccc=0;
}










boolean restoreConfig() {
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");
  Serial.print("WIFI-SSID: ");
  M5.Lcd.print("WIFI-SSID: ");
  Serial.println(wifi_ssid);
  M5.Lcd.println(wifi_ssid);
  Serial.print("WIFI-PASSWD: ");
  M5.Lcd.print("WIFI-PASSWD: ");
  Serial.println(wifi_password);
  M5.Lcd.println(wifi_password);
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

  if(wifi_ssid.length() > 0) {
    return true;
} else {
    return false;
  }
}

boolean checkConnection() {
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  M5.Lcd.print("Waiting for Wi-Fi connection");
  while ( count < 30 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      M5.Lcd.println();
      Serial.println("Connected!");
      M5.Lcd.println("Connected!");
      return (true);
    }
    delay(500);
    Serial.print(".");
    M5.Lcd.print(".");
    count++;
  }
  Serial.println("Timed out.");
  M5.Lcd.println("Timed out.");
  return false;
}

void startWebServer() {
  if (settingMode) {
    Serial.print("Starting Web Server at ");
    M5.Lcd.print("Starting Web Server at ");
    Serial.println(WiFi.softAPIP());
    M5.Lcd.println(WiFi.softAPIP());
    webServer.on("/rad",[]() {
      
  char buffer[50];
  sprintf(buffer, "%d", rad);
webServer.send(200, "text/plain", buffer);
    });
    /*
    webServer.on("/settings", []() {
      String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the SSID.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
    });
    webServer.on("/setap", []() {
      String ssid = urlDecode(webServer.arg("ssid"));
      Serial.print("SSID: ");
      M5.Lcd.print("SSID: ");
      Serial.println(ssid);
      M5.Lcd.println(ssid);
      String pass = urlDecode(webServer.arg("pass"));
      Serial.print("Password: ");
      M5.Lcd.print("Password: ");
      Serial.println(pass);
      M5.Lcd.println(pass);
      Serial.println("Writing SSID to EEPROM...");
      M5.Lcd.println("Writing SSID to EEPROM...");

      // Store wifi config
      Serial.println("Writing Password to nvr...");
      M5.Lcd.println("Writing Password to nvr...");
      preferences.putString("WIFI_SSID", ssid);
      preferences.putString("WIFI_PASSWD", pass);

      Serial.println("Write nvr done!");
      M5.Lcd.println("Write nvr done!");
      String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
      delay(3000);
      ESP.restart();
    });*/
    webServer.onNotFound([]() {
      String s = "<h1>AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("AP mode", s));
    });
  }
  else {
    Serial.print("Starting Web Server at ");
    M5.Lcd.print("Starting Web Server at ");
    Serial.println(WiFi.localIP());
    M5.Lcd.println(WiFi.localIP());
    webServer.on("/rad",[]() {
      
      double longi = 7.4803387500000005;
  
      double lati = 43.75944287499999;
      char outpp[200];
  
      sprintf(outpp, "{\"rad\":%d,\"latitude\":%f,\"longitude\":%f}", rad, lati, longi);
      webServer.send(200, "application/json", outpp);
    });
    webServer.on("/ol.css",[](){
      webServer.send(200, "text/css", olcss);
    });
    webServer.on("/jquery.js",[](){
      webServer.send(200,"text/javascript",jqueryjs);
    });
    webServer.on("/ol.js",[]() {
      webServer.send(200,"text/javascript",oljs);
    });
    webServer.on("/", []() {
      webServer.send(200, "text/html", indexhtml);
    });
    webServer.on("/givedata",[](){
      webServer.send(200, "text/html", givedata);
    });
    webServer.on("/worldmap",[](){
      webServer.send(200,"text/html",worldmap);
    });
    webServer.on("/reset", []() {
      // reset the wifi config
      preferences.remove("WIFI_SSID");
      preferences.remove("WIFI_PASSWD");
      String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
      webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
      delay(3000);
      ESP.restart();
    });
  }
  webServer.begin();
}

void setupMode() {
  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  Serial.println("");
  M5.Lcd.println("");
  for (int i = 0; i < n; ++i) {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID);
  WiFi.mode(WIFI_MODE_AP);
  // WiFi.softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
  // WiFi.softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0);
  // dnsServer.start(53, "*", apIP);
  startWebServer();
  Serial.print("Starting Access Point at \"");
  M5.Lcd.print("Starting Access Point at \"");
  Serial.print(apSSID);
  M5.Lcd.print(apSSID);
  Serial.println("\"");
  M5.Lcd.println("\"");
}

String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String urlDecode(String input) {
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}