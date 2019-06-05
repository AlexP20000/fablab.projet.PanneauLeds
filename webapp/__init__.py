# The Web framework.
from flask import Flask, render_template, request, redirect, url_for
# To get the local IP.
import netifaces as ni
# Signal handling
import sys
import signal
# Import our stuff
from objects import m
import packagevars as p
import scheduled


# Entry point into our web app
app = Flask(__name__)


@app.route('/')
def root():
    """The root page of this app."""
    return render_template('root.html',
                           text=p.currtext,
                           bright=p.currbright,
                           color=p.currcolor,
                           morninghour=scheduled.onhour,
                           eveninghour=scheduled.offhour,
                           sched_alive=scheduled.isalive())


@app.route('/text')
def text():
    """Display text on the screen."""
    m.reset()
    # The ifs here are so we don't necessarily need all the arguments.
    text = request.args.get('text')
    if text != None:
        p.currtext = request.args.get('text')

    bright = request.args.get('brightness')
    if bright != None:
        p.currbright = float(bright)
    if 0 <= p.currbright <= 1:
        m.strip.brightness = p.currbright
    
    color = request.args.get('color')
    if color != None:
        p.currcolor = color

    m.text(p.currtext, m.color[p.currcolor])
    return redirect(url_for('root'))


@app.route('/reset')
def reset():
    """Reset (clear) the screen."""
    p.currtext = ''
    m.reset()
    return redirect(url_for('root'))


@app.route('/ip')
def ip():
    """Display our local WiFI IP."""
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
    m.reset()
    p.currbright = 1
    m.strip.brightness = p.currbright
    m.illuminate()
    return redirect(url_for('root'))

@app.route('/sched')
def sched():
    """Control if and when the screen will automatically turn off and on."""
    reqenable = request.args.get('enable')
    scheduled.morninghour = request.args.get('onhour')
    scheduled.eveninghour = request.args.get('offhour')
    scheduled.sched_stop()
    if reqenable != '0':
        scheduled.sched_start()
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
