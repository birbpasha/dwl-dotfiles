#!/bin/sh
udiskie --tray &
$HOME/.config/sway/scripts/lid_switch.sh &
swaync &
waybar &
nm-applet &
sleep 1 && dropbox &
wl-paste --type text --watch cliphist store &
wl-paste --type image --watch cliphist store &
swww-daemon &
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &
