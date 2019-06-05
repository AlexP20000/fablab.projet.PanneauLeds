"""For scheduling tasks such as turning off the display in the evening."""

import schedule
import time
import threading
# The screen
from objects import m
import packagevars as p


def screenon():
    """Turn on the screen with brightness set in currbright."""
    m.strip.brightness = p.currbright
    m.strip.show()
    print("Turned on the screen")

    
def screenoff():
    """Turn off the screen while keeping the old brightness value in currbright.
    """
    print("screenoff")
    m.strip.brightness = 0
    m.strip.show()
    print("Turned off the screen")

    
def sched_loop():
    """The loop to be run to turn the screen on and off acoording to schedule.
    Loop can be terminated by setting the sched_stop_event.
    """
    schedule.every().day.at(morninghour).do(screenon)
    schedule.every().day.at(eveninghour).do(screenoff)
    while True:
        schedule.run_pending()
        time.sleep(1)
        if sched_stop_event.is_set():
            schedule.clear()
            break

        
def sched_start():
    """Start the auto sleep scheduling thread."""
    sched_thread = threading.Thread(target=sched_loop, daemon=True)
    sched_thread.start()

    
def sched_stop():
    """Terminate the auto sleep scheduling thread."""
    sched_stop_event.set()
    if sched_thread != None:
        sched_thread.join()
    sched_stop_event.clear()

# Hours at which the screen will be automatically turned on or off.
morninghour = "08:00"
eveninghour = "17:00"

# The event to stop the thread.
sched_stop_event = threading.Event()
