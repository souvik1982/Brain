/* 
==================================================
 BrainInWorld
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
Copyright (C) 11 November 2013 Souvik Das
ALL RIGHTS RESERVED
=================================================
*/

#include <TApplication.h>
#include <TROOT.h>
#include <TStyle.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <TFile.h>

#include <TCanvas.h>
#include <TEllipse.h>
#include <TLine.h>
#include <TRandom3.h>
#include <TTimer.h>

#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>

#include "interface/Bot.h"
#include "interface/Neuron.h"
#include "interface/Brain.h"
#include "interface/Fire.h"
#include "interface/Food.h"
#include "interface/ToolBox.h"

int timeStep=1000;
double worldSize=100;
double regenFood=0.90;

// Debug Levels
// bits: xxxx
// bit 0 = TCanvas visualization
// bit 1 = Verbalization
// bit 2 = Fill histograms
// bit 3 = Draw the histograms
int debug = 0x1;

int main()
{ 
  r3->SetSeed(120);
  
  std::cout<<"debug = "<<debug<<std::endl;
  std::cout<<"visualization = "<<decodeDebug(debug, 0)<<std::endl;
  
  TApplication *myapp=new TApplication("myapp",0,0);
  
  typedef std::vector<Bot*> Bots;
  Bots bots;
  unsigned int nBots=10;
  for (unsigned int i=0; i<nBots; ++i)
  {
    Bot *bot=new Bot(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi, 30, "Bot_"+itoa(i), worldSize, debug);
    bots.push_back(bot);
  }
  std::cout<<"Made bots"<<std::endl;
  
  typedef std::vector<Fire*> Fires;
  Fires fires;
  unsigned int nFires=0;
  for (unsigned int i=0; i<nFires; ++i)
  {
    Fire *fire=new Fire(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi, worldSize);
    fires.push_back(fire);
  }
  
  typedef std::vector<Food*> Foods;
  Foods foods;
  unsigned int nFoods=100;
  for (unsigned int i=0; i<nFoods; ++i)
  {
    Food *food=new Food(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi, worldSize);
    foods.push_back(food);
  }
  std::cout<<"Made food"<<std::endl;
  
  /*std::vector <double> time_vector;
  std::vector <double> avgBrainSize_vector;
  std::vector <double> generation_vector;*/
  
  gStyle->SetPalette(1);
  
  TCanvas *c_World;
  if (decodeDebug(debug, 0)==1)
  {
    c_World=new TCanvas("c_World", "Natural Neural Network in Genetic Algorithm", 700, 700);
    c_World->Range(0,0,worldSize,worldSize);
  }
  
  TCanvas *c_Potential_Histograms;
  TCanvas *c_SynapticStrength_Histograms;
  TCanvas *c_Distance_Histograms;
  if (decodeDebug(debug, 3)==1)
  {
    c_Potential_Histograms=new TCanvas("c_Potential_Histograms", "Brain Data - Neural Potentials", 700, 700);
    c_SynapticStrength_Histograms=new TCanvas("c_SynapticStrength_Histograms", "Brain Data - Synaptic Strengths", 700, 700);
    c_Distance_Histograms=new TCanvas("c_Distance_Histograms", "Brain Data - Neural Distances", 700, 700);
    c_Potential_Histograms->Divide(ceil(bots.size()/3.), 3);
    c_SynapticStrength_Histograms->Divide(ceil(bots.size()/3.), 3);
    c_Distance_Histograms->Divide(ceil(bots.size()/3.), 3);
  }
    
  int time=0;
  int generations=0;
    
  // Time loop
  while (foods.size()>0 && bots.size()>0 && generations<50000)
  {
    
    ++time;
    // double avgBrainSize=0;
    
    for (unsigned int i=0; i<bots.size(); ++i)
    {
      bots.at(i)->seeFood(&foods);
      bots.at(i)->stepInTime();
    }
    for (unsigned int i=0; i<fires.size(); ++i)
    {
      fires.at(i)->moveForward();
    }
    for (unsigned int i=0; i<foods.size(); ++i)
    {
      foods.at(i)->moveForward();
    }
  
    // check for bots eating food
    int nEaten=0;
    int nBots=bots.size();
    for (unsigned int i=0; i<nBots; ++i)
    {
      int eatenFood=-1;
      for (unsigned int j=0; j<foods.size(); ++j)
      {
        double d2=pow(bots.at(i)->x_-foods.at(j)->x_, 2)+pow(bots.at(i)->y_-foods.at(j)->y_, 2);
        if (d2<13)
        {
          eatenFood=j;
          if (decodeDebug(debug, 1)==1) std::cout<<"Bot "<<bots.at(i)->name_<<" ate food "<<j<<std::endl;
          break;
        }
      }
      if (eatenFood!=-1) 
      {
        ++nEaten;
        
        if (decodeDebug(debug, 1)==1) std::cout<<"foods.size() = "<<foods.size()<<" and eatenFood = "<<eatenFood<<std::endl;
        delete *(foods.begin()+eatenFood);
        foods.erase(foods.begin()+eatenFood);
        
        if (r3->Rndm()<regenFood)
        {
          Food *food=new Food(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*pi, worldSize);
          foods.push_back(food);
        }
        
        Bot *bot=new Bot(bots.at(i)->x_, bots.at(i)->y_, bots.at(i)->theta_, bots.at(i));
        bots.push_back(bot);
        ++generations;
        
        // avgBrainSize_vector.push_back(bot->brain_->neurons_.size());
        // time_vector.push_back(time);
        if (decodeDebug(debug, 1)==1) std::cout<<"removed from vector, foods.size() = "<<foods.size()<<std::endl;
      }
    }
      
    for (unsigned int i=0; i<nEaten; ++i)
    {
      delete *(bots.begin());
      bots.erase(bots.begin());
    }
    
    if (decodeDebug(debug, 0)==1 && time%timeStep==0) // Draw visualization
    {
      c_World->cd();
      for (unsigned int i=0; i<bots.size(); ++i) bots.at(i)->draw();
      for (unsigned int i=0; i<fires.size(); ++i) fires.at(i)->draw();
      for (unsigned int i=0; i<foods.size(); ++i) foods.at(i)->draw();
      c_World->Update();
    }
    
    if (decodeDebug(debug, 3)==1 && time%timeStep==0) // Flash histograms
    {
      for (unsigned int i=0; i<bots.size(); ++i)
      {
        c_Potential_Histograms->cd(i+1);
        bots.at(i)->brain_->drawPotentials();
        /*c_Distance_Histograms->cd(i+1);
        bots.at(i)->brain_->drawDistances();
        c_SynapticStrength_Histograms->cd(i+1);
        bots.at(i)->brain_->drawSynapticStrengths();*/
      }
      c_Potential_Histograms->Modified();
      c_Potential_Histograms->Update();
      /*c_SynapticStrength_Histograms->Modified();
      c_SynapticStrength_Histograms->Update();
      c_Distance_Histograms->Modified();
      c_Distance_Histograms->Update();*/
      
      // c_Potential_Histograms->SaveAs("c_Potential_Histograms.png");
      // c_SynapticStrength_Histograms->SaveAs("c_SynapticStrength_Histograms.png");
      // c_Distance_Histograms->SaveAs("c_Distance_Histograms.png");
    }
  }
  
  /*TGraph *g_avgBrainSize=new TGraph(avgBrainSize_vector.size(), &time_vector[0], &avgBrainSize_vector[0]);
  g_avgBrainSize->SetTitle("Average size of brains against time; time steps; Average size of brains");
  TCanvas *c_avgBrainSize=new TCanvas("avgBrainSize", "Average Brain Size", 700, 700);
  g_avgBrainSize->Draw("AL*");
  c_avgBrainSize->SaveAs("c_avgBrainSize.png");
  
  TFile *file=new TFile("AnalyzeThis.root", "recreate");
  g_avgBrainSize->Write();
  file->Close();*/
  
  return 0;
}
    
