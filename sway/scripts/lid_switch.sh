#!/bin/bash

exec acpi_listen | while read -r event; do
  if [[ "$event" == *"LID close"* ]]; then
    swaylock -C .config/sway/swaylock.conf -f
    systemctl suspend
  fi
done

