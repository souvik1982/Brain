ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

all: Neuron.o Brain.o Entity.o Fire.o Food.o

clean:
	rm -rf lib/*.o

Neuron.o: src/Neuron.cc
	c++ -c src/Neuron.cc -c $(ROOTFLAGS)
	mv Neuron.o lib/

Brain.o: src/Brain.cc
	c++ -c src/Brain.cc -c $(ROOTFLAGS)
	mv Brain.o lib/

Entity.o: src/Entity.cc
	c++ -c src/Entity.cc -c $(ROOTFLAGS)
	mv Entity.o lib/

Fire.o: src/Fire.cc
	c++ -c src/Fire.cc -c $(ROOTFLAGS)
	mv Fire.o lib/

Food.o: src/Food.cc
	c++ -c src/Food.cc -c $(ROOTFLAGS)
	mv Food.o lib/

