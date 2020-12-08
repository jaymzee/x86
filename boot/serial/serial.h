/*
 * 8250 original IBM PC UART (limited to 9600 bps)
 * 16450 same arch but faster bps (dos okay at 9600 but not a multitasking OS)
 * 16550 has a 16 byte FIFO
 * 16750 has a 64 byte FIFO
 */

void COM_Init();

// is serial transmitter empty
int COM_IsTxEmpty();

// blocking
void COM_WriteChar(char a);

// does serial receive have data
int COM_IsRxReady();

// blocking
char COM_ReadChar();

void COM_WriteString(const char *str);

