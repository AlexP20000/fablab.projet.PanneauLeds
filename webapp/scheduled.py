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

    
def loop():
    """The loop to be run to turn the screen on and off acoording to schedule.
    It can be terminated by setting the sched_stop_event.
    """
    while True:
        schedule.run_pending()
        if sched_stop_event.is_set():
            schedule.clear()
            sched_thread = None
            break
        time.sleep(1)

def fill():
    """Start the auto sleep scheduling thread."""
    schedule.every().day.at(onhour).do(screenon)
    schedule.every().day.at(offhour).do(screenoff)

    
def sched_clear():
    schedule.clear()

def isalive():
    """Is our scheduler thread alive? Retrun True if yes."""
    j = schedule.jobs
    print(j)
    if not j:
        return False
    else:
        return True


# Hours at which the screen will be automatically turned on or off.
onhour = "09:00"
offhour = "17:00"
# The event to stop the thread.
sched_stop_event = threading.Event()
# Initialize the thread
sched_thread = threading.Thread(target=loop, daemon=True)
