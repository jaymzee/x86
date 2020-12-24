void SetTextCursor(int x, int y);
void GetTextCursor(int *x, int *y);
void ClearText(unsigned char attr);
void LocateText(int x, int y, const char *str);
void WriteText(const char *str);
void EnableBlink(void);
void DisableBlink(void);
int ScanKeyboard(void);
extern char kbd_decode[128];
