#include <stdio.h>
#include <sys/graphics.h>
#include <sys/serial.h>

int main(int argc, char *argv[], char *envp[])
{
    for (int i = 0; i < 200; i++) {
        DrawLine(0, i, 319, i, i);
    }
    println("VGA graphics in protected mode demo");
    while (1) {
        print("\npress a key ");
        int c = getchar();
        print("\nyou pressed: ");
        putchar(c);
    }
}
