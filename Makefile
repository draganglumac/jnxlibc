all: main_artifact shipping

main_artifact:
	gcc -Wall -fPIC -c src/*.c
	gcc -shared -W1,-soname,libjnxc.so -o libjnxc.so.1.0 -ldl *.o
	
shipping:
	rm *.o
	find . -type f -name "*~" -exec rm {} \;
	mkdir -p lib; mv libjnxc.so.1.0 lib;
	mkdir -p lib/jnxc_headers; cp src/*.h lib/jnxc_headers;
	
install:
	mv lib/libjnxc.so.1.0 /usr/lib/libjnxc.so.1.0; 
	cp -rf lib/jnxc_headers /usr/include;
	ln -sf /usr/lib/libjnxc.so.1.0 /usr/lib/libjnxc.so
	ln -sf /usr/lib/libjnxc.so.1.0 /usr/lib/libjnxc.so.1
	rm -rf lib
	echo "Installed successfully"