#include <M5StickCPlus2.h>
#include <WiFi.h>
#include <Preferences.h>

#define DISABLE_CODE_FOR_RECEIVER
#define DECODE_NEC
#define NO_LED_FEEDBACK_CODE
#include <IRremote.h>

#define IR_PIN 19
#define SCREEN_TIMEOUT 10000

// ---------- GLOBAL ----------
Preferences prefs;
IRsend irsend(IR_PIN);

bool wifiAP = false;
bool screenOn = true;
bool needRedraw = true;
unsigned long lastActivity = 0;

// ---------- MENU ----------
enum Menu {
  MENU_MAIN,
  MENU_WIFI,
  MENU_IR,
  MENU_SYSTEM
};

Menu menu = MENU_MAIN;
int menuIndex = 0;

// ---------- MENU DATA ----------
const char* mainMenu[] = {"WiFi Tools", "IR Remote", "System"};
const char* wifiMenu[] = {"Scan", "AP Mode"};
const char* irMenu[]   = {"Power", "Vol +", "Vol -"};
const char* sysMenu[]  = {"Info", "Battery", "Sleep", "Reset"};

int menuSize() {
  if (menu == MENU_MAIN)   return 3;
  if (menu == MENU_WIFI)   return 2;
  if (menu == MENU_IR)     return 3;
  if (menu == MENU_SYSTEM) return 4;
  return 0;
}

const char** menuItems() {
  if (menu == MENU_MAIN)   return mainMenu;
  if (menu == MENU_WIFI)   return wifiMenu;
  if (menu == MENU_IR)     return irMenu;
  if (menu == MENU_SYSTEM) return sysMenu;
  return nullptr;
}

// ---------- DRAW ----------
void drawTitle() {
  M5.Display.setTextSize(2);
  M5.Display.setCursor(8, 5);
  M5.Display.print("Rust OS");

  M5.Display.setTextSize(1);
  M5.Display.setCursor(120, 8);
  M5.Display.printf("%d%%", M5.Power.getBatteryLevel());
}

void drawMenu() {
  M5.Display.fillScreen(BLACK);
  drawTitle();

  M5.Display.setTextSize(2);
  for (int i = 0; i < menuSize(); i++) {
    M5.Display.setCursor(5, 30 + i * 20);
    M5.Display.print(i == menuIndex ? "> " : "  ");
    M5.Display.println(menuItems()[i]);
  }
}

// ---------- FUNCTIONS ----------
void wifiScan() {
  M5.Display.fillScreen(BLACK);
  M5.Display.println("Scanning...");
  int n = WiFi.scanNetworks();
  M5.Display.fillScreen(BLACK);
  for (int i = 0; i < n && i < 6; i++) {
    M5.Display.printf("%d:%s\n", i + 1, WiFi.SSID(i).c_str());
  }
  delay(3000);
}

void toggleAP() {
  wifiAP = !wifiAP;
  prefs.putBool("wifiAP", wifiAP);

  if (wifiAP) WiFi.softAP("M5StickC");
  else WiFi.softAPdisconnect(true);
}

void sendIR() {
  if (menuIndex == 0) irsend.sendNEC(0x20DF10EF, 32);
  if (menuIndex == 1) irsend.sendNEC(0x20DF40BF, 32);
  if (menuIndex == 2) irsend.sendNEC(0x20DFC03F, 32);
}

void systemInfo() {
  M5.Display.fillScreen(BLACK);
  M5.Display.printf("Heap: %d\n", ESP.getFreeHeap());
  M5.Display.printf("WiFi AP: %s\n", wifiAP ? "ON" : "OFF");
  delay(3000);
}

void batteryInfo() {
  M5.Display.fillScreen(BLACK);
  M5.Display.printf("Level: %d%%\n", M5.Power.getBatteryLevel());
  M5.Display.printf("Voltage: %dmV\n", M5.Power.getBatteryVoltage());
  M5.Display.printf("Charging: %s\n",
    M5.Power.isCharging() ? "YES" : "NO");
  delay(3000);
}

void goSleep() {
  M5.Display.fillScreen(BLACK);
  M5.Display.println("Sleeping...");
  delay(500);
  M5.Display.sleep();
  esp_light_sleep_start();
}

void resetAll() {
  prefs.clear();
  ESP.restart();
}

// ---------- SETUP ----------
void setup() {
  M5.begin();
  M5.Display.setRotation(3);

  WiFi.mode(WIFI_STA);
  prefs.begin("settings", false);

  wifiAP = prefs.getBool("wifiAP", false);
  if (wifiAP) toggleAP();

  drawMenu();
  lastActivity = millis();
}

// ---------- LOOP ----------
void loop() {
  M5.update();

  bool A = M5.BtnA.wasPressed();
  bool B = M5.BtnB.wasPressed();
  bool C = M5.BtnC.wasPressed();
  bool B_LONG = M5.BtnB.pressedFor(700);

  // --- navigation ---
  if (A) {
    menuIndex = (menuIndex + 1) % menuSize();
    needRedraw = true;
  }

  if (B) {
    if (menu == MENU_MAIN) {
      menu = (Menu)(menuIndex + 1);
      menuIndex = 0;
    } else if (menu == MENU_WIFI) {
      if (menuIndex == 0) wifiScan();
      if (menuIndex == 1) toggleAP();
    } else if (menu == MENU_IR) {
      sendIR();
    } else if (menu == MENU_SYSTEM) {
      if (menuIndex == 0) systemInfo();
      if (menuIndex == 1) batteryInfo();
      if (menuIndex == 2) goSleep();
      if (menuIndex == 3) resetAll();
    }
    needRedraw = true;
  }

  // --- universal back ---
  if ((C || B_LONG) && menu != MENU_MAIN) {
    menu = MENU_MAIN;
    menuIndex = 0;
    needRedraw = true;
  }

  // --- update last activity and wake screen ---
  if (A || B || C) {
    lastActivity = millis();
    if (!screenOn) {
      M5.Display.wakeup();
      screenOn = true;
    }
  }

  // --- screen timeout ---
  if (millis() - lastActivity > SCREEN_TIMEOUT && screenOn) {
    M5.Display.sleep();
    screenOn = false;
  }

  // --- redraw ---
  if (needRedraw) {
    drawMenu();
    needRedraw = false;
  }
}