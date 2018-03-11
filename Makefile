ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

all: BrainInWorld

clean:
	rm -rf BrainInWorld lib/*.o

BrainInWorld: lib/Neuron.o lib/Brain.o lib/Entity.o lib/Fire.o lib/Food.o lib/Bot.o lib/BrainInWorld.o
	c++ lib/Neuron.o lib/Brain.o lib/Entity.o lib/Fire.o lib/Food.o lib/Bot.o lib/BrainInWorld.o -o BrainInWorld $(ROOTFLAGS) $(ROOTLIBS) -O2

TestNeuron: Neuron.o TestNeuron.o
	c++ lib/Neuron.o lib/TestNeuron.o -o TestNeuron $(ROOTFLAGS) $(ROOTLIBS) -O2

lib/Neuron.o: interface/Neuron.h src/Neuron.cc
	c++ -c src/Neuron.cc -o lib/Neuron.o -c $(ROOTFLAGS) -O2

lib/Brain.o: interface/Brain.h src/Brain.cc
	c++ -c src/Brain.cc -o lib/Brain.o -c $(ROOTFLAGS) -O2

lib/Entity.o: interface/Entity.h src/Entity.cc
	c++ -c src/Entity.cc -o lib/Entity.o -c $(ROOTFLAGS) -O2

lib/Fire.o: interface/Fire.h src/Fire.cc
	c++ -c src/Fire.cc -o lib/Fire.o -c $(ROOTFLAGS) -O2

lib/Food.o: interface/Food.h src/Food.cc
	c++ -c src/Food.cc -o lib/Food.o -c $(ROOTFLAGS) -O2

lib/Bot.o: interface/Bot.h src/Bot.cc
	c++ -c src/Bot.cc -o lib/Bot.o -c $(ROOTFLAGS) -O2

lib/BrainInWorld.o: BrainInWorld.cc
	c++ -c BrainInWorld.cc -o lib/BrainInWorld.o -c $(ROOTFLAGS) -O2

lib/TestNeuron.o: test/TestNeuron.cc
	c++ -c test/TestNeuron.cc -o lib/TestNeuron.o -c $(ROOTFLAGS) -O2
