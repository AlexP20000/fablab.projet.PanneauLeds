I recommned checking out the [library](https://learn.adafruit.com/neopixels-on-raspberry-pi) this library is based on to get a better understanding. 

This library is used through an object. Let's initialize an object represeting a screen of 30 by 25 NeoPixels:
```
import neopixel_matrix
m = neopixel_matrix.NeoMatrix(30, 25)
```
You can manage individual pixels on the screen by modifying a list. Let's set the pixel on the 6th row and 11th column green, and set the pixel on the 11th row and 15th column magenta:
```
m.matrix[5][10] = 0x00ff00
m.matrix[10][15] = (255, 0, 255)
m.show()
```
To clear the screen we use `reset()`. It does some extra things we need and there is no neet for a `show()` after it:
```
m.reset()
```
Now let's display some text:
```
m.text("Hello world!")
```
Let's change the brightness. We can access the object of the library we are based on with `m.strip`:
```
m.strip.brightness = 0.25
m.strip.show()
```
Now let's display the same text but in another color:
```
m.text("Hello world!", (0, 255, 0))
```
Long text that doesn't fit on the screen in scrolled vertically:
```
m.text("Big brown fox jumps over the lazy dog! Let's make this text a tiny little bit longer.")
```

The code of this library isn't long, it has plenty of docstrings and comments so don't hesitate to look at the code to find out more. 
