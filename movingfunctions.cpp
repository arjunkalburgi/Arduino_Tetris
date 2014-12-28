#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
#include <SD.h>
#include "string.h"
#include "tetris.h"

#include <lcd_image.h>    // U of A Utils


// draw (black) all four components of the block and set the corresponding grid spots to 0
void removeblock(block b) {
	for (int i = 0; i < 4; ++i) {
		tft.drawRect(8*b.col[i], 8*b.row[i], 8, 8, ST7735_BLACK);
		tft.fillRect(8*b.col[i], 8*b.row[i], 8, 8, ST7735_BLACK);
		grid[b.row[i]][b.col[i]] = 0;
	}
}

// draw (white) all four components of the block and set the corresponding grid spots to 1
void printblock(block b) {
	for (int i = 0; i < 4; ++i) {
		tft.drawRect(8*b.col[i], 8*b.row[i], 8, 8, ST7735_WHITE);
		tft.fillRect(8*b.col[i], 8*b.row[i], 8, 8, ST7735_WHITE);
		grid[b.row[i]][b.col[i]] = 1;
	}
}

char down(block* b) {
	// get the new positions
	block nb = (*b);
	for (int i = 0; i < 4; ++i)
	{
		nb.row[i] = nb.row[i] + 1;
	}

	// check if the new spots are filled
		char spot1 = 0; char spot2 = 0; char spot3 = 0; char spot4 = 0;
		if (nb.type == 1) {
			spot1 = grid[nb.row[3]][nb.col[3]];
			spot2 = grid[nb.row[2]][nb.col[2]];
		} else if (nb.type == 2) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[2]][nb.col[2]];
			}
		} else if (nb.type == 3) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			}
		} else if (nb.type == 4) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[1]][nb.col[1]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[0]][nb.col[0]];
			}
		} else if (nb.type == 5) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[2]][nb.col[2]];
			}
		} else if (nb.type == 6) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[1]][nb.col[1]];
				spot4 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[2]][nb.col[2]];
				spot4 = grid[nb.row[3]][nb.col[3]];
			}
		} else if (nb.type == 7) {
			if (nb.rotation == 1) {
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[2]][nb.col[2]];
				spot4 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot2 = grid[nb.row[0]][nb.col[0]];
				spot4 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
				spot4 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 4) {
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			}
		}
		
	if (spot1 || spot2 || spot3 || spot4 || nb.row[0] == 20 || nb.row[1] == 20 || nb.row[2] == 20 || nb.row[3] == 20) {
		// the spot we want to drop to is full, so the block
		// should not drop.
		return false;
	} else {
		// the spot we want to drop to is empty,

		// update the grid (necessary)
		for (int i = 0; i < 4; ++i) {
			grid[nb.row[i] - 1][nb.col[i]] = 0;
		}
		for (int i = 0; i < 4; ++i) {
			grid[nb.row[i]][nb.col[i]] = 1;
		}

		// update the position of the block,
		*b = nb;

		return true;
	}
}

void left(block* b) {
	// get the new positions
	block nb = (*b);
	for (int i = 0; i < 4; ++i)
	{
		nb.col[i] = nb.col[i] - 1;
	}
	
	// check if the new spots are filled
		char spot1 = 0; char spot2 = 0; char spot3 = 0; char spot4 = 0;
		if (nb.type == 1) {
			spot1 = grid[nb.row[0]][nb.col[0]];
			spot2 = grid[nb.row[2]][nb.col[2]];
		} else if (nb.type == 2) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[2]][nb.col[2]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[0]][nb.col[0]];
			}
		} else if (nb.type == 3) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			}
		} else if (nb.type == 4) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[1]][nb.col[1]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			}
		} else if (nb.type == 5) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[2]][nb.col[2]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			}
		} else if (nb.type == 6) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[2]][nb.col[2]];
				spot4 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[1]][nb.col[1]];
				spot4 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[0]][nb.col[0]];
			}
		} else if (nb.type == 7) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot3 = grid[nb.row[1]][nb.col[1]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			}
		}
	if (spot1 || spot2 || spot3 || spot4 || nb.col[0] < 0 || nb.col[1] < 0 || nb.col[2] < 0 || nb.col[3] < 0) {
		// the spot we want to move to is full!
	} else {
		// the spot we want to drop to is empty,
		// update the position of the block,
		*b = nb;
	}
}

