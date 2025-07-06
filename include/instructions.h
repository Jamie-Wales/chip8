#define SYS_CALL 0x000
#define CLEAR 0xE0
#define RETURN 0xEE
#define JUMP 0x1000
#define LOADR 0x6000
#define ARITH 0x8000
#define LOADI 0xA000
#define DRAW 0xD000
#define FIRST_NIBBLE(instruction) instruction & 0x00FF;
