#!/bin/bash
docker run -ti -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix vision/odometry python visualOdometrySIM.py
