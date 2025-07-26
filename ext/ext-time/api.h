// ----------------------------------------------------------------------------
// time

extern uint64_t    (*date)(void);        // YYYYMMDDhhmmss
extern uint64_t    (*date_epoch)(void);  // linux epoch
extern char*       (*date_string)(void); // "YYYY-MM-DD hh:mm:ss"

extern double      (*time_hh)(void);
extern double      (*time_mm)(void);
extern double      (*time_ss)(void);
extern uint64_t    (*time_ms)(void);
extern uint64_t    (*time_us)(void);
extern uint64_t    (*time_ns)(void);

extern void        (*sleep_ss)(double ss);
extern void        (*sleep_ms)(double ms);
extern void        (*sleep_us)(double us);
extern void        (*sleep_ns)(double us);

extern void        (*yield)(void);

#if CODE
#pragma once
#include v3_time_backend

uint64_t    (*date)(void) = date_;        // YYYYMMDDhhmmss
uint64_t    (*date_epoch)(void) = date_epoch_;  // linux epoch
char*       (*date_string)(void) = date_string_; // "YYYY-MM-DD hh:mm:ss"

double      (*time_hh)(void) = time_hh_;
double      (*time_mm)(void) = time_mm_;
double      (*time_ss)(void) = time_ss_;
uint64_t    (*time_ms)(void) = time_ms_;
uint64_t    (*time_us)(void) = time_us_;
uint64_t    (*time_ns)(void) = time_ns_;

void        (*sleep_ss)(double) = sleep_ss_;
void        (*sleep_ms)(double) = sleep_ms_;
void        (*sleep_us)(double) = sleep_us_;
void        (*sleep_ns)(double) = sleep_ns_;

void        (*yield)(void) = yield_;
#endif
