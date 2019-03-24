#/usr/bin/env bash
set -ex

version=$(git describe --dirty)
hex=${version}.hex
[[ -e $hex ]] && exists='exists' || exists='does not exist'

cat <<TEXT

Current HEX file is: $hex
($exists)

TEXT

case $1 in
 build|b)
  docker-machine start || true
  eval $(docker-machine env) || true
  DIR=/$PWD ./util/docker_build.sh ergodox_ez:zored
  mv ergodox_ez_zored.hex $hex
 ;;

 sync|s)
  git remote add target git@github.com:qmk/qmk_firmware.git || true
  git fetch target master:master
  git merge target/master
 ;;

 flash|f)
  teensy=/d/zored/downloads/teensy_loader_cli.exe
  # wget https://www.pjrc.com/teensy/teensy_loader_cli_windows.zip -O teensy.zip
  # unzip $_ -d .
  cat <<'TEXT'

===========

ENTER BOOTLOADER ON YOUR ERGODOX
- Either push a little button on top-right.
- Or hold SPACE + B on reconnect.
- Press Bootloader button (`ZKC_BTL`)

TEXT

  $teensy -mmcu=atmega32u4 -w $hex
 ;;

 build-and-flash|bf)
  $0 build
  $0 flash
 ;;

 *)
  echo Use sync|build
  exit 1
 ;;
esac

