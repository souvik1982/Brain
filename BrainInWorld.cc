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
#include "interface/CommandLineArguments.h"

int skipGenerations=0;
int timeStep=200;
double worldSize=100;
double regenFood=1.0;

unsigned int nFoods=5;
unsigned int nBots=10;
unsigned int nPredators=5;

// Mutation parameters
double mu_newNeuron=0; // 0.001;
double mu_newConnection=0.05;
double mu_modConnection=0.05;
double mu_visualAngle=0.05;

// Debug Levels
// bits: xxxx
// bit 0 = TCanvas visualization
// bit 1 = Verbalization
// bit 2 = Fill histograms
// bit 3 = Draw the histograms
int debug = 0x2;

int main(int argc, char *argv[])
{ 
  // Get command line arguments
  std::map<std::string, int> cmdMap=commandLineArguments(argc, argv);
  if (cmdMap.find("-debug")!=cmdMap.end())           debug=cmdMap["-debug"];
  if (cmdMap.find("-skipGenerations")!=cmdMap.end()) skipGenerations=cmdMap["-skipGenerations"];
  if (cmdMap.find("-timeStep")!=cmdMap.end())        timeStep=cmdMap["-timeStep"];
  if (cmdMap.find("-worldSize")!=cmdMap.end())       worldSize=cmdMap["-worldSize"];
  if (cmdMap.find("-nBots")!=cmdMap.end())           nBots=cmdMap["-nBots"];
  if (cmdMap.find("-nFoods")!=cmdMap.end())          nFoods=cmdMap["-nFoods"];
  if (cmdMap.find("-nPredators")!=cmdMap.end())      nPredators=cmdMap["-nPredators"];

  r3->SetSeed(100);
  
  std::cout<<"debug = "<<debug<<std::endl;
  std::cout<<"visualization = "<<decodeDebug(debug, 0)<<std::endl;
  
  TApplication *myapp=new TApplication("myapp",0,0);
  gStyle->SetCanvasPreferGL(true);
  gStyle->SetPalette(1);
  
  typedef std::vector<Bot*> Bots;
  Bots bots;
  for (unsigned int i=0; i<nBots; ++i)
  {
    Bot *bot=new Bot("Bot", r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi, pi/4., 30, kBlue, 1.0, "Bot_"+itoa(i), worldSize, debug);
    bots.push_back(bot);
  }
  std::cout<<"Instantiated bots."<<std::endl;
  
  Bots predators;
  for (unsigned int i=0; i<nPredators; ++i)
  {
    Bot *predator=new Bot("Predator", r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi, pi/4., 30, kRed, 1.0, "Predator_"+itoa(i), worldSize, debug);
    predators.push_back(predator);
  }
  std::cout<<"Instantiated predators."<<std::endl;
  
  typedef std::vector<Food*> Foods;
  Foods foods;
  for (unsigned int i=0; i<nFoods; ++i)
  {
    Food *food=new Food(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi, worldSize);
    foods.push_back(food);
  }
  std::cout<<"Instantiated food."<<std::endl;
  
  std::vector <double> time_vector;
  std::vector <double> time_predator_vector;
  std::vector <double> avgBrainSize_vector;
  std::vector <double> avgBrainSize_predator_vector;
  std::vector <double> generation_vector;
  std::vector <double> generation_predator_vector;
  std::vector <double> dtime_vector;
  std::vector <double> dtime_predator_vector;
  
  gStyle->SetPalette(1);
  
  TCanvas *c_World;
  if (decodeDebug(debug, 0)==1)
  {
    c_World=new TCanvas("c_World", "Natural Neural Network in Genetic Algorithm", 700, 700);
    // Safety Circle
    // TEllipse *e_safe=new TEllipse(worldSize/2., worldSize/2., 70, 70);
    // e_safe->Draw();
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
  int generations_predator=0;
  int oldGeneration=generations;
  int dtime=0;
  int dtime_predator=0;
  
  // Time loop
  while (foods.size()>0)
  {
    
    ++time;
    ++dtime;
    ++dtime_predator;
    
    for (unsigned int i=0; i<bots.size(); ++i)
    {
      bots.at(i)->seeFoods(&foods);
      // bots.at(i)->seeBots(&bots);
      bots.at(i)->seeBots(&predators);
      bots.at(i)->stepInTime();
    }
    for (unsigned int i=0; i<foods.size(); ++i)
    {
      foods.at(i)->moveForward();
    }
    for (unsigned int i=0; i<predators.size(); ++i)
    {
      predators.at(i)->seeFoods(&foods);
      predators.at(i)->seeBots(&bots);
      // predators.at(i)->seeBots(&predators);
      predators.at(i)->stepInTime();
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
          Food *food=new Food(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*(2.*pi), worldSize);
          foods.push_back(food);
        }
        
        Bot *bot=new Bot(bots.at(i), mu_newNeuron, mu_newConnection, mu_modConnection, mu_visualAngle);
        bots.push_back(bot);
        ++generations;
        
        avgBrainSize_vector.push_back(bot->brain_->neurons_.size());
        time_vector.push_back(time);
        generation_vector.push_back(generations);
        dtime_vector.push_back(dtime);
        dtime=0;
        
        if (decodeDebug(debug, 1)==1) std::cout<<"removed from vector, foods.size() = "<<foods.size()<<std::endl;
      }
    }
    for (unsigned int i=0; i<nEaten; ++i)
    {
      delete *(bots.begin());
      bots.erase(bots.begin());
    }
    
    // check for predators eating bots
    int nEatenBots=0;
    int nPredators=predators.size();
    for (unsigned int i=0; i<nPredators; ++i)
    {
      int eatenBot=-1;
      for (unsigned int j=0; j<bots.size(); ++j)
      {
        double d2=pow(predators.at(i)->x_-bots.at(j)->x_, 2)+pow(predators.at(i)->y_-bots.at(j)->y_, 2);
        double d3=pow(bots.at(j)->x_-worldSize/2., 2)+pow(bots.at(j)->y_-worldSize/2., 2);
        if (d2<13) // && d3>4900) // Safe
        {
          eatenBot=j;
          if (decodeDebug(debug, 1)==1) std::cout<<"Predator "<<predators.at(i)->name_<<" ate bot "<<j<<std::endl;
          break;
        }
      }
      if (eatenBot!=-1)
      {
        ++nEatenBots;
        
        if (decodeDebug(debug, 1)==1) std::cout<<"bots.size() = "<<bots.size()<<" and eatenBot = "<<eatenBot<<std::endl;
        delete *(bots.begin()+eatenBot);
        bots.erase(bots.begin()+eatenBot);
        
        Bot *bot=new Bot(bots.at(0), mu_newNeuron, mu_newConnection, mu_modConnection, mu_visualAngle);
        bots.push_back(bot);
        
        Bot *predator=new Bot(predators.at(i), mu_newNeuron, mu_newConnection, mu_modConnection, mu_visualAngle);
        predators.push_back(predator);
        ++generations_predator;
        
        avgBrainSize_predator_vector.push_back(predator->brain_->neurons_.size());
        time_predator_vector.push_back(time);
        generation_predator_vector.push_back(generations_predator);
        dtime_predator_vector.push_back(dtime_predator);
        dtime_predator=0;
      }
    }
    for (unsigned int i=0; i<nEatenBots; ++i)
    {
      delete *(predators.begin());
      predators.erase(predators.begin());
    }
    
    // Draw visualization
    if (decodeDebug(debug, 0)==1 && time%timeStep==0 && generations>skipGenerations)
    {
      c_World->cd();
      for (unsigned int i=0; i<bots.size(); ++i) bots.at(i)->draw();
      for (unsigned int i=0; i<foods.size(); ++i) foods.at(i)->draw();
      for (unsigned int i=0; i<predators.size(); ++i) predators.at(i)->draw();
      c_World->Update();
      // c_World->SaveAs(("Movie/c_World_"+itoa(time)+".png").c_str());
    }
    
    if (decodeDebug(debug, 3)==1 && time%timeStep==0 && generations>skipGenerations) // Flash histograms
    {
      for (unsigned int i=0; i<bots.size(); ++i)
      {
        c_Potential_Histograms->cd(i+1);
        bots.at(i)->brain_->drawPotentials();
        c_Distance_Histograms->cd(i+1);
        bots.at(i)->brain_->drawDistances();
        c_SynapticStrength_Histograms->cd(i+1);
        bots.at(i)->brain_->drawSynapticStrengths();
      }
      c_Potential_Histograms->Modified();
      c_Potential_Histograms->Update();
      c_SynapticStrength_Histograms->Modified();
      c_SynapticStrength_Histograms->Update();
      c_Distance_Histograms->Modified();
      c_Distance_Histograms->Update();
      
      // c_Potential_Histograms->SaveAs("c_Potential_Histograms.png");
      // c_SynapticStrength_Histograms->SaveAs("c_SynapticStrength_Histograms.png");
      // c_Distance_Histograms->SaveAs("c_Distance_Histograms.png");
    }
    
    if (generations%100==0 && generations!=oldGeneration) 
    {
      std::cout<<"Generation "<<generations<<std::endl;
      oldGeneration=generations;
      
      TGraph *g_avgBrainSize_time=new TGraph(avgBrainSize_vector.size(), &time_vector[0], &avgBrainSize_vector[0]);
      g_avgBrainSize_time->SetName("g_avgBrainSize_time");
      g_avgBrainSize_time->SetTitle("; time steps; Average size of brains");

      TGraph *g_avgBrainSize_generation=new TGraph(avgBrainSize_vector.size(), &generation_vector[0], &avgBrainSize_vector[0]);
      g_avgBrainSize_generation->SetName("g_avgBrainSize_generation");
      g_avgBrainSize_generation->SetTitle("; generations; Average size of brains");
      
      TGraph *g_dtime_generation=new TGraph(dtime_vector.size(), &generation_vector[0], &dtime_vector[0]);
      g_dtime_generation->SetName("g_dtime_generation");
      g_dtime_generation->SetTitle("; generations; Time to next meal");
      
      TGraph *g_dtime_time=new TGraph(dtime_vector.size(), &time_vector[0], &dtime_vector[0]);
      g_dtime_time->SetName("g_dtime_time");
      g_dtime_time->SetTitle("; time steps; Time to next meal");
      
      TGraph *g_avgBrainSize_predator_time=new TGraph(avgBrainSize_predator_vector.size(), &time_predator_vector[0], &avgBrainSize_predator_vector[0]);
      g_avgBrainSize_predator_time->SetName("g_avgBrainSize_predator_time");
      g_avgBrainSize_predator_time->SetTitle("; time steps; Average size of predator brains");
      
      TGraph *g_avgBrainSize_predator_generation=new TGraph(avgBrainSize_predator_vector.size(), &generation_predator_vector[0], &avgBrainSize_predator_vector[0]);
      g_avgBrainSize_predator_generation->SetName("g_avgBrainSize_predator_generation");
      g_avgBrainSize_predator_generation->SetTitle("; generations; Average size of predator brains");
      
      TGraph *g_dtime_predator_generation=new TGraph(dtime_predator_vector.size(), &generation_predator_vector[0], &dtime_predator_vector[0]);
      g_dtime_predator_generation->SetName("g_dtime_predator_generation");
      g_dtime_predator_generation->SetTitle("; generations; Time for predator to next meal");
      
      TGraph *g_dtime_predator_time=new TGraph(dtime_predator_vector.size(), &time_vector[0], &dtime_predator_vector[0]);
      g_dtime_predator_time->SetName("g_dtime_predator_time");
      g_dtime_predator_time->SetTitle("; time steps; Time to next meal");
      
      int nSizeMatrix=bots.at(0)->brain_->neurons_.size();
      TH1F *h_spontaneity=new TH1F(("h_spontaneity_"+itoa(generations)).c_str(), "; i^{th} Neuron", nSizeMatrix, 0, nSizeMatrix);
      TH2F *h_distances_matrix=new TH2F(("h_distances_matrix_"+itoa(generations)).c_str(), "; i^{th} Neuron; j^{th} Neuron", nSizeMatrix, 0, nSizeMatrix, nSizeMatrix, 0, nSizeMatrix);
      TH1F *h_distances=new TH1F(("h_distances_Generation_"+itoa(generations)).c_str(), "; distance", 50, 0, 1.0);
      for (unsigned int i=0; i<nBots; ++i)
      {
        // file->mkdir(("Bot_brain_"+itoa(i)).c_str());
        Brain *brain=bots.at(i)->brain_;
        for (unsigned int j=0; j<brain->neurons_.size(); ++j)
        {
          Neuron *neuron=brain->neurons_.at(j);
          h_spontaneity->Fill(j, neuron->spontaneousRate_/double(nBots));
          double sumDistance=0;
          for (unsigned int k=0; k<neuron->neuralRelations_.size(); ++k)
          {
            sumDistance+=neuron->neuralRelations_.at(k)->distance;
            h_distances_matrix->Fill(j, neuron->neuralRelations_.at(k)->index, (neuron->neuralRelations_.at(k)->distance)/double(nSizeMatrix*nBots));
          }
          h_distances->Fill(sumDistance/double(brain->neurons_.size()), 1./double(nBots));
        }
      }
      
      TFile *file;
      if (generations==100) file=new TFile("AnalyzeThis.root", "recreate");
      else file=new TFile("AnalyzeThis.root", "update");
      g_avgBrainSize_time->Write(g_avgBrainSize_time->GetName(), 5 );
      g_avgBrainSize_generation->Write(g_avgBrainSize_generation->GetName(), 5 );
      g_dtime_generation->Write(g_dtime_generation->GetName(), 5 );
      g_dtime_time->Write(g_dtime_time->GetName(), 5 );
      g_avgBrainSize_predator_time->Write(g_avgBrainSize_predator_time->GetName(), 5 );
      g_avgBrainSize_predator_generation->Write(g_avgBrainSize_predator_generation->GetName(), 5 );
      g_dtime_predator_generation->Write(g_dtime_predator_generation->GetName(), 5 );
      g_dtime_predator_time->Write(g_dtime_predator_time->GetName(), 5 );
      file->mkdir("Brain");
      file->cd("Brain");
      h_spontaneity->Write();
      h_distances->Write();
      h_distances_matrix->Write();
      file->Close();
      
      delete g_avgBrainSize_time;
      delete g_avgBrainSize_generation;
      delete g_dtime_generation;
      delete g_dtime_time;
      delete g_avgBrainSize_predator_time;
      delete g_avgBrainSize_predator_generation;
      delete g_dtime_predator_generation;
      delete g_dtime_predator_time;
      delete h_spontaneity;
      delete h_distances_matrix;
      delete h_distances;
    }
  }
  
  return 0;
}
    
