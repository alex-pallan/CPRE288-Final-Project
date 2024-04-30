/*
 * music.c
 *
 *  Created on: Apr 27, 2024
 *      Author: pmyehl
 */
#include <open_interface.h>

#define STAR_WARS   1

void playSong(int num){
    oi_play_song(num);
}

int load_song_SW(void){
    unsigned char StarWarsNumNotes = 19;
    unsigned char StarWarsNotes[19] = {55, 55, 55, 51, 58, 55, 51, 58, 55, 0, 62, 62, 62, 63, 58, 54, 51, 58, 55};
    unsigned char StarWarsLen[19] = {32, 32, 32, 20, 12, 32, 20, 12, 32, 32, 32, 32, 32, 20, 12, 32, 20, 12, 32};
    oi_loadSong(STAR_WARS, StarWarsNumNotes, StarWarsNotes, StarWarsLen);
    return 1;
}


