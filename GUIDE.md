# Installing
- OS: Installation was tested on Raspbian Stretch Lite
- Connect the Raspberry Pi to a monitor and a keyboard. Default login is `pi` and password `raspberry`.
- Type `sudo raspi-config`
  - Set up your WiFi connection
  - Boot options -> Wait for network (may help us)
  - Set up your timezone
- Make sure the system is up to date: `sudo apt update && sudo apt full-upgrade` 
- (optional) If NTP is blocked on your network, disable the NTP service and install htpdate:  
  `sudo systemctl daemon-reload`  
  `sudo systemctl disable systemd-timesyncd`  
  `sudo apt install ntpdate`
- Now reboot: `sudo reboot`
- Install the OS packages we'll need: `sudo apt install git python3-pip`
- Create a directory for our project: `mkdir ~/git; cd ~/git`
- Download the project: `git clone "https://github.com/AlexP20000/fablab.projet.PanneauLeds"`
- Install library dependencies:  
  `sudo pip3 install adafruit-circuitpython-neopixel`
- Install webapp dependencies:  
  `sudo pip3 install Flask gunicorn[gevent] schedule netifaces`
- Deploy the app `./quickredeploy.sh` (A message about "fail to stop" is okay here)

# Using
- Plug in the power
- Wait a little bit
- The local WiFi IP address is displayed on the screen
- Connect to this IP address using a Web browser
- You can now control the screen

# Updating
- Connect to the device
- Update the OS: `sudo apt update && sudo apt full-upgrade`
- Go to project's git directory: `cd ~/git/fablab.projet.PanneauLeds`
- Download update: `git pull`
- Install the update: `./quickredeploy.sh`

# FAQ
**Can I see and confirm my WiFi password? Without `raspi-config`?**  
As [documented](https://www.raspberrypi.org/documentation/configuration/wireless/wireless-cli.md) on the RPi website, you can see and edit the WiFi settings directly: `sudo nano /etc/wpa_supplicant/wpa_supplicant.conf`

**Can I connect to the panel without an external WiFi AP? Can the panel have its own WiFi AP?**  
You can set up the panel to host its own WiFi access point. This is useful in cases where there are no other suitable access points around. Raspberry Pi website provides a [guide](https://www.raspberrypi.org/documentation/configuration/wireless/access-point.md) for creating a simple WiFi access point on the Raspberry Pi 3. There is another guide [here](https://frillip.com/using-your-raspberry-pi-3-as-a-wifi-access-point-with-hostapd/). The "add routing and masquerade" part proably isn't needed for our use case. The bridge part is also not needed. You can skip them.

Note that without any internet connection, the Raspberry Pi will not know the time so the functionality to automatically turn off the screen won't work properly.

**Any way to install without git?**  
You can manually copy the files.
- Put the `webapp` and the `neopixel_matrix` directories inside `/opt/`
- Put the systemd service file, `webapp.service` inside `/etc/systemd/system/`
- Make the webapp start automatically on boot and start it now:  
  `sudo systemd daemon-reload`  
  `sudo systemd enable webapp`  
  `sudo systemd start webapp`
