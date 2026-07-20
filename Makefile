

build:
	mkdir -p build
	cd build && cmake .. && make

clean:
	rm -rf build temp.dat

rebuild:
	echo "Rebuilding..."
	$(MAKE) clean
	$(MAKE) build
	echo "Done"


