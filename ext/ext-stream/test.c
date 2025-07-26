AUTOTEST {
    // create some random streams
    enum {
        WORDS,
        MESH_VERTEXSTREAM_2F2F,
        MESH_VERTEXSTREAM_3F3F4UB,
    };

    test(stream_size(WORDS) == 0);
    test(stream_data(WORDS) == 0);

    stream_push(WORDS, "hello", 5);
    stream_push(WORDS, "world", 5+1);
    test(stream_size(WORDS) == 11);

    stream_grow(WORDS, 36);
    test(stream_size(WORDS) == 36);

    test(strcmp(stream_data(WORDS), "helloworld") == 0);

    stream_drop(WORDS);
    test(stream_size(WORDS) == 0);
    test(stream_data(WORDS) == 0);
}


AUTOTEST {
    enum { MUSIC = 123 }; // create a new category for music
    enum { TRACK_01 = 1, TRACK_02 }; // create our music assets

    #define id64(factory, item) (((uint64_t)(factory)) << 32 | (item))

    // cache contents
    // we're only caching filenames here, but could be the decoded audio samples instead.
    stream_push(id64(MUSIC, TRACK_01), "hello_menu.mp3", 14+1);
    stream_push(id64(MUSIC, TRACK_02), "hello_credits.mp3", 17+1);

    test( 0 == strcmp(stream_data(id64(MUSIC, TRACK_01)), "hello_menu.mp3"));
    test( 0 == strcmp(stream_data(id64(MUSIC, TRACK_02)), "hello_credits.mp3"));

    strcpy(stream_data(id64(MUSIC, TRACK_02)), "hello_ingame.mp3"); // patch

    test( 0 == strcmp(stream_data(id64(MUSIC, TRACK_01)), "hello_menu.mp3"));
    test( 0 == strcmp(stream_data(id64(MUSIC, TRACK_02)), "hello_ingame.mp3"));

    #undef id64
}
