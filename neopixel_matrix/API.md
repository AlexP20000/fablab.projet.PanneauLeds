# Neomatrix_pixel Library API

## `NeoMatrix(x, y, pin, font, color)`

The object to create and control a matrix of NeoPixels.

- **Parameters**

  **Required**  
  `x`: Width of the matrix in pixels.  
  `y`: Height of the matrix in pixels.  
  
  **Optional**  
  `pin`: What pin to use for the NeoPixel data line.  
  `color`: What color dictionnary to use.
  
- **Examples**

  Create a screen of 30 by 25 NeoPixels:
  ```
  import neopixel_matrix 
  m = neopixel_matrix.NeoMatrix(30, 25)
  ```

### `x`

Width of the screen.
	
### `y`

Height of the screen.
	
### `text(text, color)`

Display text on the screen. If the text doesn't fit on the screen it will be scrolled vertically.

- **Parameters**

  **Required**  
  `text`: A string of the text to dislay.

  **Optional**  
  `color`: The NeoPixel color value.
