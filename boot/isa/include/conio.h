void SetCursorPos(int x, int y);
void GetCursorPos(int *x, int *y);
void ClearScreen(unsigned char attr);
void LocateText(int x, int y, const char *str, unsigned char attr);
void DisplayText(const char *str);
void EnableBlink(void);
void DisableBlink(void);
