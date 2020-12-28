// state
//  0 - wait for byte
//  1 - E0 recvd wait for one more byte
//  2 - E1 recvd wait for 1 of 2 more bytes
//  3 - E1 recvd wait for 2 of 2 more bytes
struct virt_kb {
    int lshft;
    int rshft;
    int lctrl;
    int rctrl;
    int lalt;
    int ralt;
    int numlock;
    int state;
} vkbd;
