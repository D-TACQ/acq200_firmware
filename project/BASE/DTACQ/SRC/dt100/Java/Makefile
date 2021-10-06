# BASE/DTACQ/SRC/dt100/Linux/Makefile


all install release clean:
	(cd ./DtacqServer;make $@)
	(cd ./dt100rc;make $@)

install_package: install
	./bin/ship.dt100 install

release_package:	
	./bin/ship.dt100

