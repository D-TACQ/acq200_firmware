# BASE/DTACQ/SRC/dt100/Linux/Makefile


all install release clean:
	(cd Drivers; make $@)
	(cd API; make $@)
	(cd Apps; make $@)


install_package: install
	./bin/ship.dt100 install

release_package:	
	./bin/ship.dt100

