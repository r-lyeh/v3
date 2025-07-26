// streams are a powerful concept, kind of a very dynamic arena manager.
// - create any stream on the fly, as long as a custom ID is provided (like factory<<32|item),
// - push as many bytes as needed into stream, or enlarge/shrink its contents dynamically.

// for example, we could record all player actions in a multiplayer game:
// - create as many recordstreams as existing players, then
// - push each players actions into their recordstreams at every frame.
// - when any player respawns, we rewind their recordstream to the last checkpoint and continue recording from there.

// or, we could use streams to cache all assets during level loading:
// - create as many vertexstreams as mesh types we have (see attribs in vertex shaders).
// - while level is loading, push as many objects as needed in their associated vertexstreams.
// - when level is loaded, push each vertexstream to the gpu and get a handle.
// - when level is reloaded, do nothing as the vertexstreams are already preloaded and the handles are still valid.
// - when level is unloaded, drop each vertexstream and delete the gpu handles.

// or a dynamic level streamer
// - we keep players and enemy datas in global levelstreams that are always ready, never dropped,
// - we load environment surroundings into N/NE/E/SE/S/SW/W/NW levelstreams as long player is progressing towards them,
// - we drop levelstreams when the player is far from their locations. and repeat.

// or a logger
// - push as many loglines as needed into a logstream,
// - once some length treshold is met, flush bytes to disk and restart the logstream.

// etc.

extern int   (*stream_push)(uint64_t stream_id, const void *ptr, int len); // pushes blob into stream. returns length before resizing
extern int   (*stream_size)(uint64_t stream_id); // returns current length
extern int   (*stream_grow)(uint64_t stream_id, int newsize); // shrinks or enlarges stream as needed. returns length after resizing
extern void* (*stream_data)(uint64_t stream_id); // returns blob. caution: enlarging operations will invalidate this pointer
extern void  (*stream_drop)(uint64_t stream_id); // frees pushed memory


#if CODE
#pragma once
#include v3_stream_backend

int   (*stream_push)(uint64_t, const void *, int) = stream_push_;
int   (*stream_size)(uint64_t) = stream_size_;
int   (*stream_grow)(uint64_t, int) = stream_grow_;
void* (*stream_data)(uint64_t) = stream_data_;
void  (*stream_drop)(uint64_t) = stream_drop_;
#endif
