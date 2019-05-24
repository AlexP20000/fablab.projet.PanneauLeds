This project is intended to create a Web interface for a LED display made of a NeoPixel strip.

Specifically, this project was made for an already existing [LED screen](https://uboopenfactory.univ-brest.fr/Les-projets/Panneau-de-leds-pour-course-bmx). We only use the display hardware, the computer hardware and the software is for us to develop.

This project contains 2 main components:

- **neomatrix_pixel**: A Python library that takes a strip of NeoPixels and transforms it into a 2D matrix so you can create a screen. It can also display and scroll text. It builds upon the [NeoPixel library for Raspberry Pi](https://learn.adafruit.com/neopixels-on-raspberry-pi).

- **webapp**: A Web app to control the display from other devices of the local network. It too is written in Python and it uses the Flask framework. 

An [installation guide](guide.md) is available.
