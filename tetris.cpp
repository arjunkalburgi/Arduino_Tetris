/* ~Tetris

	Arjun Kalburgi
	1388774
	
	Zhengyang Li
	1408218
 
	Draw a scrollable map with a curser controlled by the joystick.
	List restaurants in the area.
 */

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "string.h"
#include "tetris.h"

#include <lcd_image.h>    // U of A Utils

#define SD_CS    5
#define TFT_CS   6
#define TFT_DC   7
#define TFT_RST  8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int grid[20][16] = {{0}};
int score = 0;
int leftPin = 2;
int rightPin = 3;
int turnPin = 40;
char rowkilled = 0;


void setup() {
	Serial.begin(9600);

	// initialise display
	tft.initR(INITR_BLACKTAB);
	tft.fillScreen(ST7735_BLACK);

	// initialise pins
	pinMode(leftPin, INPUT);
	pinMode(rightPin, INPUT);
	pinMode(turnPin, INPUT);
	
	// display splashscreen
	tft.setTextColor(ST7735_WHITE);
	tft.setTextWrap(true);
	
	tft.setCursor(34, 20);
	tft.println("WELCOME TO");
	
	tft.setCursor(45, 30);
	tft.println("~TETRIS");
	
	tft.setCursor(16, 50);
	tft.println("PRESS THE MIDDLE");
	
	tft.setCursor(18, 60);
	tft.println("BUTTON TO START");

	tft.setCursor(57, 110);
	tft.println("BY:");

	tft.setCursor(33, 120);
	tft.println("NICHOLAS LI");

	tft.setCursor(56, 130);
	tft.println("AND");

	tft.setCursor(24, 140);
	tft.println("ARJUN KALBURGI");

	// only continue if turnPin is pressed
	while(digitalRead(turnPin) == LOW) {}

	tft.fillScreen(ST7735_BLACK);
}

void loop() {
	// get block
	block blck = getRdm();

	// update the grid and print
	// for (int i = 0; i < 4; ++i){
	// 	grid[blck.row[i]][blck.col[i]] = 1;
	// }
	block old_blck = blck;

	printblock(blck);

	while(down(&blck)) {
		// blck is has been moved down.

		// remove old_blck
		removeblock(old_blck);

		// update old_blck
		old_blck = blck;

		// check "turn" button
		if (digitalRead(turnPin) == HIGH) {
			// turn us
			turnblock(&blck);
		}

		// check "left block"
		if (digitalRead(leftPin) == HIGH) {
			// go left if possible - arguments are blockr and blockc
			left(&blck);
		}

		// check "right block"
		if (digitalRead(rightPin) == HIGH) {
			// go right if possible  - arguments are blockr and blockc
			right(&blck);
		}

		// remove old_blck
		removeblock(old_blck);

		// update old_blck
		old_blck = blck;

		// print new blck
		printblock(blck);
		
		// difficulty
		// change how fast the block falls based on score
		if (score < 5) {
			Serial.println(1);
			delay(250);
		}
		if (score >= 5 && score < 10) {
			Serial.println(2);
			delay(200);
		}
		if (score >= 10 && score < 20) {
			Serial.println(3);
			delay(150);
		}
		if (score >= 20) {
			Serial.println(4);
			delay(100);
		}

		// gridprinter();
	}

	// check grid for and full rows or endgame.
	checkrow();
}