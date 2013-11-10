## What's this?
This is an easy program about detecting ean13-barcode, written in C++ and Qt

## Enviroment
I've tested on both Window7 and Fedora19.
I use these tools:
	OpenCV(version>=2.4.3)
	Qt-Creator

## How does it detect?
Every time we put an image to the program, it reads pixels from left to right.
According to the ratio of zebra lines, we can get appropiate numbers.
