import board
import neopixel
import time
import random # for lulz function
import threading
from neopixel_matrix import striptomatrix
from neopixel_matrix import bitmaps
from neopixel_matrix import color

# default color
defcolor = color.color['amber']

class NeoMatrix:
    def __init__(self, x, y, pin=board.D18,
                 font=bitmaps.font1, color=color.color,
                 zigzag=True, topright=True):
        """Initilizes an instance of NeoMatrix class

        x, y: Sizes of matrix in pixels.
        pin:  The pin used for signal. ex: board.D18
        color: The dictionary that contains color definitions.
        zigzag, topright: How is the LED strip wired to be a 2D screen?
            Zigzag or straight? Is the 1st pixel on top right or top left? 
            straight, topleft:
            1 2 3
            4 5 6
            7 8 9
            zigzag, topleft:
            1 2 3
            6 5 4
            7 8 9
            straight, topright:
            3 2 1
            6 5 4
            9 8 7
            zigzag, topright:
            3 2 1
            4 5 6
            9 8 7
        """
        # Size of the matrix in pixels.
        self.x = x
        self.y = y
        # This NeoPixel object creates a LED strip.
        # You can access its goodies from this obeject.
        self.strip = neopixel.NeoPixel(pin, x*y, auto_write=False)
        # We need to use different functions to transform between the LED
        # strip and the 2D screen depending on how the strip is wired. 
        funcs = striptomatrix.decide_functions(zigzag, topright)
        self.striptomatrix = funcs[0]
        self.matrixtostrip = funcs[1]
        # Create the 2D list that represents our matrix.
        # You can modify individual pixels by modifying self.matrix[y][x]
        # Don't forget to self.show() for changes to take effect.
        self.matrix = self.striptomatrix(self.strip, x)
        # The default font.
        self.font = font
        # The scrollng process
        self.scrollproc = None
        # Include the colors in the object
        self.color = color
        # Sentinel, set True to shut down the scroll thread.
        self.sentinel = False
        # Event set when the scroll thread is dead. This is so we don't have 2
        # scrolling threads running at the same time after setting new text.
        self.killedevent = threading.Event()
        self.killedevent.set()

    def show(self):
        """Makes what's on the matrix show up on the strip."""
        tmpstrip = self.matrixtostrip(self.matrix)
        for i in range(len(tmpstrip)):
            self.strip[i] = tmpstrip[i]
        self.strip.show()

    def clear(self):
        """Go through the matrix and set the pixels to dark.
        Note: show() is needed for this to show up on screen. (not doing show()
        here is useful in other functions."""
        for y in range(len(self.matrix)):
            for x in range(len(self.matrix[0])):
                self.matrix[y-1][x-1] = (0,0,0)

    def reset(self):
        """Reset the screen, terminate the scrolling process."""
        self.sentinel = True
        self.killedevent.wait()
        self.clear()
        self.show()

    def insert_bitmap(self, a, xpos, ypos, color=defcolor):
        """Insert a bitmap into a draw buffer. Needs show() to take effect
        on the screen.
        a: the "bitmap", 2 dimensional list of 1 or 0
        x, y: position of the screen
        color: the neopixel color tuple"""
        for y in range(len(a)):
            matrix_y = ypos + y
            for x in range(len(a[y])):
                matrix_x = xpos + x
                # If the pixel is 1 in the bitmap and if it's is inside the
                # the matrix. Otherwise don't write it on the matrix.
                if (a[y][x] == 1 and
                    matrix_x >= 0 and matrix_y >= 0 and
                    matrix_x < self.x and matrix_y < self.y
                ):
                    self.matrix[matrix_y][matrix_x] = color

    def insert_line(self, text, x, y, color=defcolor):
        """Insert a line. What doesn't fit will be truncated."""
        for i in range(len(text)):
            x2 = x + (i * (self.font['width'] + 1))
            if text[i] in self.font:
                self.insert_bitmap(self.font[text[i]], x2, y, color)
            else:
                self.insert_bitmap(self.font['?'], x2, y, color)

    def insert_lines(self, lines, color=defcolor):
        """Inserts line in the lines list. Intended forx my particular hardware.
        lines: list (or tuple) of lines"""
        for i in range(len(lines)):
            self.insert_line(lines[i], 0, i*(self.font['height']+1), color)
        
            
    def scroll_text(self, text, color=defcolor, scrolldelay=0.25):
        """Scroll text vertically. This scrolls 1 pixel at a time.
        Works when there is more lines than the screen has.
        This function is an endless loop. Launch it in a separate process."""
        # Line height including spacing.
        lineh = self.font['height'] + 1
        # Calculate characters we can have per line.
        chars_per_line = self.x // (self.font['width'] + 1)
        # Split the text into its lines.
        linelist = striptomatrix.listto2d(text, chars_per_line)
        # Add an empty line at the end of the list.
        linelist.append(" " * chars_per_line)
        # Initialize the vertical offset.
        voffset = 0
        # Reset the sentinel
        self.sentinel = False
        self.killedevent.clear()
        while self.sentinel == False:
            self.clear()
            
            # Uncomment to understand how this works.
            #print('{0} voffset={1}'.format(linelist, voffset))

            # Insert the lines, taking into account the vertical offset. 
            for l in range(len(linelist)):
                self.insert_line(linelist[l], 0, (l*lineh)-voffset, color)    
            self.show()
            time.sleep(scrolldelay)
            # Upate the offset value.
            # After we scrolled 1 char worth, 
            if voffset == self.font['height']:
                voffset = 0
                # Place the last item to the beginnig of the list.
                linelist.append(linelist[0])
                del linelist[0]
            else:
                voffset += 1
        self.killedevent.set()

    def text(self, text, color=defcolor):
        """General text command.

        If the text doesn't fit on the screen it will be scrolled vertically.
        """
        # Line height including spacing.
        lineh = self.font['height'] + 1
        # Calculate characters we can have per line.
        chars_per_line = self.x // (self.font['width'] + 1)
        # Split the text into its lines.
        linelist = striptomatrix.listto2d(text, chars_per_line)
        # How many lines can we fit on the screen?
        maxlines = self.y // (self.font['height'] + 1)
        # If the text doesn't fit the screen, scroll:
        if len(linelist) > maxlines:
            self.reset()
            self.scrollproc = threading.Thread(target=self.scroll_text,
                                               args=(text, color))
            self.killedevent.wait()
            self.scrollproc.start()
        # If the text does fit
        else:
            self.insert_lines(linelist, color)
            self.show()

    def lulz(self):
        """Fill the screen with an animation of colorful pixels."""
        self.reset()
        self.scrollproc = threading.Thread(target=self.lulzloop)
        self.killedevent.wait()
        self.scrollproc.start()

    def lulzloop(self):
        """The background loop for the lulz."""
        self.sentinel = False
        while self.sentinel == False:
            for i in range(len(self.strip)):
                self.strip[i] = (random.randint(0, 255),
                                 random.randint(0, 255),
                                 random.randint(0, 255))
            self.strip.show()
            time.sleep(0.025)
        self.killedevent.set()

    def illuminate(self):
        """Illumination mode."""
        self.strip.brightness = 1
        for i in range(len(self.strip)):
            self.strip[i] = (255, 255, 255)
        self.strip.show()
