extern struct file_t (*blob)(void *bin, unsigned len);

#if CODE
#pragma once

file_t blob_(void *bin, unsigned len) {
    return ((file_t){bin, len, ifndef(V3_RETAIL, {0})});
}

file_t (*blob)(void *, unsigned) = blob_;

#endif
