#ifndef TETRIS_H
#define TETRIS_H

#include <Arduino.h>
#include <Adafruit_ST7735.h>

extern int grid[20][16];
extern int score; 
extern int leftPin; 
extern int rightPin;
extern int turnPin;
extern char rowkilled; 

extern Adafruit_ST7735 tft;

typedef struct {
	// array for four row blocks
	int row[4];

	// array for four column blocks
	int col[4]; 

	// parameter for orientation
	int rotation; 

	// parameter for type 
	int type; 
} block;


block getRdm();
void printblock(block b);
char down(block* b);
void removeblock(block b);
void turnblock(block* b);
void left(block* b);
void right(block* b);
int convertx(int gridx);
int converty(int gridy);
void checkrow();



#endif