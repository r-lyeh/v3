extern int (*battery)(void); // [0..100] level, [+]charging, [-]discharging

#if CODE
#pragma once
#include v3_battery_backend

int (*battery)(void) = battery_;
#endif
