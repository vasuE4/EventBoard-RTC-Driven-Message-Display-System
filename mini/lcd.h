#include"types.h"
void writeLCD(u8 data);
void cmdLCD(u8 cmd);
void charLCD(u8 ascii);
void InitLCD(void);
void strLCD(u8 *str);
void u32LCD(u32 n);
void f32LCD(f32 fnum,u8 ndp);

