#include <Arduino.h>
#include "tetris.h"

void gridprinter() {
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			Serial.print(grid[i][j]);
		}
		Serial.println(grid[i][15]);
	}

	Serial.println("");
	Serial.println("");
}
void blockprinter(block b, char* str) {
	Serial.print(str);
	Serial.print(": {{"); Serial.print(b.row[0]); Serial.print(", "); Serial.print(b.col[0]); 
	Serial.print("}, {"); Serial.print(b.row[1]); Serial.print(", "); Serial.print(b.col[1]); 
	Serial.print("}, {"); Serial.print(b.row[2]); Serial.print(", "); Serial.print(b.col[2]); 
	Serial.print("}, {"); Serial.print(b.row[3]); Serial.print(", "); Serial.print(b.col[3]); 
	Serial.print("}, r:"); Serial.print(b.rotation); Serial.print(", t:"); Serial.print(b.type); 
	Serial.println("}");
}
void rowprinter(int r) {
	Serial.print("row#"); Serial.print(r); Serial.print("{");
	for (int i = 0; i < 16; ++i)
	{
		Serial.print(grid[r][i]); Serial.print(", ");
	}
	Serial.println("}");
}