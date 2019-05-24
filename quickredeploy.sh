#!/bin/sh
# WARNING: Removes and copies stuff as root.
# Installs the webapp and the neopixel_matrix to /opt/.
sudo systemctl stop webapp.service
sudo rm -rf /opt/neopixel_matrix/ /opt/webapp/
sudo cp -R ./neopixel_matrix/ /opt/
sudo cp -R ./webapp/ /opt/
sudo mv /opt/webapp/webapp.service /etc/systemd/system/
sudo systemctl daemon-reload
sudo systemctl start webapp.service
