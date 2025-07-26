extern struct file_t (*asset)(const char *pathfile); // locates, cooks and reads item. pathfile can be a partial path that will get resolved (case insensitive search)

#if CODE
#pragma once
#include v3_asset_backend

struct file_t (*asset)(const char *) = asset_;
#endif
