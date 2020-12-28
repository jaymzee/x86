#include <conio.h>
#include <stdio.h>
#include <string.h>
#include "keyboard.h"

static void DumpKeycode(int keycode)
{
    char sbuf[80];
    char nbuf[20];

    strcpy(sbuf, "0x");
    strcat(sbuf, itoa(keycode, 16, 2, nbuf));
    strcat(sbuf, " ");
    if (keycode >= 0 && keycode < 128) {
        char ch;
        if (vkbd.lshft || vkbd.rshft) {
            ch = kbd_decode[keycode + 128];
        } else {
            ch = kbd_decode[keycode];
        }
        if (ch > 31 && ch < 127) {
            if (ch >= 'a' && ch <= 'z' && (vkbd.lctrl || vkbd.rctrl)) {
                strcat(sbuf, "CTRL+");
                ch -= 32;
            } else if (ch >= 'a' && ch <= 'z' && (vkbd.lalt || vkbd.ralt)) {
                strcat(sbuf, "ALT+");
                ch -= 32;
            }
            char str[] = " \n";
            str[0] = ch;
            strcat(sbuf, str);
        }
    }
    fputs(sbuf, console);
}

void KeyboardHandlerM(void) {
    int keycode = ScanKeyboard();

    switch (keycode) {
    case 0x2a:
        vkbd.lshft = 1;
        break;
    case 0xaa:
        vkbd.lshft = 0;
        break;
    case 0x36:
        vkbd.rshft = 1;
        break;
    case 0xb6:
        vkbd.rshft = 0;
        break;
    case 0x1d:
        if (vkbd.state == 0) {
            vkbd.lctrl = 1;
        } else if (vkbd.state == 1) {
            vkbd.rctrl = 1;
        } else if (vkbd.state == 2) {
            vkbd.state = 3;
        }
        break;
    case 0x9d:
        if (vkbd.state == 0) {
            vkbd.lctrl = 0;
        } else if (vkbd.state == 2) {
            vkbd.state = 3;
        }
        break;
    case 0x38:
        vkbd.lalt = 1;
        break;
    case 0xb8:
        vkbd.lalt = 0;
        break;
    case 0xe0:
        vkbd.state = 1;
        break;
    case 0xe1:
        vkbd.state = 2;
        break;
    case 0x45:
        if (vkbd.state == 0) {
            vkbd.numlock = 1;
        } else if (vkbd.state == 3) {
            // pause pressed (part 1)
            vkbd.state = 0;
        }
        break;
    case 0xc5:
        if (vkbd.state == 0) {
            vkbd.numlock = 0;
        } else if (vkbd.state == 3) {
            // pause pressed (part 2)
            vkbd.state = 0;
        }
        break;
    default:
        if (keycode >= 0 && keycode < 128) {
            DumpKeycode(keycode);
        }
        break;
    }
}

