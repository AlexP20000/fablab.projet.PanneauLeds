# Web app client API

All the requests are made using the GET method unless noted otherwise.

## Display text
Display text on the screen. If the text doesn't fit on the screen it will be scrolled vertically.

- **URL**

  `/text`
  
- **URL Parameters**

  `text`: The text to be displayed. Only US ASCII characters are supported. All text will be displayed as upper case.

  `brightness`: Set the brightness of the screen. Accepts a number between 0 and 1.
  
  `color`: Set the color of the text. Accepts a name of a color. For a list of colors see [here](neopixel_matrix/color.py).
  
- **Examples**

  Display "Hello world!" in amber color at medium brightness.  
  `/text?text=Hello+world%21&color=amber&brightness=0.5`

## Reset
Reset (clear) the screen. Deletes the currently displayed text.

- **URL**

  `/reset`

## Display our IP
Set the local WiFi IP as our currently displayed text.

- **URL**

  `/ip`
  
## LOL mode
Fill the screen with an animation of colorful pixels.

- **URL**

  `/lulz`
  
## Illuminate
Turns on all the pixels on the display as bright white in full brightness.

- **URL**

  `/illuminate`

## Auto sleep scheduling
Control if and when the screen will automatically turn off and on.

- **URL**  
  `/sched`

- **URL Parameters**  
  `enable`: If set to `on` this functionality will be enabled. Otherwise it will be disabled.
  
  `offhour`: The hour at which the screen will turn off.
  
  `onhour`: The hour at which the screen will turn on.

- **Notes**  
  The format for hour is in the form of "HH:MM". For example 8:00 needs to be 08:00. Also make sure to take into account the URL [percent encoding](https://developer.mozilla.org/en-US/docs/Glossary/percent-encoding) for ":". The tools you use will most likely handle this for you.

- **Examples**  
  Disable this functionality: `/sched`
  
  Enable, turn the display on at 9am and turn it off at 5pm:  
  `/sched?enable=1&onhour=09%3A00&offhour=17%3A00`
