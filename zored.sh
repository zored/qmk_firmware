#/usr/bin/env bash

case $1 in
 build-win|build)
  DIR=/$PWD ./util/docker_build.sh ergodox_ez:zored
 ;;

 sync)
  git remote add target git@github.com:qmk/qmk_firmware.git;
  git merge target master
 ;;

 *)
  echo Use sync|build
 ;;
esac

