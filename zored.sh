#/usr/bin/env bash
set -ex

case $1 in
 build-win|build)
  rm *.hex || true
  docker-machine start || true
  eval $(docker-machine env) || true
  DIR=/$PWD ./util/docker_build.sh ergodox_ez:zored
 ;;

 sync)
  git remote add target git@github.com:qmk/qmk_firmware.git || true
  git fetch target master:master
  git merge target/master
 ;;

 flash)
  teensy=/d/zored/downloads/teensy_loader_cli.exe
  # wget https://www.pjrc.com/teensy/teensy_loader_cli_windows.zip -O teensy.zip
  # unzip $_ -d .
  cat <<TEXT

===========

ENTER BOOTLOADER ON YOUR ERGODOX
- Either push a little button on top-right.
- Or hold SPACE + B on reconnect.

TEXT

  $teensy -mmcu=atmega32u4 -w ergodox_ez_zored.hex
 ;;

 bf)
  $0 build
  $0 flash
 ;;

 *)
  echo Use sync|build
  exit 1
 ;;
esac

