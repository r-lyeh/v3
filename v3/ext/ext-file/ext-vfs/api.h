bool (*vfs_mount)(const char *pathfile); // only .zip supported at the moment

#ifdef CODE
#pragma once
#include v3_vfs_backend

bool (*vfs_mount)(const char *pathfile) = vfs_mount_;
#endif
