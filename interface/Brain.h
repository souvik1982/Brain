/* 
==================================================
 Brain
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
Copyright (C) 11 November 2013 Souvik Das
ALL RIGHTS RESERVED
=================================================
*/

#include "Neuron.h"

#include "TH1F.h"
#include "TH2F.h"

class Brain
{
  public:
  
    Neurons neurons_;
    
    TH1F *h_potentials_;
    TH2F *h_synapticStrengths_, *h_distances_;
    
    int debug_;
    std::string name_;
    
  public:
  
    Brain(int size, int debug = 0, std::string name = "0");
    Brain(Brain *brain, int diffBrainSize, int debug = 0, std::string name = "0");
    ~Brain();
    void stepInTime();
    
    Neurons* getNeurons();
    std::string getName();
    TH1F* get_h_potentials();
    TH2F* get_h_synapticStrengths();
    TH2F* get_h_distances();
    
    void print();
    void drawPotentials();
    void drawSynapticStrengths();
    void drawDistances();
};
