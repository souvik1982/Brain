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

#include "TH1F.h"
#include "TH2F.h"

class Brain
{
  public:
  
    Neurons neurons_;
    
    TH1F *h_potentials_;
    TH2F *h_synapticStrengths_, *h_distances_;
    
  private:
  
    Brain(int size, int debug = 0, float timestep = 1);
    Brain(Brain brain, double mutability, int debug = 0);
    ~Brain();
    void stepInTime(int debug = 0);
    void print();
    void drawPotentials();
    void drawSynapticStrengths();
    void drawDistances();
};
