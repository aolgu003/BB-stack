#!/usr/bin/env bash

add-apt-repository ppa:levi-armstrong/qt-libraries-xenial
add-apt-repository ppa:levi-armstrong/ppa
apt-get update 
apt-get install -y qt57creator-plugin-ros
add-apt-repository --remove ppa:beineri/opt-qt57-xenial
add-apt-repository --remove ppa:beineri/opt-qt571-xenial
ln -s /opt/qt57/bin/qtcreator /usr/local/bin/

