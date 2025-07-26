typedef struct audio_handle {
    int dummy;
} audio_handle;

audio_t audio_clip_( const char *pathfile ) {
    audio_handle *a = heap(sizeof(audio_handle), 0);
    return a;
}
audio_t audio_stream_( const char *pathfile ) {
    audio_handle *a = heap(sizeof(audio_handle), 0);
    return a;
}

int audio_play_( audio_t a, int flags ) {
    return 0;
}
int audio_stop_( audio_t a ) {
    return 0;
}

void audio_init(unsigned flags) {

}
