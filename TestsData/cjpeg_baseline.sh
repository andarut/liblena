#!/bin/bash

# Check if an argument is provided
if [ -z "$1" ]; then
  echo "Usage: $0 <filename_without_extension>"
  exit 1
fi

# Assign filename argument
FILE="$1"

# Ensure input file exists
if [ ! -f "${FILE}.ppm" ]; then
  echo "Error: ${FILE}.ppm not found!"
  exit 1
fi

# Convert PPM to JPEG with specified quality and sampling
cjpeg -baseline -quality 50 -sample 1x1 "${FILE}.ppm" > "${FILE}.jpg"

# Optimize JPEG and remove metadata
jpegtran -copy none "${FILE}.jpg" > "${FILE}.jpeg"

# Remove intermediate file
rm -f "${FILE}.jpg"

echo "Conversion complete: ${FILE}.jpeg created."
