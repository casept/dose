#!/usr/bin/env bash
set -eo pipefail

sudo apt update -y
sudo apt upgrade -y
sudo apt install -y build-essential
sudo apt install -y python3 python3-pip
sudo pip3 install quom

