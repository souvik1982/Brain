ROOTFLAGS = $(shell root-config --cflags)
ROOTLIBS  = $(shell root-config --libs)

all: BrainInWorld

clean:
	rm -rf BrainInWorld lib/*.o

BrainInWorld: Neuron.o Brain.o Entity.o Fire.o Food.o Bot.o BrainInWorld.o
	c++ lib/Neuron.o lib/Brain.o lib/Entity.o lib/Fire.o lib/Food.o lib/Bot.o lib/BrainInWorld.o -o BrainInWorld $(ROOTFLAGS) $(ROOTLIBS) -O2

Neuron.o: src/Neuron.cc
	c++ -c src/Neuron.cc -c $(ROOTFLAGS) -O2
	mv Neuron.o lib/

Brain.o: src/Brain.cc
	c++ -c src/Brain.cc -c $(ROOTFLAGS) -O2
	mv Brain.o lib/

Entity.o: src/Entity.cc
	c++ -c src/Entity.cc -c $(ROOTFLAGS) -O2
	mv Entity.o lib/

Fire.o: src/Fire.cc
	c++ -c src/Fire.cc -c $(ROOTFLAGS) -O2
	mv Fire.o lib/

Food.o: src/Food.cc
	c++ -c src/Food.cc -c $(ROOTFLAGS) -O2
	mv Food.o lib/

Bot.o: src/Bot.cc
	c++ -c src/Bot.cc -c $(ROOTFLAGS) -O2
	mv Bot.o lib/

BrainInWorld.o: BrainInWorld.cc
	c++ -c BrainInWorld.cc -c $(ROOTFLAGS) -O2
	mv BrainInWorld.o lib/BrainInWorld.o

