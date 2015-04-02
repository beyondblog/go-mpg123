package main

import (
	"fmt"
	"github.com/beyondblog/go-mpg123"
	"os"
)

func main() {
	args := os.Args
	if len(args) < 2 {
		fmt.Println("usage: mpg123 musicile")
		os.Exit(-1)
	}
	mpg123.SetVolume(0.5)
	mpg123.PlayMusic(args[1])
}
