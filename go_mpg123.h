#ifndef __GO_PLAY_LIST_H
#define __GO_PLAY_LIST_H


#include <ao/ao.h>
#include <mpg123.h>
#include <stdlib.h>
#include <stdio.h>

extern void play_finish();

void play_music_file(char *file);

off_t get_file_length(char *file);

int set_volume(double vol);

#endif
