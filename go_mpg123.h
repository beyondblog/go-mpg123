#ifndef __GO_PLAY_LIST_H
#define __GO_PLAY_LIST_H


#include <ao/ao.h>
#include <mpg123.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    char *Title;
    char *Artist;
    char *Album;
    char *Year;
    char *Comment;
    char *Genre;
;

} mpg123_file_Info;



extern void play_finish();

void play_music_file(char *file);

off_t get_file_length(char *file);

int set_volume(double vol);

mpg123_file_Info* get_file_info(char *file);


void free_file_info(mpg123_file_Info* info);


#endif
