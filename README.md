This project is intended to create a Web interface for a LED display panel made from a NeoPixel strip.

Specifically, this project was made for an already existing [LED panel](https://uboopenfactory.univ-brest.fr/Les-projets/Panneau-de-leds-pour-course-bmx). We only use the display hardware, the computer hardware and the software is for us to develop.

This project is made of 2 main components:
- A Python library that takes a strip of NeoPixels and transforms it into a 2D matrix so you can create a screen. It can also display and scroll text. It builds upon the [NeoPixel library for Raspberry Pi](https://learn.adafruit.com/neopixels-on-raspberry-pi). (neomatrix_pixel)
- A Web app to control the display from other devices of the local network. It too is written in Python and it uses the Flask framework. (webapp)

Docs:
- [Installation guide](GUIDE.md)
- [Web app API](webapp/API.md)
- [Library guide](neopixel_matrix/README.md)

# The architecture
```
                +--------------+      +---------------+
                |              |      |               |
  Web     HTTP  | Gunicon WSGI | WSGI | Flask Web App |
browser <------>+ Web server   +<---->+    /webapp    |
                |              |      |               |
                +--------------+      +-------+-------+
                                              ^
                                 Python object|
                                              v
                                   +----------+-------+
                                   |                  |
                                   |      Library     |
                                   | /neopixel_matrix |
                                   |                  |
                                   +------------------+
```
