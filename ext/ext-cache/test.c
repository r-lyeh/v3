
AUTOTEST {
    enum { MUSIC = 123 }; // create a new category for music
    enum { TRACK_01 = 1, TRACK_02 }; // create our music assets

    // cache contents
    // we're only caching filenames here, but could be the audio streams instead.
    cache(MUSIC, TRACK_01, "hello_menu.mp3");
    cache(MUSIC, TRACK_02, "hello_credits.mp3");

    test( !find(0, 0) );
    test( !find(MUSIC, 0) );

    test( 0 == strcmp(find(MUSIC, TRACK_01), "hello_menu.mp3"));
    test( 0 == strcmp(find(MUSIC, TRACK_02), "hello_credits.mp3"));

    cache(MUSIC, TRACK_01, "hello_ingame.mp3"); // patch

    test( 0 == strcmp(find(MUSIC, TRACK_01), "hello_ingame.mp3"));
    test( 0 == strcmp(find(MUSIC, TRACK_02), "hello_credits.mp3"));

    //list(MUSIC, NULL);
    //list(MUSIC-1, NULL);
}
