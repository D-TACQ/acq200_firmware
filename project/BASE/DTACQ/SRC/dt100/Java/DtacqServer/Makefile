


JAVAC=javac -d classes


SOURCES= \
	Acq32Wrapper.java \
	CommandLine.java \
	Debug.java \
	DtacqClient.java \
	DtacqComms.java \
	DtacqServer.java \
	Interpreter.java \
	InterpreterFactory.java \
	MasterInterpreterFactory.java \
	PrintableString.java \
	ReadWriteStream.java 


.classes:
	mkdir -p classes
	$(JAVAC) $(SOURCES)

all:	.classes $(CLASSES)
	jar -cvfm DtacqServer.jar DtacqServer-manifest -C classes .
	@echo All Done

clean:
	rm -Rf *.jar classes

release install: all
	mkdir -p ../../Linux/server
	cp DtacqServer.jar ../../Linux/server

.SUFFIXES: .class .java
