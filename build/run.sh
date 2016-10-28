#!/bin/bash
clear
find .. -name *.cxx -exec ./long_methods.sh {} $* \;
./trains.sh
