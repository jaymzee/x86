#include "serial.h"

void Main()
{
    COM_Init();
    COM_WriteString("Welcome to my first bootloader\n");
    COM_WriteString("Stay tuned for enabling protected mode\n");
    while (1) {
        COM_WriteString("\npress a key ");
        char c = COM_ReadChar();
        COM_WriteString("\nyou pressed: ");
        COM_WriteChar(c);
    }
}
