// -----------------------------------------------------------------------------
// Battery API. Based on code by Rabia Alhaffar (UNLICENSE)
// - rlyeh, public domain.

#ifdef _WIN32
#include <winsock2.h>

int battery_(void) {
    SYSTEM_POWER_STATUS ibstatus;

    if (GetSystemPowerStatus(&ibstatus) == FALSE) {
        return 0;
    }

    int level = (ibstatus.BatteryLifePercent != 255) ? ibstatus.BatteryLifePercent : 0;
    int charging = (ibstatus.BatteryFlag & 8) > 0;
    return charging ? +level : -level;
}

#elif defined __linux__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int battery_(void) {
    static int battery_status_handle;
    static int battery_capacity_handle;

    ONCE {
        battery_status_handle = open("/sys/class/power_supply/BAT0/status", O_RDONLY);
        battery_capacity_handle = open("/sys/class/power_supply/BAT0/capacity", O_RDONLY);
    }

    if (battery_status_handle == -1 || battery_capacity_handle == -1) {
        return 0;
    }

    char buffer[512];

    // level
    lseek(battery_capacity_handle, 0, SEEK_SET);
    int readlen = read(battery_capacity_handle, buffer, 511); buffer[readlen < 0 ? 0 : readlen] = '\0';
    int level = atoi(buffer);

    // charging
    lseek(battery_status_handle, 0, SEEK_SET);
    readlen = read(battery_status_handle, buffer, 511); buffer[readlen < 0 ? 0 : readlen] = '\0';
    int charging = strstr(buffer, "Discharging") ? 0 : 1;
    return charging ? +level : -level;
}

#elif defined __APPLE__
#import <Foundation/Foundation.h>
#include <CoreFoundation/CoreFoundation.h>
#import <IOKit/ps/IOPowerSources.h>
#import <IOKit/ps/IOPSKeys.h>

int battery_(void) {
    static CFDictionaryRef psrc;

    ONCE {
        CFTypeRef blob = IOPSCopyPowerSourcesInfo();
        CFArrayRef sources = IOPSCopyPowerSourcesList(blob);
        int sourcesCount = CFArrayGetCount(sources);

        if (sourcesCount > 0) {
            psrc = IOPSGetPowerSourceDescription(blob, CFArrayGetValueAtIndex(sources, 0));
        }
    }

    if(psrc == NULL) return 0;

    int cur_cap = 0;
    CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(psrc, CFSTR(kIOPSCurrentCapacityKey)), kCFNumberSInt32Type, &cur_cap);

    int max_cap = 0;
    CFNumberGetValue((CFNumberRef)CFDictionaryGetValue(psrc, CFSTR(kIOPSMaxCapacityKey)), kCFNumberSInt32Type, &max_cap);

    int level = (int)(cur_cap * 100.f / max_cap);
    int charging = CFDictionaryGetValue(psrc, CFSTR(kIOPSIsChargingKey)) == kCFBooleanTrue;
    return charging ? +level : -level;
}

#else

int battery_(void) {
    return 0;
}

#endif
