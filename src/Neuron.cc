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

#include <iostream>

#include "TRandom3.h"

#include "../interface/Neuron.h"

TRandom3 *r3=new TRandom3();

Neuron::Neuron()
{
  activationThreshold_=0.4;
  synapticReinforcement_=0.8;
  synapticDecay_=0.999; // 0.999
  potentialDecay_=0.99; // 0.9
  
  potential_=0;
  potential_buffer_=0;
  spontaneousRate_=0.05; // 0.05
}

void Neuron::push_back_relation(NeuralRelation *relation)
{
  neuralRelations_.push_back(relation);
}

void Neuron::receive(double charge)
{
  potential_buffer_+=charge;
  if (potential_buffer_>1.) potential_buffer_=1.;
  if (potential_buffer_<-1.) potential_buffer_=-1.;
}

void Neuron::stepInTime1(Neurons *neurons)
{
  if (potential_>=activationThreshold_ || r3->Rndm()<spontaneousRate_) // fire
  {
    double totalSynapticWeight=0;
    for (unsigned int i=0; i<neuralRelations_.size(); ++i) totalSynapticWeight+=neuralRelations_.at(i)->synapticStrength;
    for (unsigned int i=0; i<neuralRelations_.size(); ++i)
    {
      Neuron *targetNeuron=neurons->at(neuralRelations_.at(i)->index);
      double synapticStrength=neuralRelations_.at(i)->synapticStrength;
      targetNeuron->receive(0.5*(synapticStrength/totalSynapticWeight)*(neuralRelations_.at(i)->distance));
      neuralRelations_.at(i)->synapticStrength=synapticStrength+synapticReinforcement_*(1.-synapticStrength);
    }
    potential_=0; // -0.01;
    // std::cout<<"Neuron fired"<<std::endl;
  }
  else
  {
    for (unsigned int i=0; i<neuralRelations_.size(); ++i)
    {
      (neuralRelations_.at(i)->synapticStrength)*=synapticDecay_;
      if (neuralRelations_.at(i)->synapticStrength<1e-6) (neuralRelations_.at(i)->synapticStrength)=1e-6;
    }
    potential_*=potentialDecay_;
    if (fabs(potential_)<1e-6) potential_=0;
  }
}

void Neuron::stepInTime2()
{
  potential_+=potential_buffer_;
  potential_buffer_=0;
}

double Neuron::potential()
{
  return potential_;
}

void Neuron::print()
{
  std::cout<<" === Neuron Properties === "<<std::endl;
  std::cout<<" Current Potential = "<<potential_<<std::endl;
  std::cout<<" Number of connections = "<<neuralRelations_.size()<<std::endl;
  std::cout<<" (connection #, index, synaptic strength, distance) | ";
  for (unsigned int i=0; i<neuralRelations_.size(); ++i)
  {
    std::cout<<i<<", "<<neuralRelations_.at(i)->index
                <<", "<<neuralRelations_.at(i)->synapticStrength
                <<", "<<neuralRelations_.at(i)->distance<<" | ";
  }
  std::cout<<std::endl;
  std::cout<<" === === "<<std::endl;
}

void Neuron::printSimple()
{
  std::cout<<potential_<<", ";
}
