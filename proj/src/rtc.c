#include "rtc.h"

rtc_info time_info; 
int hook_id_rtc = 1;

int (rtc_subscribe_int)() {
    return sys_irqsetpolicy(IRQ_RTC, IRQ_REENABLE, &hook_id_rtc);
}


int (rtc_unsubscribe_interrupts)() {
    return sys_irqrmpolicy(&hook_id_rtc );
}

int (read_rtc) (uint8_t command, uint8_t *out) {
    if (sys_outb(INPUT_REG, command) != 0) 
        return 1;
	if (util_sys_inb(OUTPUT_REG, out) != 0) 
        return 1;
    return 0;
}

int (rtc_in_progress)() {
    uint8_t res;
    if (rtc_read(UPDATE_REG, &res)) 
        return 1;
	return res & UPDATE;
}

int (rtc_binary_count)() {
    uint8_t res;
    if (rtc_read(COUNT_REG, &res)) 
        return 1;
	return res & BINARY;
}

int (rtc_update_time_info) () {
    if (rtc_in_progress() != 0)
         return 1;
    uint8_t out;

    if (rtc_output(RTC_SEC, &out) != 0) 
        return 1;
    time_info.seconds = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_output(RTC_MIN, &out) != 0) 
        return 1;
    time_info.minutes = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_output(RTC_HOURS, &out) != 0) 
        return 1;
    time_info.hours = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_output(RTC_DAY, &out) != 0) 
        return 1;
    time_info.day = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_output(RTC_MONTH, &out) != 0) 
        return 1;
    time_info.month = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    if (rtc_output(RTC_YEAR, &out) != 0) 
        return 1;
    time_info.year = rtc_binary_count() ? out : ((out >> 4) * 10) + (out & 0xF);

    return 0;
}