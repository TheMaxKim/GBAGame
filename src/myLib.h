typedef unsigned int u32;
typedef unsigned short u16;

#define REG_DISPCNT *(u16 *) 0x4000000

#define SCANLINECOUNTER *(volatile u16 *) 0x4000006

#define MODE3 3
#define BG2_ENABLE (1 << 10)

#define RGB(r, g, b) ((b) << 10 | (g) << 5 | (r))

extern u16 *videoBuffer;
extern u16 __key_curr;
extern u16 __key_prev;

#define OFFSET(r, c, numcols) ((r) * (numcols) + (c))


//Input ===========================================
#define BUTTON_A (1 << 0)
#define BUTTON_B (1 << 1)
#define BUTTON_SELECT (1 << 2)
#define BUTTON_START (1 << 3)
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)
#define BUTTON_UP (1 << 6)
#define BUTTON_DOWN (1 << 7)
#define BUTTON_R (1 << 8)
#define BUTTON_L (1 << 9)

#define KEY_MASK 0x03FF

#define KEY_DOWN_NOW(key) (~(BUTTONS) & key)

#define BUTTONS *(volatile u32 *)0x4000130

typedef struct {
	int row;
	int col;
	int size;
	u16 color;
} PLAYEROBJ;

typedef struct {
	int row;
	int col;
	int size;
	int isAlive;
	u16 color;
} BULLET;

typedef struct {
	int row;
	int col;
	int speed;
	int size;
	int isAlive;
	int health;
	u16 color;
} ENEMY;

// DMA

typedef struct {
	const volatile void *src;
	volatile void *dst;
	volatile u32 cnt;
} DMAREC;

#define DMA ((volatile DMAREC *) 0x040000B0)

#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

#define DMA_REPEAT (1 << 25)

#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

#define DMA_NOW () << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

#define DMA_IRQ (1 << 30)
#define DMA_ON (1 << 31)

#define START_ON_FIFO_EMPTY 0x30000000

// Prototypes

void setPixel(int x, int y, u16 color);
void drawRect(int x, int y, int height, int width, volatile u16 color);
void fillScreen(u16 color);
void drawPictureBackground(const u16 arr[]);
void drawImage3(int x, int y, int height, int width, const u16* image);
void waitForVblank();

void key_poll();
u32 key_curr_state();

u32 key_prev_state();

u32 key_is_down(u32 key);

u32 key_is_up(u32 key);

u32 key_was_down(u32 key);

u32 key_was_up(u32 key);

u32 KEY_HIT(u32 key);

u32 KEY_RELEASED(u32 key);












