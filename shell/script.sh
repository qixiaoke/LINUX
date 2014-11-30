#!/bin/bash

echo "cannot include exclamation - ! within double quotes"

printf "%-10s%-5s%-10s\n" 1 2 3

echo -e "1\t1\t2"

echo -e "\e[1;33m this is red text \e[0m"

var="value"
echo "${var}(s)"

echo "PWD:"  $PWD 
echo "UID:"  $UID
echo "SHELL:"  $SHELL
echo "HOME:"  $HOME

echo ${#var}

