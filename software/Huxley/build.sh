#!/bin/bash

# File presets
IF=bin/huxley
OF=bin/huxley.sha1

# Make and clean
make && make clean

# Create binary hash
sha1sum $IF > $OF

# Display
echo ''
echo 'Build complete'
echo ''
cat $OF

exit

