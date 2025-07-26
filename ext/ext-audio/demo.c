#define V3_IMPLEMENTATION
#include "v3.h"

int main() {
    audio_t s = audio(.url="the_entertainer.ogg"); // "coin.wav");

    speaker_t spk = speaker(.source = s, .loop = false);
    speaker_play(&spk);

    sleep_ss(2);

    speaker_drop(&spk);
    audio_drop(&s);

    puts("Ok");
    return 0;
}

#if 0

int main() {
    audio_t sfx[] = {
        audio("coin.wav", AUDIO_CLIP), // y
        audio("the_entertainer.ogg", AUDIO_CLIP), // y
        audio("wrath_of_the_djinn.xm", AUDIO_CLIP), // n
        audio("coin.wav", AUDIO_STREAM), // n
        audio("the_entertainer.ogg", AUDIO_STREAM), // n
        audio("wrath_of_the_djinn.xm", AUDIO_STREAM), // y
    };

    for( app_create(.title = "hi", "0.25"); app_swap(); ) {
        if( ui_button("play") ) audio_play(sfx[0]);
        if( ui_button("stop") ) audio_stop(sfx[0]);

        int choiceM = ui_buttons(3,"Cclip","Eclip","Wclip");
        if( choiceM ) audio_play(sfx[choiceM-1]);

        int choiceS = ui_buttons(3,"Cstrm","Estrm","Wstrm");
        if( choiceS ) audio_play(sfx[choiceS-1+3]);
    }

    for( int i = 0; i < COUNTOF(sfx); ++i ) audio_drop(sfx[i]);

    return 0;
}

#endif
