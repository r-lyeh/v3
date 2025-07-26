#include "3rd/microui.h"

mu_Container *mu_exists(mu_Context *ctx, const char *name);

#if CODE
#pragma once

#ifdef ABSOLUTE
#undef ABSOLUTE
#endif
#ifdef RELATIVE
#undef RELATIVE
#endif
#include "3rd/microui.c"
#undef unused
#undef expect
#undef push
#undef pop
#include "3rd/microui_renderer.c"

mu_Container *mu_exists(mu_Context *ctx, const char *name) {
    mu_Id id = mu_get_id(ctx, name, strlen(name));
    /* try to get existing container from pool */
    int idx = mu_pool_get(ctx, ctx->container_pool, MU_CONTAINERPOOL_SIZE, id);
    if (idx >= 0) return &ctx->containers[idx];
    return NULL;
}

#endif
