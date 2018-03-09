/* 
==================================================
 Neuron
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
Copyright (C) 11 November 2013 Souvik Das
ALL RIGHTS RESERVED
=================================================
*/

#pragma once

#include <vector>

struct NeuralRelation
{
  int index;
  double synapticStrength;
  double distance;
};
typedef std::vector<NeuralRelation*> NeuralRelations;

class Neuron;
typedef std::vector<Neuron*> Neurons;

class Neuron
{
  public:
    double activationThreshold_;
    double synapticReinforcement_;
    double synapticDecay_;
    double potentialDecay_;
  
    double potential_;
    double potential_buffer_;
    double spontaneousRate_;
    NeuralRelations neuralRelations_;
    NeuralRelations v_firedAtThis_;
    
  public:
  
    // Constructor
    Neuron();
    
    // Update the neuron
    void push_back_relation(NeuralRelation *relation);
    void receive(double charge);
    void stepInTime1(Neurons *neurons);
    void stepInTime2();
    
    // Debug the neuron
    double potential();
    void print();
    void printSimple();
};
