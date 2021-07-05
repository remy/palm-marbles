// Main Include file for SDK 3.5
#include <PalmOS.h>
#include <PalmCompatibility.h>

#include "MarblesSq.h"
#include "grid.h"

//// Should only be used at the start of the game - to set up the grid
//// with defalult values
void InitGrid(void)
{
    UInt16 y,x;

    for (y = GRID_Y_MIN; y < GRID_Y_MAX; y++ )
        for (x = GRID_X_MIN; x < GRID_X_MAX; x++)
        {
            grid[y][x].value = RandN(NUM_BALLS,randomSeed)+1; //1; //(SysRandom ( (TimGetSeconds()*(x+y)) ) % NUM_BALLS) +1; //random();
			//grid[y][x].attributes = SetGridAttributes(x, y);

            if (randomSeed)
            	randomSeed = 0;
        }


}


/*//// Set the initial state of the block based on it's position in the grid
UInt32 SetGridAttributes(UInt16 x, UInt16 y)
{
    UInt32 attrib = 0;

    // top
    if (x == GRID_X_MIN) attrib += LEFT_WALL;

    // left
    if (y == GRID_Y_MIN) attrib += TOP_WALL;

    // right
    if (y == GRID_Y_MAX) attrib += BOTTOM_WALL;

    // bottom
    if (x == GRID_X_MAX) attrib += RIGHT_WALL;

    // don't mark the block as being marked

    return attrib;
}*/

//// Checks whether an x,y coord is inside the X-Y grid
Boolean IsValidCoord(Int32 x, Int32 y)
{
 	Boolean rc = true;

    if (x < GRID_X_MIN) rc = false;
    if (x >= GRID_X_MAX) rc = false;
    if (y < GRID_Y_MIN) rc = false;
    if (y >= GRID_Y_MAX) rc = false;

    return rc;
}


Boolean OutsideGrid(UInt16 x, UInt16 y)
{
    Boolean rc = false;

    if (x > screenWidth) rc = true;
    if (x < 0) rc = true;
    if (y > screenHeight) rc = true;
    if (y < 0) rc = true;

    return rc;
}

Boolean AlignPointsToBlock(UInt16 *x, UInt16 *y)
{
    // cells are 21(width) * 7(n) + 1(border width) * 7(n)
    // the point of this - is so that the user
    // isn't tapping on a line
    Boolean ok = false;

    *y -= topBorderOffset;

    if (!OutsideGrid(*x,*y)) // only care if we're in the board
    {
        *x -= (*x % borderAroundScreen);
        *x /=  borderAroundScreen;

        *y -= (*y % borderAroundScreen);
        *y /= borderAroundScreen;

        ok = true;
    }

    return ok;
}


/***********************************************************************
 *
 * FUNCTION:    MarkTouchingBlocks
 *
 * DESCRIPTION: Sets all those blocks with the same value as the block
 *              speicifed to marked and paints a mask over it
 *
 * PARAMETERS:  x,y - coord of initial value
 *              match_value - the value to match against (could be null
 *                            I use it in the recusive loop)
 *              *total - to return the total found (because of recurse)
 *              optional just_check - true or false to just do a check
 *              (this is used for the final check whether the game is active)
 *
 * RETURNED:    nothing
 *
 * REVISION HISTORY:
 *			Name	Date		Description
 *			----	----		-----------
 *			remy	3/10/02		Initial Revision
 *
 ***********************************************************************/
void MarkTouchingBlocks(Int32 x, Int32 y, Int16 match_value, Int16 *total, Boolean just_check)
{
	if (IsValidCoord(x, y))
	    if ( (!(grid[y][x].attributes & BLOCK_MARKED))
             || (just_check) )
			if (grid[y][x].value == match_value)
        	{
	        	if (!just_check) grid[y][x].attributes += BLOCK_MARKED;
                *total += 1;

	            // left
	            if (!(x == GRID_X_MIN)) MarkTouchingBlocks(x-1, y, match_value, total, just_check); // left wall
	            // right
	            if (!(x == GRID_X_MAX)) MarkTouchingBlocks(x+1, y, match_value, total, just_check); // right wall
	            // up
	            if (!(y == GRID_Y_MIN)) MarkTouchingBlocks(x, y-1, match_value, total, just_check); // top wall
	            // down
	            if (!(y == GRID_Y_MAX)) MarkTouchingBlocks(x, y+1, match_value, total, just_check); // bottom wall
#ifndef ONE_BLOCK
                if (*total == 1)
                    UnMarkBlock(&grid[y][x].attributes);
#endif
	        }
}

void MarkBlock(UInt8 *attributes)
{
	if (!(*attributes & BLOCK_MARKED))
		*attributes += BLOCK_MARKED;
}

void UnMarkBlock(UInt8 *attributes)
{
	if (*attributes & BLOCK_MARKED)
		*attributes -= BLOCK_MARKED;
}
