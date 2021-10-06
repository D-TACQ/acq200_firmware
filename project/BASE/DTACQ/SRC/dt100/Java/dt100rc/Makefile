
JARS=jars

BIGJAR=$(JARS)/dt100rc.jar

DTACQCOMPONENTS=\
	images/*.gif \
	-C classes .

JF1=../jfreechart-0.9.4/jcommon-0.7.1.jar
JF2=../jfreechart-0.9.4/jfreechart-0.9.4.jar

LIBS=$(JF1) $(JF2)

SYMANTECCOMPONENTS=\
	symantec com


COMPONENTS=\
	$(DTACQCOMPONENTS) \
	$(SYMANTECCOMPONENTS) \

RUNTIME_SCRIPTS=\
	dt100rc \
	DT100RC.BAT \
	default-resources

MANIFEST=dt100rc-manifest

SOURCES=`find . -name \*.java -print`

JAVAC=javac -d classes

all:	jar
	@echo All Done

clean:
	rm -Rf classes jars


install:
	mkdir -p ../../Linux/client/jars
	cp $(JARS)/* ../../Linux/client/jars
	cp $(RUNTIME_SCRIPTS) ../../Linux/client

$(JARS):
	mkdir -p $(JARS)

com/jrefinery:
	jar xf $(JF1)
	jar xf $(JF2)

classes: 
	mkdir -p classes
	$(JAVAC) $(SOURCES)

jar:	com/jrefinery classes $(JARS) 
	mkdir -p $(JARS)
	jar cfm $(BIGJAR) $(MANIFEST) $(COMPONENTS)

release: all install 
	echo release done


ij:
	mkdir libs
	cd libs;ln -s ../com com;ln -s ../symantec symantec

.SUFFIXES: .class .java 
