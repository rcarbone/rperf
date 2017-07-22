#!/bin/sh
#

run=1

pkgs="libstdc++-6-dev"
pkgs="$pkgs gcc-6"
pkgs="$pkgs g++-6"
pkgs="$pkgs libglib2.0-dev"
pkgs="$pkgs libapr1-dev"
pkgs="$pkgs libsparsehash-dev"
pkgs="$pkgs libboost1.62-dev"
pkgs="$pkgs libqt5core5a"
pkgs="$pkgs libtbb-dev"

echo "Checking software requirements for rperf ..."
echo

if [ ! -f /etc/debian_version ]; then
  echo "You are not on a Debian System" && exit 0
fi

# packages longest name
longest=0
for p in $pkgs; do
  l=${#p}
  if [ $longest -lt $l ]; then
    longest=$l
  fi
done

# Main loop to check for all required packages
for p in $pkgs; do

  # Format and print the package name
  name=`echo $longest $p | awk '{ printf ("%-*.*s", $1, $1, $2) }'`
  echo -n "  Package $name ... "

  # Query the Debian Package Manager
  dpkg -l $p 1> /dev/null 2>& 1
  if [ $? = 1 ]; then
    echo "No"
    run=0
  else
    echo "Ok"
  fi
done

echo

if [ $run = 0 ]; then
  echo "Not all required packages are currently installed on this system"
else
  echo "All required packages are already installed on this system"
fi
