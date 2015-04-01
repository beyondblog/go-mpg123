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

//export play_finish
func play_finish() {
	done <- 1
}
