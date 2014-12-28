#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "string.h"
#include "tetris.h"

#include <lcd_image.h>    // U of A Utils


// draws the current matrix to the board
void drawboard() {
	// loop the rows
	for (int r = 0; r < 20; ++r)
	{
		// loop the columns
		for (int c = 0; c < 16; ++c)
		{
			// draw a white part for filled parts of the grid
			if (grid[r][c] == 1) {
				tft.drawRect(8*c, 8*r, 8, 8, ST7735_WHITE);
				tft.fillRect(8*c, 8*r, 8, 8, ST7735_WHITE);
			}
			// draw a black part for empty parts of the grid
			if (grid[r][c] == 0) {
				tft.drawRect(8*c, 8*r, 8, 8, ST7735_BLACK);
				tft.fillRect(8*c, 8*r, 8, 8, ST7735_BLACK);
			}
		}
	}
}

void killrow(int deadrow) {
	// loop over the rows from the row that needs to be killed to the top row
	for (int r = deadrow; r > 0; --r)
	{
		// copy the row with the one above it
		memcpy(&grid[r], &grid[r-1], 32);
	}
	// set the first row to zero's
	int temp[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	memcpy(&grid[0], &temp, 16);
}

void endgame() {
	// turn the screen black and type the end splash screen
	tft.fillScreen(ST7735_BLACK);
	tft.setTextColor(ST7735_WHITE);
	tft.setTextWrap(true);
	
	tft.setCursor(34, 20);
	tft.println("YOU SCORED");
	
	tft.setCursor(61, 40);
	tft.println(score);
	
	tft.setCursor(47, 60);
	if (score == 1)
	{
		tft.println("POINT");
	} else {
		tft.println("POINTS");
	}
	
	tft.setCursor(16, 80);
	tft.println("PRESS THE MIDDLE");
	
	tft.setCursor(13, 90);
	tft.println("BUTTON TO RESTART");

	tft.setCursor(57, 110);
	tft.println("BY:");

	tft.setCursor(33, 120);
	tft.println("NICHOLAS LI");

	tft.setCursor(56, 130);
	tft.println("AND");

	tft.setCursor(24, 140);
	tft.println("ARJUN KALBURGI");

	// when the button is pressed the while loop will stop
	while(digitalRead(turnPin) == LOW) {}
	
	score = 0;
	
	// clear the grid
	for (int r = 0; r < 20; ++r)
	{
		for (int c = 0; c < 16; ++c)
		{
			grid[r][c] = 0;
		}
	}
	
	// clear the screen
	tft.fillScreen(ST7735_BLACK);
	
	return;
}

// runs before a new block comes
// checks if we need to end the game and if we need to remove a row
void checkrow() {
	// check top row for anything
	for (int c = 0; c < 16; ++c) {
		if (grid[0][c]) {
			// if there is anything in the top row, game is over.
			endgame();
		}
	}
	
	// check if we need rows to be killed
	for (int r = 0; r <= 19; ++r)
	{
		int fullrow = 0;
		
		for (int c = 0; c < 16; ++c)
		{
			if (grid[r][c]) {
				fullrow = 1;
			} else {
				fullrow = 0;
				c = 16; // breaks inner array, we know this column isn't full
			}
		}
		if (fullrow == 1) {
			killrow(r); // remove the row and bring everything down
			score = score + 1;
			rowkilled = 1;
		}
	}
	if (rowkilled) {
		drawboard();
		rowkilled = 0;
	}
}


block getRdm() {
	// get a new sequence of random numbers
	randomSeed(analogRead(A10));
	
	// pick a random number from 1 to 7
	int num = random(1,7);

	// create the block based on num and return it
	if (num == 1) {
		block blck = {{0,0,1,1},{7,8,7,8},1,1};
		return blck;
	}
	else if (num == 2) {
		block blck = {{0,1,2,2},{7,7,7,8},1,2};
		return blck;
	}
	else if (num == 3) {
		block blck ={{0,1,2,2},{8,8,8,7},1,3};
		return blck;
	}
	else if (num == 4) {
		block blck = {{1,1,0,0},{7,8,8,9},1,4};
		return blck;
	}
	else if (num == 5) {
		block blck = {{0,0,1,1},{7,8,8,9},1,5};
		return blck;
	}
	else if (num == 6) {
		block blck = {{0,1,2,3},{7,7,7,7},1,6};
		return blck;
	}
	else if (num == 7) {
		block blck = {{0,1,1,1},{8,7,8,9},1,7};
		return blck;
	}
}
