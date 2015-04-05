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

#include "../interface/Brain.h"
#include "../interface/ToolBox.h"

Brain::Brain(int size, int debug, std::string name)
{
  debug_=debug;
  if (decodeDebug(debug_, 2)==1)
  {
    std::cout<<"Initialized histos"<<std::endl;
    h_potentials_=new TH1F(("h_potentials_"+name).c_str(), "Neural Action Potentials", size, 0, size);
    h_synapticStrengths_=new TH2F(("h_synapticStrengths_"+name).c_str(), "Neural Synaptic Strengths", size, size, 0, size, 0, size);
    h_distances_=new TH2F(("h_distances_"+name).c_str(), "Neural Distances", size, size, 0, size, 0, size);
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
        n->distance=r3->Rndm();
        neuron->push_back_relation(n);
        
        if (decodeDebug(debug_, 2)==1)
        {
          h_synapticStrengths_->Fill(i, j, n->synapticStrength);
          h_distances_->Fill(i, j, n->distance);
        }
      }
      if (decodeDebug(debug_, 2)==1) h_potentials_->Fill(i+1);
    }
    
    // Add the neuron to the brain
    neurons_.push_back(neuron);
    
  }
}

Brain::Brain(Brain *parentBrain, int diffBrainSize, int debug, std::string name, double mu_newConnection, double mu_modConnection)
{
  int brainSize=parentBrain->neurons_.size()+diffBrainSize;
  
  debug_=debug;
  if (decodeDebug(debug_, 2)==1)
  {
    std::cout<<"Initialized histos"<<std::endl;
    h_potentials_=new TH1F(("h_potentials_"+name).c_str(), "Neural Action Potentials", brainSize, 0, brainSize);
    h_synapticStrengths_=new TH2F(("h_synapticStrengths_"+name).c_str(), "Neural Synaptic Strengths", brainSize, brainSize, 0, brainSize, 0, brainSize);
    h_distances_=new TH2F(("h_distances_"+name).c_str(), "Neural Distances", brainSize, brainSize, 0, brainSize, 0, brainSize);
  }
  
  if (diffBrainSize==0 || diffBrainSize==-1)
  {
    for (unsigned int i=0; i<brainSize; ++i)
    {
      Neuron *neuron=new Neuron();
      Neuron *parentNeuron=parentBrain->neurons_.at(i);
      NeuralRelations parentNeuralRelations=parentNeuron->neuralRelations_;
      for (unsigned int j=0; j<parentNeuralRelations.size(); ++j)
      {
        double rnd=r3->Rndm();
        if (rnd<mu_newConnection/2.) {} // Kill a connection
        else if (rnd>1.-mu_newConnection/2.) // Make a new connection
        {
          NeuralRelation *neuralRelation=new NeuralRelation;
          do
          {
            neuralRelation->index=int(brainSize*r3->Rndm());
          } while (neuralRelation->index==i);
          neuralRelation->synapticStrength=r3->Rndm();
          neuralRelation->distance=r3->Rndm();
          neuron->push_back_relation(neuralRelation);
        }
        else // Modify the existing connection
        {
          if (parentNeuralRelations.at(j)->index < brainSize-1)
          {
            NeuralRelation *neuralRelation=new NeuralRelation;
            neuralRelation->index=parentNeuralRelations.at(j)->index;
            neuralRelation->synapticStrength=r3->Rndm();
            double newDistance=(parentNeuralRelations.at(j)->distance)+mu_modConnection*(-0.5+r3->Rndm());
            if (newDistance<0) newDistance=0;
            if (newDistance>1) newDistance=1;
            neuralRelation->distance=newDistance;
            neuron->push_back_relation(neuralRelation);
          }
        }
      }
      neurons_.push_back(neuron);
    }
  } 
  else if (diffBrainSize==1)
  {
    Neuron *newNeuron=new Neuron();
    for (unsigned int i=0; i<brainSize-1; ++i)
    {
      Neuron *neuron=new Neuron();
      Neuron *parentNeuron=parentBrain->neurons_.at(i);
      NeuralRelations parentNeuralRelations=parentNeuron->neuralRelations_;
      for (unsigned int j=0; j<parentNeuralRelations.size(); ++j)
      {
        double rnd=r3->Rndm();
        if (rnd<mu_newConnection/2.) {} // Kill a connection
        else if (rnd>1.-mu_newConnection/2.) // Make a new connection
        {
          NeuralRelation *neuralRelation=new NeuralRelation;
          do
          {
            neuralRelation->index=int(brainSize*r3->Rndm());
          } while (neuralRelation->index==i);
          neuralRelation->synapticStrength=r3->Rndm();
          neuralRelation->distance=r3->Rndm();
          neuron->push_back_relation(neuralRelation);
        }
        else // Modify the existing connection
        {
          NeuralRelation *neuralRelation=new NeuralRelation;
          neuralRelation->index=parentNeuralRelations.at(j)->index;
          neuralRelation->synapticStrength=r3->Rndm();
          double newDistance=(parentNeuralRelations.at(j)->distance)+mu_modConnection*(-0.5+r3->Rndm());
          if (newDistance<0) newDistance=0;
          if (newDistance>1) newDistance=1;
          neuralRelation->distance=newDistance;
          neuron->push_back_relation(neuralRelation);
        }
      }
      neurons_.push_back(neuron);
      
      // With 50% probability do a fwd connection of the new neuron to other neurons
      if (r3->Rndm()>0.5)
      {
        NeuralRelation *neuralRelation=new NeuralRelation;
        neuralRelation->index=i;
        neuralRelation->synapticStrength=r3->Rndm();
        neuralRelation->distance=r3->Rndm();
        newNeuron->push_back_relation(neuralRelation);
      }
    }
    neurons_.push_back(newNeuron);
  }
    
  if (decodeDebug(debug_, 2)==1)
  {
    for (unsigned int i=0; i<neurons_.size(); ++i)
    {
      Neuron *neuron=neurons_.at(i);
      NeuralRelations *neuralRelations=&(neuron->neuralRelations_);
      for (unsigned int j=0; j<neuralRelations->size(); ++j)
      {
        NeuralRelation *neuralRelation=neuralRelations->at(j);
        h_distances_->SetBinContent(i, neuralRelation->index, neuralRelation->distance);
      }
    }
  }
}   

Brain::~Brain()
{
  for (unsigned int i=0; i<neurons_.size(); ++i)
  {
    delete neurons_.at(i);
  }
  if (decodeDebug(debug_, 2)==1)
  {
    delete h_potentials_;
    delete h_synapticStrengths_;
    delete h_distances_;
  }
}

void Brain::stepInTime()
{
  for (unsigned int i=0; i<neurons_.size(); ++i)
  {
    neurons_.at(i)->stepInTime1(&neurons_);
  }
  for (unsigned int i=0; i<neurons_.size(); ++i)
  {
    neurons_.at(i)->stepInTime2();
    if (decodeDebug(debug_, 2)==1)
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
        
