struct RTC {
    unsigned char second;
    unsigned char minute;
    unsigned char hour;
    unsigned char day;
    unsigned char month;
    unsigned int year;
};

void RTC_GetDate(struct RTC *now);
