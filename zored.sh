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

 teensy|t)
  mkdir -p vendor/teensy
  pushd $_
  git clone https://github.com/zored/teensy_loader_cli . || true
  git checkout zored || true
  OS=MACOSX make
  popd
 ;;

 flash|f)
  mcu='atmega32u4'

  teensy_vendor=./vendor/teensy/teensy_loader_cli
  if [[ -e $teensy_vendor ]]; then
    teensy="$teensy_vendor --mcu=$mcu -v"
  else
    # wget https://www.pjrc.com/teensy/teensy_loader_cli_windows.zip -O teensy.zip
    # unzip $_ -d .
    teensy="/d/zored/downloads/teensy_loader_cli.exe -mmcu=$mcu"
  fi

  cat <<'TEXT'

===========

ENTER BOOTLOADER ON YOUR ERGODOX
- Either push a little button on top-right.
- Or hold SPACE + B on reconnect.
- Press Bootloader button (`ZKC_BTL`)

TEXT

  $teensy -w $hex
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

