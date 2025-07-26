#ifndef EDITOR_MESSAGE_H
#define EDITOR_MESSAGE_H

typedef struct editor_message {
    uint64_t timestamp; // used for ordering and reply-id
    const char *to;     // is it a message sent to us? wildcards allowed (eg, `to:renderer.*`)
    const char *from;   // to whom we'll reply, in case of any response. wildcards allowed (eg, `from:game.system*`)
    const char *title;  // what is it about? discard quickly if not interested
    const char *body;   // the full serialized message
} editor_message, editor_message_t;

#define editor_message(...) (ifdef(V3_C,(editor_message),editor_message) { __VA_ARGS__ })

void editor_send(editor_message);

#elif CODE
#pragma once

void editor_send(editor_message m) {

}

#endif
