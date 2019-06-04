This guide is for creating a simple WiFi access point on the Raspberry Pi 3.

`sudo apt install dnsmasq hostapd`  
`sudo nano /etc/dhcpcd.conf`
```
interface wlan0
    static ip_address=192.168.4.1/24
    nohook wpa_supplicant
```
`sudo systemctl restart dhcpcd`
