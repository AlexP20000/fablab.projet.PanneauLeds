# The Web framework.
from flask import Flask, render_template, request, redirect, url_for
# To get the local IP.
import netifaces as ni
# Signal handling
import sys
import signal
# Import our stuff
from objects import m
#from packagewide import currtext, currbright, currcolor
import packagevars as p
import scheduled


# Entry point into our web app
app = Flask(__name__)


@app.route('/')
def root():
    return render_template('root.html',
                           text=p.currtext,
                           bright=p.currbright,
                           color=p.currcolor,
                           morninghour=scheduled.morninghour,
                           eveninghour=scheduled.eveninghour)


@app.route('/text')
def text():
    """Display text on the screen."""
    #global currtext
    #global currbright
    #global currcolor
    text = request.args.get('text')
    m.reset()
    # The ifs here are so we don't necessarily need all the arguments.
    if text != None:
        p.currtext = request.args.get('text')
    bright = request.args.get('brightness')
    if bright != None:
        p.currbright = float(bright)
    color = request.args.get('color')
    if color != None:
        p.currcolor = color
    if 0 <= p.currbright <= 1:
        m.strip.brightness = p.currbright
    m.text(p.currtext, m.color[p.currcolor])
    return redirect(url_for('root'))


@app.route('/reset')
def reset():
    """Reset (clear) the screen."""
    #global currtext
    p.currtext = ''
    m.reset()
    return redirect(url_for('root'))


@app.route('/ip')
def ip():
    """Display our local WiFI IP."""
    #global currtext
    p.currtext = ourip()
    m.reset()
    m.text(p.currtext, m.color[p.currcolor])
    return redirect(url_for('root'))


@app.route('/lulz')
def lulz():
    """lulzmode: Fill the screen with colorful pixels."""
    m.lulz()
    return redirect(url_for('root'))


@app.route('/illuminate')
def illuminate():
    """Full white illumination mode."""
    #global currbright
    m.reset()
    p.currbright = 1
    m.strip.brightness = p.currbright
    m.illuminate()
    return redirect(url_for('root'))


def ourip():
    """Get our local(?) WiFi IP address."""
    return ni.ifaddresses('wlan0')[ni.AF_INET][0]['addr']


# Display the local IP on startup so poeple can find us.
p.currtext = ourip()
m.text(p.currtext)
# Set the default brightness on startup.
m.strip.brightness = p.currbright
# Start the automatic turning on and off of the screen.
scheduled.sched_start()

# Handle signals
def signal_handler(signal, frame):
    scheduled.sched_stop()
    m.reset()
    sys.exit()

signal.signal(signal.SIGUSR1, signal_handler)
signal.signal(signal.SIGTERM, signal_handler)
