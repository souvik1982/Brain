ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

all: Neuron.o

clean:
	rm -rf lib/Neuron.o

Neuron.o: src/Neuron.cc
	c++ -c src/Neuron.cc -c $(ROOTFLAGS)
	mv Neuron.o lib/
