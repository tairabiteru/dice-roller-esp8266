#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <ESP8266TrueRandom.h>
#include <d6.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define SCREEN_ADDRESS 0x3C // OLED I2C bus address

#define BUTTON 16 // GPIO input for button

#define FRAMERATE 60 // Target 60 FPS, or about 17 ms delay per frame.
// Be careful setting this higher, as it's limited by the bandwidth of I2C, which is not very high.


// Connection on GPIO 5 and 4.
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


// Position the specified die at the specified X and Y coordinates.
void place_die(int die, int x, int y) {
  display.clearDisplay();
  display.drawBitmap(x, y, d6_array[die-1], 64, 64, 1);
  display.display();
}


// Roll the die in place at 0, 0.
void roll() {
  int face = ESP8266TrueRandom.random(1, 7);
  place_die(face, 0, 0);
  delay(1000 / FRAMERATE);
}


// "Finishes" the roll by having the die travel across
// the screen before coming to rest.
void finish_roll() {
	for (int i=0; i<10; i++) {
		int face = ESP8266TrueRandom.random(1, 7);
		place_die(face, i*8, 0);
		delay(1000 / FRAMERATE);
	}
}


// Run once at boot.
void setup() {
  // Button utilizes hardware pulldown on GPIO 16.
  // If a different pin is used, you may have to use an external pulldown resistor.
  pinMode(BUTTON, INPUT_PULLDOWN_16);

  // Initialize display.
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.display();
  // Wait for 200 ms so display can finish up.
  delay(200);
}


// Loop forever.
void loop() {
  // While button is pressed, show roll animation.
  while (digitalRead(BUTTON) == true) {
	roll();
  }

  // When button is released, finish the roll...
  finish_roll();

  // Then wait for the button to be pressed again.
  while (digitalRead(BUTTON) == false) {
	delay(5);
  }
}