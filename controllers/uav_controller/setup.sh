#! /usr/bin/bash

export WEBOTS_HOME=$HOME/src/webots
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$WEBOTS_HOME/lib/controller

echo './core.%p' | sudo tee /proc/sys/kernel/core_pattern
