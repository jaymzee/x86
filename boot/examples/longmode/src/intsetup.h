#define IDT_ENTRIES 256
#define IDT_ADDRESS 0x1000
#define IDT_SIZE (IDT_ENTRIES * sizeof(struct IDT_entry))

void EnableInterrupts(void);
