#!/bin/bash +x

./configure
make
cd test
if [ "$1" == "DEBUG" ]
then
	valgrind --version
	if [  $? -ne 0 ]; then
		echo "Please install valgrind to use library debugging"
	else
		./run_tests_valgrind
	fi
else
	./run_tests
fi
result=$?
if [ ! $result -eq 0 ]; then
	echo "Error during testing: Aborting installation"
	cd ../ 
	exit 1
fi
cd ../

sudo make install
echo "Installed succesfully"
