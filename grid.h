//////  Header file for grid functions

//// Grid layout and macros
#define GRID_X_MAX 10
#define GRID_X_MIN 0
#define GRID_Y_MAX 10
#define GRID_Y_MIN 0

/*#define TOP_WALL 1
#define BOTTOM_WALL 2
#define LEFT_WALL 4
#define RIGHT_WALL 8
*/
#define BLOCK_MARKED 1
#define BLOCK_CHANGED 2

// let's make this an option in future
#define NUM_BALLS 4
// this is
#define DEAD_BLOCK NUM_BALLS + 1
#define MASK_BLOCK NUM_BALLS + 2

// nicked this so don't really know what it's for!
//#define borderAroundScreen 16		// should be as big as the biggest object.
#define borderAroundScreen 16
#define topBorderOffset 0

#define screenWidth					160
#define screenHeight				160 - topBorderOffset  // is 145 for 16x16 blocks


#define RandN(N,r)						((((long) SysRandom (r)) * (N)) / ((long) sysRandomMax + 1))

// allow to click on just one block
#undef ONE_BLOCK
//#define ONE_BLOCK


//// Each blob in the grid will be a block
typedef struct {
    UInt8  value;
    Byte  attributes;
} blockType;


blockType grid[GRID_Y_MAX][GRID_X_MAX];
blockType undo_grid[GRID_Y_MAX][GRID_X_MAX];
blockType original_grid[GRID_Y_MAX][GRID_X_MAX];



//// For the images - so that we lock when we load and
//// and unlock when we stop the app
static Handle					ObjectBitmapHandles[NUM_BALLS + 2];
static BitmapPtr				ObjectBitmapPtr[NUM_BALLS + 2];
static WinHandle				ObjectWindowHandles[NUM_BALLS + 2];


void InitGrid(void);
//UInt32 SetGridAttributes(UInt16 x, UInt16 y);
Boolean OutsideGrid(UInt16 x, UInt16 y);
Boolean AlignPointsToBlock(UInt16 *x, UInt16 *y);
Boolean IsValidCoord(Int32 x, Int32 y);
Boolean AlignPointsToBlock(UInt16 *x, UInt16 *y);
void MarkTouchingBlocks(Int32 x, Int32 y, Int16 match_value, Int16 *total, Boolean just_check);
void MarkBlock(UInt8 *attributes);
void UnMarkBlock(UInt8 *attributes);

UInt32 randomSeed;
