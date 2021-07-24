## What's this?
This is a project for recognizing ean13-barcode, algorithm written in C++ and GUI programming with Qt

## How does it recogize?
Every time we put an image to the program, it reads pixels from left to right.
According to the ratio of zebra lines, we can get appropiate numbers.

## Enviroment
I've tested on both Window7 and Fedora19, and Ubuntu 20.04
I use these tools:
	OpenCV(version>=2.4.3)
	Qt-Creator

There is both qmake(.pro) and CMake(CMakeLists.txt) build support. Choose the one you prefer. Go to [barcode](barcode) for details.
