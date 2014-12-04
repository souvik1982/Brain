ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

all: Neuron.o Brain.o

clean:
	rm -rf lib/*.o

Neuron.o: src/Neuron.cc
	c++ -c src/Neuron.cc -c $(ROOTFLAGS)
	mv Neuron.o lib/

Brain.o: src/Brain.cc
	c++ -c src/Brain.cc -c $(ROOTFLAGS)
	mv Brain.o lib/

