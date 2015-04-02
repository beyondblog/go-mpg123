package mpg123

/*
#cgo pkg-config: libmpg123 ao
#cgo LDFLAGS:
#include "go_mpg123.h"
*/
import "C"

import "unsafe"

var done = make(chan int)

func PlayMusic(file string) {
	go func() {
		musicFile := C.CString(file)
		defer C.free(unsafe.Pointer(musicFile))
		C.play_music_file(musicFile)
	}()

	<-done
}

func GetLength(file string) int32 {
	musicFile := C.CString(file)
	defer C.free(unsafe.Pointer(musicFile))
	return int32(C.get_file_length(musicFile))
}

func SetVolume(volume float64) int {
	return int(C.set_volume(C.double(volume)))
}

//export play_finish
func play_finish() {
	done <- 1
}