void right(block* b) {
	// get the new positions
	block nb = (*b);
	for (int i = 0; i < 4; ++i)
	{
		nb.col[i] = nb.col[i] + 1;
	}
	
	// check if the new spots are filled
		char spot1 = 0; char spot2 = 0; char spot3 = 0; char spot4 = 0;
		if (nb.type == 1) {
			spot1 = grid[nb.row[3]][nb.col[3]];
			spot2 = grid[nb.row[1]][nb.col[1]];
		} else if (nb.type == 2) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
			}
		} else if (nb.type == 3) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[2]][nb.col[2]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			}
		} else if (nb.type == 4) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[3]][nb.col[3]];

			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[2]][nb.col[2]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			}
		} else if (nb.type == 5) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[1]][nb.col[1]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[2]][nb.col[2]];
				spot2 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[0]][nb.col[0]];
			}
		} else if (nb.type == 6) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
				spot3 = grid[nb.row[2]][nb.col[2]];
				spot4 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[1]][nb.col[1]];
				spot4 = grid[nb.row[0]][nb.col[0]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[3]][nb.col[3]];
			}
		} else if (nb.type == 7) {
			if (nb.rotation == 1) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 2) {
				spot1 = grid[nb.row[1]][nb.col[1]];
				spot2 = grid[nb.row[0]][nb.col[0]];
				spot3 = grid[nb.row[3]][nb.col[3]];
			} else if (nb.rotation == 3) {
				spot1 = grid[nb.row[0]][nb.col[0]];
				spot2 = grid[nb.row[1]][nb.col[1]];
			} else if (nb.rotation == 4) {
				spot1 = grid[nb.row[3]][nb.col[3]];
				spot2 = grid[nb.row[2]][nb.col[2]];
				spot3 = grid[nb.row[1]][nb.col[1]];
			}
		}

	if (spot1 || spot2 || spot3 || spot4 || nb.col[0] == 16 || nb.col[1] == 16 || nb.col[2] == 16 || nb.col[3] == 16) {
		// the spot we want to move to is full!
	} else {
		// the spot we want to drop to is empty,
		// update the position of the block,
		*b = nb;
	}
}


