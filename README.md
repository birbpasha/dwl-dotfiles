# birbpasha DWL Dotfiles

NOTE: I don't know if this will work for PC (or any) users since i'm only on my macbook and only made for me I am not going to maintain this repo anytime soon

## Requirements:

Note: Those are all optional if you want to make a custom setup
1. waybar (top bar)
2. fuzzel (launcher and clipboard viewer)
3. cliphist, wl-clipboard (clipboard history)
4. grim, slurp (screenshot tools)
5. waypaper (wallpaper selector)
6. bemoji (emoji selector)

## To mimic my setup:

1. Clone DWL repository on codeberg and put my config.h included inside dwl-configs to the cloned DWL repository. and compile it with "make" and "sudo make install" while inside the dwl repository directory
2. Put (or replace) everything inside the repo (not including dwl-configs directory) to your home folder's .config directory
3. Autostart works with this command "dwl -s $HOME/.config/sway/scripts/dwlas". create an automatic script that will run this when logging in
4. Done! If i missed something (considering i am not very experienced with git and github) please forgive me

## Keybinds

SUPER + Return = Launches the default terminal provided in config.h

SUPER + D = Launches fuzzel application launcher

SUPER + L = Swaylock

SUPER + Q = Closes focused window

SUPER + T = Toggle floating for focused window

SUPER + F = Toggle fullscreen for focused window

SUPER + W = Wallpaper selector (waypaper)

SUPER + Period = Emoji selector (bemoji)

SUPER + B = Launches the default browser provided inside config.h

SUPER + E = Launches defualt file manager provided inside config.h

SUPER + V = Clipboard history

SUPER + SHIFT + S = Screenshot selection

SUPER + 1 to 9 = Change workspace between 1-9 (couldnt configure waybar to show workspaces)

SUPER + Increase Volume = Shutdown device **!!!(delete line 165 and 166 inside config.h if you don't want this)!!!**

SUPER + SHIFT + Increase Volume = Restart device

## Screenshots

<img width="1280" height="799" alt="shot_2025-09-05_13-16-45" src="https://github.com/user-attachments/assets/1ea6adcc-7857-43f1-8822-41707dff2344" />
<img width="1280" height="800" alt="shot_2025-09-05_13-19-26" src="https://github.com/user-attachments/assets/8ac3bbaf-9d50-4080-9486-65e23541b1af" />
<img width="1280" height="800" alt="shot_2025-09-05_13-19-38" src="https://github.com/user-attachments/assets/40359206-956e-470b-9e49-39f05b838592" />
