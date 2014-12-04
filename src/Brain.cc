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

#include <iostream>

#include "TRandom3.h"

#include "../interface/Neuron.h"
#include "../interface/Brain.h"
#include "../interface/ToolBox.h"

TRandom3 *r3=new TRandom3();

Brain::Brain(int size, int debug, float timeStep)
{
  if (debug == 1)
  {
    h_potentials_=new TH1F(("h_potentials_"+ftoa(timeStep)).c_str(), "Neural Action Potentials", size, 0, size);
    h_synapticStrengths_=new TH2F(("h_synapticStrengths_"+ftoa(timeStep)).c_str(), "Neural Synaptic Strengths", size, size, 0, size, 0, size);
    h_distances_=new TH2F(("h_distances_"+ftoa(timeStep)).c_str(), "Neural Distances", size, size, 0, size, 0, size);
  }
  for (int i=0; i<size; ++i)
  {
    // A new neuron
    Neuron *neuron=new Neuron;
    
    // Construct its neural relations
    for (int j=0; j<size; ++j)
    {
      // It it connected to 50% of the other neurons.
      // Each synaptic strength and distance parameter 
      // is flat a random number from 0 to 1.
      if (i!=j && r3->Rndm()>0.5)
      {
        NeuralRelation *n = new NeuralRelation;
        n->index = j;
        n->synapticStrength = r3->Rndm();
        neuron->push_back_relation(n);
        
        if (debug == 1)
        {
          h_synapticStrengths_->Fill(i, j, n->synapticStrength);
          h_distances_->Fill(i, j, n->distance);
        }
      }
      if (debug == 1)
      {
        h_potentials_->Fill(i+1);
      }
    }
    
    // Add the neuron to the brain
    neurons_.push_back(neuron);
    
  }
}

Brain::~Brain()
{
  for (unsigned int i=0; i<neurons_.size(); ++i)
  {
    delete neurons_.at(i);
  }
  delete h_potentials_;
  delete h_synapticStrengths_;
  delete h_distances_;
}

void Brain::stepInTime(int debug)
{
  for (unsigned int i=0; i<neurons_.size(); ++i)
  {
    neurons_.at(i)->stepInTime1(&neurons_);
  }
  for (unsigned int i=0; i<neurons_.size(); ++i)
  {
    neurons_.at(i)->stepInTime2();
    if (debug == 1)
    {
      h_potentials_->SetBinContent(i+1, neurons_.at(i)->potential_);
      NeuralRelations *neuralRelations=&(neurons_.at(i)->neuralRelations_);
      for (unsigned int j=0; j<neuralRelations->size(); ++j)
      {
        h_synapticStrengths_->SetBinContent(i, neuralRelations->at(j)->index, neuralRelations->at(j)->synapticStrength);
      }
    }
  }
}
    
void Brain::print()
{
  for (unsigned int i=0; i<neurons_.size(); ++i)
  {
    neurons_.at(i)->printSimple();
  }
  std::cout<<std::endl;
}

void Brain::drawPotentials()
{
  h_potentials_->Draw("CL");
}

void Brain::drawSynapticStrengths()
{
  h_synapticStrengths_->Draw("colz");
}

void Brain::drawDistances()
{
  h_distances_->Draw("colz");
}       
        
