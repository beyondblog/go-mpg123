#include "go_mpg123.h"

#define BITS 8

static mpg123_handle *mpg_handle = NULL;
static int done;
static int _init = 0;
static double m_volume;

static void init();

mpg123_file_Info* get_file_info(char *file) {
    init();

    mpg123_id3v1 *v1 = NULL;
    mpg123_id3v2 *v2 = NULL;
    mpg123_file_Info *info = NULL;

    mpg123_open(mpg_handle, file);

    mpg123_scan(mpg_handle);

    int meta = mpg123_meta_check(mpg_handle);

    if(meta & MPG123_ID3 && mpg123_id3(mpg_handle, &v1, &v2) == MPG123_OK)
    {
        info  = (mpg123_file_Info *) malloc(sizeof(mpg123_file_Info));
        memset(info, 0, sizeof(mpg123_file_Info));

        if(v2->title != NULL && v2->title->fill) {
            info->Title = (char *) malloc(v2->title->fill + 1);
            memcpy(info->Title, v2->title->p , v2->title->fill);
            info->Title[v2->title->fill] = '\0';
        }

        if(v2->artist != NULL && v2->artist->fill) {
            info->Artist = (char *) malloc(v2->artist->fill + 1);
            memcpy(info->Artist, v2->artist->p , v2->artist->fill);
            info->Artist[v2->artist->fill] = '\0';
        }

        if(v2->album != NULL && v2->album->fill) {
            info->Album = (char *) malloc(v2->album->fill + 1);
            memcpy(info->Album, v2->album->p , v2->album->fill);
            info->Album[v2->album->fill] = '\0';
        }

        if(v2->year != NULL && v2->year->fill) {
            info->Year = (char *) malloc(v2->year->fill + 1);
            memcpy(info->Year, v2->year->p , v2->year->fill);
            info->Year[v2->year->fill] = '\0';
        }

        if(v2->comment != NULL && v2->comment->fill) {
            info->Comment = (char *) malloc(v2->comment->fill + 1);
            memcpy(info->Comment, v2->comment->p , v2->comment->fill);
            info->Comment[v2->comment->fill] = '\0';
        }

        if(v2->genre != NULL && v2->genre->fill) {
            info->Genre = (char *) malloc(v2->genre->fill + 1);
            memcpy(info->Genre, v2->genre->p , v2->genre->fill);
            info->Genre[v2->genre->fill] = '\0';
        }
    }
    //printf("Title:  %s\n", v2->title);
    //printf("Artist: %s\n", v2->artist);
    //printf("Album: %s\n", v2->album);
    //printf("Year: %s\n", v2->year);
    //printf("Comment: %s\n", v2->comment);
    //printf("Genre: %i\n", v2->genre);
    mpg123_close(mpg_handle);
    return info;
}


void free_file_info(mpg123_file_Info* info) {
    if(info != NULL) {
        if(info->Title != NULL) free(info->Title);
        if(info->Artist != NULL) free(info->Artist);
        if(info->Album != NULL) free(info->Album);
        if(info->Year != NULL) free(info->Year);
        if(info->Comment != NULL) free(info->Comment);
        if(info->Genre != NULL) free(info->Genre);
        free(info);
    }
    info = NULL;
}


off_t get_file_length(char *file)
{
    init();
    mpg123_open(mpg_handle, file);
    off_t length = mpg123_length(mpg_handle);
    mpg123_close(mpg_handle);
    return length;
}

int set_volume(double vol)
{
    m_volume = vol;
    if (mpg_handle != NULL) {
        return mpg123_volume(mpg_handle, m_volume);
    } else {
        return -1;
    }
}

void play_music_file(char *file)
{
    init();
    done = 1;

    int driver;
    ao_device *dev;
    ao_sample_format format;
    int channels, encoding;
    long rate;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;

    done = 0;
    driver = ao_default_driver_id();
    buffer_size = mpg123_outblock(mpg_handle);
    buffer = (unsigned char* )malloc(buffer_size * sizeof(unsigned char));
    mpg123_open(mpg_handle, file);
    mpg123_getformat(mpg_handle, &rate, &channels, &encoding);

    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
    mpg123_seek(mpg_handle, 0, SEEK_SET);

    set_volume(m_volume);

    while(mpg123_read(mpg_handle, buffer, buffer_size, &done) == MPG123_OK) {
        ao_play(dev, (char *)buffer, done);
    }
    free(buffer);
    done = 1;
    ao_close(dev);
    mpg123_close(mpg_handle);

    play_finish(); //callback go
}

static void init()
{
    if (_init != 0)
        return;
    ao_initialize();
    mpg123_init();
    mpg_handle = mpg123_new(NULL, NULL);
    mpg123_param(mpg_handle, MPG123_RESYNC_LIMIT, -1, 0);
    _init = 1;
}

