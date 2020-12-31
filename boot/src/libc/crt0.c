#include <crt0.h>
#include <stdlib.h>
#include <sys/serial.h>

char *args[1] = {"boot"};
char *envp[2] = {NULL, NULL};

void _start(void)
{
    COM_Init();
    main(1, args, envp);
}

