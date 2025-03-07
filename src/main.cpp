#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Define button pins
#define BUTTON_UP 4
#define BUTTON_DOWN 2
#define BUTTON_ENTER 18
#define BUTTON_BACK 5

#define DHTPIN 32
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

int menuIndex = 0;
int numMenus = 10;

//check for enter menu
bool tempMenu = false;
bool humidityMenu = false;

int itemPerPage = 5;
static int menuOffset = 0;

String menus[] = {"Menu 1", "Menu 2", "Menu 3", "Menu 4", "Menu 5", "Menu 6", "Menu 7", "Menu 8", "Menu 9", "Menu 10"};

void displayMain(){
  if (menuIndex >= menuOffset + itemPerPage) menuOffset += 1;
  if (menuIndex < menuOffset) menuOffset -= 1;

  for (int i = 0; i < itemPerPage; i++) {
      int menuPosition = menuOffset + i;
      if (menuPosition >= numMenus) break; 

      if (menuPosition == menuIndex) display.drawRect(0, i * 10 + 10, 100, 10, SSD1306_WHITE);
      
      display.setCursor(5, i * 10 + 12);
      display.print(menus[menuPosition]);
  }

  int scrollbarHeight = itemPerPage * 10;
  int maxOffset = numMenus - itemPerPage;
  int scrollbarY = 10;

  if (maxOffset > 0) {
      scrollbarY = 10 + ((menuOffset * (scrollbarHeight - 10)) / maxOffset);
  }

  display.drawRect(120, 10, 5, scrollbarHeight, SSD1306_WHITE);
  display.fillRect(120, scrollbarY, 5, 10, SSD1306_WHITE);
  display.display();
}

void displayTemperature() {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(10, 10);
    display.println("DHT22");

    float temperature = dht.readTemperature();

    display.setTextSize(1);
    display.setCursor(10, 30);
    display.print("Temp: ");
    display.print(temperature);
    display.println(" C");

    if (isnan(temperature)) {
        Serial.println("Failed to read temperature!");
        display.print("Error");
    } else {
        Serial.print(temperature);
        Serial.println(" C");
    }
    display.display();
}

void displayHumidity(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 10);
  display.println("DHT22");

  float humidity = dht.readHumidity();

  display.setCursor(10, 40);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");

    if (isnan(humidity)) {
        Serial.println("Failed to read humidity!");
        display.print("Error");
    } else {
        Serial.print(humidity);
        Serial.println(" %");
    }

  display.display();
}

void handleButtonPress() {
    if (digitalRead(BUTTON_UP) == LOW) {
        if (menuIndex > 0) {
            menuIndex -= 1;
            Serial.println(menuIndex);
        } else {
            menuIndex = numMenus - 1;
        }
        Serial.println("btn up pressed");
        delay(200);
    }
    if (digitalRead(BUTTON_DOWN) == LOW) {
        if (menuIndex < numMenus - 1) {
            menuIndex += 1;
            Serial.println(menuIndex);
        } else {
            menuIndex = 0;
        }
        Serial.println("btn down pressed");
        delay(200);
    }
    if (digitalRead(BUTTON_ENTER) == LOW) {
        if (menuIndex == 1) {  
          tempMenu = true;
        } else if(menuIndex == 2) {
          humidityMenu = true;
          Serial.print("humidityMenu : " );
          Serial.println(humidityMenu);
        }
        Serial.println("btn enter pressed");
        delay(200);
    }
    if (digitalRead(BUTTON_BACK) == LOW) {
        if (tempMenu) tempMenu = false;
        else if(humidityMenu) humidityMenu = false;
        Serial.println("btn back pressed");
        delay(200);
    }
}

void displayMenu() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);

    displayMain();
}

void setup() {
    Serial.begin(115200);
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

    dht.begin();
    delay(2000);

    display.display();
    delay(2000);
    display.clearDisplay();

    pinMode(BUTTON_UP, INPUT_PULLUP);
    pinMode(BUTTON_DOWN, INPUT_PULLUP);
    pinMode(BUTTON_ENTER, INPUT_PULLUP);
    pinMode(BUTTON_BACK, INPUT_PULLUP);
}

void loop() {
    handleButtonPress();

    if (tempMenu) {
        displayTemperature();
    } else if(humidityMenu) {
        displayHumidity();
    } else {
        displayMenu();
    }
}
