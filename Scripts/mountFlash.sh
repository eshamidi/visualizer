#!/bin/sh

mkdir -p /media/es/fdrive
mount -o rw,users,umask=000 /dev/sdb /media/es/fdrive
