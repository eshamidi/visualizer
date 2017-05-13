#!/bin/sh

mkdir -p /media/jere/fdrive
mount -o rw,users,umask=000 /dev/sdc1 /media/jere/fdrive
