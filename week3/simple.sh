#!/bin/bash

read input
case $input in
    yes|y|Yes|YES) echo "Yes!";;
    [nN]*) echo "NO!";;
    *) echo "bad input";;
esac
