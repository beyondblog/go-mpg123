package main

import "C"

import (
	"fmt"
	"github.com/beyondblog/go-mpg123"
	"os"
	//"unsafe"
)

func main() {
	args := os.Args
	if len(args) < 2 {
		fmt.Println("usage: mpg123 musicile")
		os.Exit(-1)
	}
	mpg123.SetVolume(0.5)

	info := mpg123.GetFileInfo(args[1])
	fmt.Println(C.GoString((*C.char)(info.Title)))
	fmt.Println(C.GoString((*C.char)(info.Artist)))
	fmt.Println(C.GoString((*C.char)(info.Album)))
	fmt.Println(C.GoString((*C.char)(info.Year)))
	fmt.Println(C.GoString((*C.char)(info.Comment)))
	fmt.Println(C.GoString((*C.char)(info.Genre)))
	mpg123.FreeFileInfo(info)
	mpg123.PlayMusic(args[1])
}
