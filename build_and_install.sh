#!/bin/bash +x

./configure
make
cd test
./run_tests
result=$?
if [ ! $result -eq 0 ]; then
echo "Error during testing: Aborting installation"
cd ../ 
exit 1
fi
cd ../

sudo make install
echo "Installed succesfully"
