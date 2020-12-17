# bootloader that switches to long mode with paging and runs 64-bit C code

memory map

    0x00300000 0x00400000 stack for protected mode 1M
    0x00008000 0x002FFFFF program memory 3M - 32K
    0x00007e00 0x00007fff stage 1 bootblock
    0x00007c00 0x00007dff stage 0 bootblock
    0x00007000 0x00007bff stack for real mode 3K
    0x00006000 0x00006fff PT upper half of memory
    0x00005000 0x00005fff PT lower half of memory
    0x00004000 0x00004fff PDT
    0x00003000 0x00003fff PDPT
    0x00002000 0x00002fff PML4T
    0x00001000 0x00001fff IDT (long mode)
    0x00000000 0x000003ff IVT (real mode)
