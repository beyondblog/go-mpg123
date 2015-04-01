#include "go_mpg123.h"

#define BITS 8

static mpg123_handle *mpg_handle = NULL;
static int done;
static int _init = 0;

static void init();


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
    int r = mpg123_open(mpg_handle, file);
    mpg123_getformat(mpg_handle, &rate, &channels, &encoding);

    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
    mpg123_seek(mpg_handle, 0, SEEK_SET);


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

