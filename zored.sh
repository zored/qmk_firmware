#/usr/bin/env bash
set -ex

case $1 in
 build-win|build)
  DIR=/$PWD ./util/docker_build.sh ergodox_ez:zored
 ;;

 sync)
  git remote add target git@github.com:qmk/qmk_firmware.git;
  git merge target master
 ;;

 flash)
  teensy=/d/zored/downloads/teensy_loader_cli.exe
  # wget https://www.pjrc.com/teensy/teensy_loader_cli_windows.zip -O teensy.zip
  # unzip $_ -d .
  echo PRESS RESET BUTTON ON YOUR ERGODOX
  echo
  $teensy -mmcu=atmega32u4 -w ergodox_ez_zored.hex
 ;;

 *)
  echo Use sync|build
  exit 1
 ;;
esac

