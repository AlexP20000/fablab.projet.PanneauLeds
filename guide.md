# Wifi
On a Raspberry Pi you can simply use the `raspi-config` command. If you don't have that, the configuration is still pretty simple:

Let's add a WiFi network for our device to connect to. Edit `/etc/network/interfaces` as root and add the following. Double quotes in the password are important.
```
auto wlan0
iface wlan0 inet dhcp
	wpa-ssid "your wifi network name"
	wpa-psk "your wifi password"
```
Next you can set up ssh so you don't need to attach a monitor and a keyboard to your Raspberry Pi but that's optional.

## Chaning the WiFi network
If your WiFi settings changed, you simply redo the steps. On Raspberry Pi,  it is simpler to take out the SD Card and modify its contents from another machine.

## Time server
The NTP server was blocked on my network so as an alternative, I used `htpdate` which gets the time using HTTP.
```
sudo apt-get install htpdate
```
Disable the NTP daemon:
```
sudo systemctl disable systemd-timesyncd.service
```

# Dependencies for the library
```
sudo apt install python3-pip
sudo pip3 install adafruit-circuitpython-neopixel Flask schedule
```

# Installation of the Web app
- We need a WSGI HTTP server:
  ```
  sudo pip3 install gunicorn
  ```
- Put the webapp and the neopixel_matrix directories inside `/opt/`
- Put the systemd service file, `webapp.service`, inside `/etc/systemd/system/`
- Make the webapp start automatically on boot and start it now:
  ```
  sudo systemd daemon-reload
  sudo systemd enable webapp
  sudo systemd start webapp
  ```