// LEVEL FOUR
void turnblock(block* b) {
	// get the new positions and check if the new spots are filled
		block nb = (*b);

		char spot1 = 0; char spot2 = 0; char spot3 = 0; char spot4 = 0;
		// big code.
			if (nb.type == 1) {
				// it's a square, don't need to do anything
			} else if (nb.type == 2) {
				if (nb.rotation == 1) {
					// straight L to |-
					
					nb.row[0] = nb.row[0] + 1;
					nb.col[0] = nb.col[0] + 2;
					// nb.row[1] = nb.row[1] + 0;
					nb.col[1] = nb.col[1] + 1;
					nb.row[2] = nb.row[2] - 1;
					// nb.col[2] = nb.col[2] + 2;
					// nb.row[3] = nb.row[3] - 1;
					nb.col[3] = nb.col[3] - 1;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot2 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 2;
				} else if (nb.rotation == 2) {
					// |- to mirrored r.
					
					nb.row[0] = nb.row[0] + 1;
					nb.col[0] = nb.col[0] - 1;
					// nb.row[1] = nb.row[1] + 0;
					// nb.col[1] = nb.col[1] + 1;
					nb.row[2] = nb.row[2] - 1;
					nb.col[2] = nb.col[2] + 1;
					nb.row[3] = nb.row[3] - 2;
					// nb.col[3] = nb.col[3] + 2;

					// spot1 = grid[nb.row[1]][nb.col[1]];
					spot2 = grid[nb.row[2]][nb.col[2]];
					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 3;
				} else if (nb.rotation == 3) {
					// straight mirrored r to _ _ _|.
					
					// nb.row[0] = nb.row[0] + 2;
					nb.col[0] = nb.col[0] - 2;
					nb.row[1] = nb.row[1] + 1;
					nb.col[1] = nb.col[1] - 1;
					nb.row[2] = nb.row[2] + 2;
					// nb.col[2] = nb.col[2] + 2;
					nb.row[3] = nb.row[3] + 1;
					nb.col[3] = nb.col[3] + 1;

					spot1 = grid[nb.row[0]][nb.col[0]];
					spot2 = grid[nb.row[1]][nb.col[1]];

					nb.rotation = 4;
				} else if (nb.rotation == 4) {
					// straight _ _ _ | to L.
					
					nb.row[0] = nb.row[0] - 2;
					nb.col[0] = nb.col[0] + 1;
					nb.row[1] = nb.row[1] - 1;
					// nb.col[1] = nb.col[1] + 1;
					// nb.row[2] = nb.row[2] - 1;
					nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] + 1;
					// nb.col[3] = nb.col[3] + 2;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 1;
				}
			} else if (nb.type == 3) {
				if (nb.rotation == 1) {
					// mirrored L to |_ _ _
					
					nb.row[0] = nb.row[0] + 2;
					nb.col[0] = nb.col[0] + 1;
					nb.row[1] = nb.row[1] + 1;
					// nb.col[1] = nb.col[1] + 1;
					// nb.row[2] = nb.row[2] - 1;
					nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] - 1;
					// nb.col[3] = nb.col[3] + 1;

					spot1 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 2;
				} else if (nb.rotation == 2) {
					// |_ _ _ to r.
					
					// nb.row[0] = nb.row[0] + 1;
					nb.col[0] = nb.col[0] - 1;
					nb.row[1] = nb.row[1] - 1;
					// nb.col[1] = nb.col[1] + 1;
					nb.row[2] = nb.row[2] - 2;
					nb.col[2] = nb.col[2] + 1;
					nb.row[3] = nb.row[3] - 1;
					nb.col[3] = nb.col[3] + 2;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot2 = grid[nb.row[2]][nb.col[2]];
					spot3 = grid[nb.row[3]][nb.col[3]];

					nb.rotation = 3;
				} else if (nb.rotation == 3) {
					// r to ~~~|.
					
					nb.row[0] = nb.row[0] - 1;
					nb.col[0] = nb.col[0] - 1;
					// nb.row[1] = nb.row[1] - 1;
					// nb.col[1] = nb.col[1] + 1;
					nb.row[2] = nb.row[2] + 1;
					nb.col[2] = nb.col[2] + 1;
					nb.row[3] = nb.row[3] + 2;
					// nb.col[3] = nb.col[3] + 1;

					spot2 = grid[nb.row[2]][nb.col[2]];
					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 4;
				} else if (nb.rotation == 4) {
					// ~~~| to mirrored L.
					
					nb.row[0] = nb.row[0] - 1;
					nb.col[0] = nb.col[0] + 1;
					// nb.row[1] = nb.row[1] - 1;
					// nb.col[1] = nb.col[1] + 1;
					nb.row[2] = nb.row[2] + 1;
					nb.col[2] = nb.col[2] - 1;
					// nb.row[3] = nb.row[3] - 1;
					nb.col[3] = nb.col[3] - 2;

					spot2 = grid[nb.row[2]][nb.col[2]];
					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 1;
				}
			} else if (nb.type == 4) {
				if (nb.rotation == 1) {
					// steps right to tower left
					
					nb.row[0] = nb.row[0] - 2;
					// nb.col[0] = nb.col[0] + 2;
					nb.row[1] = nb.row[1] - 1;
					nb.col[1] = nb.col[1] - 1;
					// nb.row[2] = nb.row[2] - 1;
					// nb.col[2] = nb.col[2] + 2;
					nb.row[3] = nb.row[3] + 1;
					nb.col[3] = nb.col[3] - 1;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 2;
				} else if (nb.rotation == 2) {
					// tower left to steps right.
					
					nb.row[0] = nb.row[0] + 1;
					nb.col[0] = nb.col[0] + 2;
					// nb.row[1] = nb.row[1] + 0;
					nb.col[1] = nb.col[1] + 1;
					nb.row[2] = nb.row[2] + 1;
					// nb.col[2] = nb.col[2] - 1;
					// nb.row[3] = nb.row[3] - 2;
					nb.col[3] = nb.col[3] - 1;

					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 3;
				} else if (nb.rotation == 3) {
					// steps right to tower left
					
					nb.row[0] = nb.row[0] + 1;
					nb.col[0] = nb.col[0] - 1;
					// nb.row[1] = nb.row[1] - 1;
					// nb.col[1] = nb.col[1] + 2;
					nb.row[2] = nb.row[2] - 1;
					nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] - 2;
					// nb.col[3] = nb.col[3] + 2;

					spot2 = grid[nb.row[2]][nb.col[2]];
					spot3 = grid[nb.row[3]][nb.col[3]];

					nb.rotation = 4;
				} else if (nb.rotation == 4) {
					// tower left to steps right.
					
					// nb.row[0] = nb.row[0] - 2;
					nb.col[0] = nb.col[0] - 1;
					nb.row[1] = nb.row[1] + 1;
					// nb.col[1] = nb.col[1] - 1;
					// nb.row[2] = nb.row[2] + 0;
					nb.col[2] = nb.col[2] + 1;
					nb.row[3] = nb.row[3] + 1;
					nb.col[3] = nb.col[3] + 2;

					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 1;
				}
			} else if (nb.type == 5) {
				if (nb.rotation == 1) {
					// steps left to tower right
					
					nb.row[0] = nb.row[0] - 1;
					nb.col[0] = nb.col[0] + 2;
					// nb.row[1] = nb.row[1] - 1;
					nb.col[1] = nb.col[1] + 1;
					nb.row[2] = nb.row[2] - 1;
					// nb.col[2] = nb.col[2] + 2;
					// nb.row[3] = nb.row[3] + 1;
					nb.col[3] = nb.col[3] - 1;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 2;
				} else if (nb.rotation == 2) {
					// tower left to steps right.
					
					nb.row[0] = nb.row[0] + 2;
					// nb.col[0] = nb.col[0] + 1;
					nb.row[1] = nb.row[1] + 1;
					nb.col[1] = nb.col[1] - 1;
					// nb.row[2] = nb.row[2] + 1;
					// nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] - 1;
					nb.col[3] = nb.col[3] - 1;

					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 3;
				} else if (nb.rotation == 3) {
					// steps right to tower left
					
					// nb.row[0] = nb.row[0] + 1;
					nb.col[0] = nb.col[0] - 1;
					nb.row[1] = nb.row[1] - 1;
					// nb.col[1] = nb.col[1] + 2;
					// nb.row[2] = nb.row[2] - 1;
					nb.col[2] = nb.col[2] + 1;
					nb.row[3] = nb.row[3] - 1;
					nb.col[3] = nb.col[3] + 2;

					spot2 = grid[nb.row[2]][nb.col[2]];
					spot3 = grid[nb.row[3]][nb.col[3]];

					nb.rotation = 4;
				} else if (nb.rotation == 4) {
					// tower left to steps right.
					
					nb.row[0] = nb.row[0] - 1;
					nb.col[0] = nb.col[0] - 1;
					// nb.row[1] = nb.row[1] + 1;
					// nb.col[1] = nb.col[1] - 1;
					nb.row[2] = nb.row[2] + 1;
					nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] + 2;
					// nb.col[3] = nb.col[3] + 1;

					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 1;
				}
			} else if (nb.type == 6) {
				if (nb.rotation == 1) {
					// | to ----
					
					nb.row[0] = nb.row[0] + 2;
					nb.col[0] = nb.col[0] + 2;
					nb.row[1] = nb.row[1] + 1;
					nb.col[1] = nb.col[1] + 1;
					// nb.row[2] = nb.row[2] - 1;
					// nb.col[2] = nb.col[2] + 2;
					nb.row[3] = nb.row[3] - 1;
					nb.col[3] = nb.col[3] - 1;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 2;
				} else if (nb.rotation == 2) {
					// ---- to |
					
					nb.row[0] = nb.row[0] + 1;
					nb.col[0] = nb.col[0] - 2;
					// nb.row[1] = nb.row[1] + 1;
					nb.col[1] = nb.col[1] - 1;
					nb.row[2] = nb.row[2] - 1;
					// nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] - 2;
					nb.col[3] = nb.col[3] + 1;

					spot1 = grid[nb.row[2]][nb.col[2]];
					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 3;
				} else if (nb.rotation == 3) {
					// | to ----
					
					nb.row[0] = nb.row[0] - 1;
					nb.col[0] = nb.col[0] - 1;
					// nb.row[1] = nb.row[1] - 1;
					// nb.col[1] = nb.col[1] + 2;
					nb.row[2] = nb.row[2] + 1;
					nb.col[2] = nb.col[2] + 1;
					nb.row[3] = nb.row[3] + 2;
					nb.col[3] = nb.col[3] + 2;

					spot2 = grid[nb.row[2]][nb.col[2]];
					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 4;
				} else if (nb.rotation == 4) {
					// ---- to |
					
					nb.row[0] = nb.row[0] - 2;
					nb.col[0] = nb.col[0] + 1;
					nb.row[1] = nb.row[1] - 1;
					// nb.col[1] = nb.col[1] - 1;
					// nb.row[2] = nb.row[2] + 0;
					nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] + 1;
					nb.col[3] = nb.col[3] - 2;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot3 = grid[nb.row[3]][nb.col[3]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 1;
				}
			} else if (nb.type == 7) {
				if (nb.rotation == 1) {
					// upside down T to |-
					
					// nb.row[0] = nb.row[0] - 2;
					nb.col[0] = nb.col[0] + 1;
					nb.row[1] = nb.row[1] - 2;
					nb.col[1] = nb.col[1] + 1;
					nb.row[2] = nb.row[2] - 1;
					// nb.col[2] = nb.col[2] - 1;
					// nb.row[3] = nb.row[3] + 1;
					nb.col[3] = nb.col[3] - 1;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot4 = grid[nb.row[0]][nb.col[0]];

					nb.rotation = 2;
				} else if (nb.rotation == 2) {
					// from |- to T
					
					nb.row[0] = nb.row[0] + 1;
					nb.col[0] = nb.col[0] - 1;
					nb.row[1] = nb.row[1] + 1;
					nb.col[1] = nb.col[1] + 1;
					// nb.row[2] = nb.row[2] + 0;
					// nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] - 1;
					nb.col[3] = nb.col[3] - 1;

					spot3 = grid[nb.row[3]][nb.col[3]];

					nb.rotation = 3;
				} else if (nb.rotation == 3) {
					// from T to -|
					
					nb.row[0] = nb.row[0] - 1;
					nb.col[0] = nb.col[0] - 1;
					nb.row[1] = nb.row[1] + 1;
					nb.col[1] = nb.col[1] - 1;
					// nb.row[2] = nb.row[2] + 0;
					// nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] - 1;
					nb.col[3] = nb.col[3] + 1;

					spot3 = grid[nb.row[3]][nb.col[3]];

					nb.rotation = 4;
				} else if (nb.rotation == 4) {
					// from -| to upside down T
					
					// nb.row[0] = nb.row[0] - 2;
					nb.col[0] = nb.col[0] + 1;
					// nb.row[1] = nb.row[1] - 1;
					nb.col[1] = nb.col[1] - 1;
					nb.row[2] = nb.row[2] + 1;
					// nb.col[2] = nb.col[2] - 1;
					nb.row[3] = nb.row[3] + 2;
					nb.col[3] = nb.col[3] + 1;

					spot1 = grid[nb.row[1]][nb.col[1]];
					spot3 = grid[nb.row[3]][nb.col[3]];

					nb.rotation = 1;
				}
			}

	// BUG FOUND. If the new spots are not on the grid, it updates anyway.
	char spot5 = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (nb.row[i] > 19 || nb.col[i] < 0 || nb.col[i] > 15) {
			spot5 = 1;
			break;
		}
	}

	if (spot1 || spot2 || spot3 || spot4 || spot5) {
		// the spot we want to move to is full!
	} else {
		// the spot we want to drop to is empty,
		// update the position of the block,
		*b = nb;
	}
}