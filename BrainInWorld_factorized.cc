/* 
==================================================
 BrainInWorld.cc
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
Copyright (C) 11 November 2013 Souvik Das
ALL RIGHTS RESERVED
=================================================
*/

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

#include "interface/Neuron.h"

bool verb=false;
bool EEG=true;
int timeStep=100;
double worldSize=100;

class Entity
{
  public:
  
    TEllipse *circle_;
  
  public:
  
    double x_, y_, theta_;
    
    void turnLeft()
    {
      theta_=checkAngle(theta_+0.1);//  std::cout<<"turning left!"<<std::endl;
    }
    
    void turnRight()
    {
      theta_=checkAngle(theta_-0.1);//  std::cout<<"turning right!"<<std::endl;
    }
    
    void circularBoundaries()
    {
      if (x_<0) x_=99;
      if (x_>worldSize) x_=1;
      if (y_<0) y_=99;
      if (y_>worldSize) y_=1;
    }
    
    void bouncyBoundaries()
    {
      if (x_<0) {x_=2; theta_=pi-theta_;}
      if (x_>worldSize) {x_=98; theta_=pi-theta_;}
      if (y_<0) {y_=2; theta_=-theta_;}
      if (y_>worldSize) {y_=98; theta_=-theta_;}
    }
};


class Fire: public Entity
{
  public:
  
    Fire(double x, double y, double theta)
    {
      x_=x;
      y_=y;
      theta_=theta;
      circle_=new TEllipse(x, y, 5., 5.);
      circle_->SetLineColor(kRed);
      circle_->SetFillColor(kYellow);
    }
    
    void draw()
    {
      circle_->SetX1(x_);
      circle_->SetY1(y_);
      circle_->Draw();
    }
    
    void moveForward()
    {
      x_=x_+0.01*cos(theta_);
      y_=y_+0.01*sin(theta_);
      bouncyBoundaries();
    }
}; 

class Food: public Entity
{
  public:
  
    Food(double x, double y, double theta)
    {
      x_=x;
      y_=y;
      theta_=theta;
      circle_=new TEllipse(x, y, 3., 3.);
      circle_->SetLineColor(kGreen+2);
      circle_->SetFillColor(kGreen);
    }
    
    ~Food()
    {
      circle_->Delete();
    }
    
    void draw()
    {
      circle_->SetX1(x_);
      circle_->SetY1(y_);
      circle_->Draw();
    }
    
    void moveForward()
    {
      x_=x_+0.001*cos(theta_);
      y_=y_+0.001*sin(theta_);
      bouncyBoundaries();
    }
};  

class Bot: public Entity
{
  public:
  
    Brain* brain_;
    TLine *line1_, *line2_;
    
  public:
    
    Bot(double x, double y, double theta, int brainSize)
    {
      x_=x;
      y_=y;
      theta_=theta;
      circle_=new TEllipse(x, y, 2., 2., theta*180./pi+22.5, 360.+theta*180./pi-22.5);
      circle_->SetLineColor(kBlack);
      circle_->SetFillColor(kBlue);
      line1_=new TLine(x, y, x+20.*cos(theta+pi/16.), y+20.*sin(theta+pi/16.));
      line2_=new TLine(x, y, x+20.*cos(theta-pi/16.), y+20.*sin(theta-pi/16.));
      line1_->SetLineColor(kCyan);
      line2_->SetLineColor(kCyan);
      
      brain_=new Brain(brainSize);
    }
    
    Bot(double x, double y, double theta, Brain *brain)
    {
      x_=x;
      y_=y;
      theta_=theta;
      circle_=new TEllipse(x, y, 2., 2., theta*180./pi+22.5, 360.+theta*180./pi-22.5);
      circle_->SetLineColor(kBlack);
      circle_->SetFillColor(kBlue);
      line1_=new TLine(x, y, x+20.*cos(theta+pi/16.), y+20.*sin(theta+pi/16.));
      line2_=new TLine(x, y, x+20.*cos(theta-pi/16.), y+20.*sin(theta-pi/16.));
      line1_->SetLineColor(kCyan);
      line2_->SetLineColor(kCyan);
      
      double rnd=r3->Rndm();
      int diffBrainSize=0;
      if (rnd<0.25) diffBrainSize=-1;
      else if (rnd>0.75) diffBrainSize=1; // && brain->neurons.size()<55
      int newBrainSize=brain->neurons.size()+diffBrainSize;
      brain_=new Brain(newBrainSize);
      if (diffBrainSize==0)
      {
        for (unsigned int i=0; i<brain_->neurons.size(); ++i)
        {
          Neuron *neuron_=brain_->neurons.at(i);
          Neuron *neuron=brain->neurons.at(i);
          neuron_->neuralRelations_.clear();
          NeuralRelations neuralRelations=neuron->neuralRelations_;
          for (unsigned int j=0; j<neuralRelations.size(); ++j)
          {
            NeuralRelation *neuralRelation_=new NeuralRelation;
            neuralRelation_->index=neuralRelations.at(j)->index;
            neuralRelation_->synapticStrength=r3->Rndm();
            double dist=(neuralRelations.at(j)->distance)-0.05+0.1*r3->Rndm();
            if (dist<0) dist=0;
            else if (dist>1) dist=1;
            neuralRelation_->distance=dist;
            neuron_->push_back_relation(neuralRelation_);
          }
        }
      }
      else if (diffBrainSize<0)
      {
        for (unsigned int i=0; i<brain_->neurons.size(); ++i)
        {
          Neuron *neuron_=brain_->neurons.at(i);
          Neuron *neuron=brain->neurons.at(i);
          neuron_->neuralRelations_.clear();
          NeuralRelations neuralRelations=neuron->neuralRelations_;
          for (unsigned int j=0; j<neuralRelations.size(); ++j)
          {
            if (neuralRelations.at(j)->index<brain_->neurons.size())
            {
              NeuralRelation *neuralRelation_=new NeuralRelation;
              neuralRelation_->index=neuralRelations.at(j)->index;
              neuralRelation_->synapticStrength=r3->Rndm();
              double dist=(neuralRelations.at(j)->distance)-0.05+0.1*r3->Rndm();
              if (dist<0) dist=0;
              else if (dist>1) dist=1;
              neuralRelation_->distance=dist;
              neuron_->push_back_relation(neuralRelation_);
            }
          }
        }
      }
      else if (diffBrainSize>0)
      {
        for (unsigned int i=0; i<brain_->neurons.size()-1; ++i)
        {
          Neuron *neuron_=brain_->neurons.at(i);
          Neuron *neuron=brain->neurons.at(i);
          neuron_->neuralRelations_.clear();
          NeuralRelations neuralRelations=neuron->neuralRelations_;
          for (unsigned int j=0; j<neuralRelations.size(); ++j)
          {
            NeuralRelation *neuralRelation_=new NeuralRelation;
            neuralRelation_->index=neuralRelations.at(j)->index;
            neuralRelation_->synapticStrength=r3->Rndm();
            double dist=(neuralRelations.at(j)->distance)-0.05+0.1*r3->Rndm();
            if (dist<0) dist=0;
            else if (dist>1) dist=1;
            neuralRelation_->distance=dist;
            neuron_->push_back_relation(neuralRelation_);
          }
          if (r3->Rndm()>0.5) // Add one more neural relation to each neuron
          {
            int newIndex=int(r3->Rndm()*brain_->neurons.size());
            if (i!=newIndex)
            {
              NeuralRelation *neuralRelation_=new NeuralRelation;
              neuralRelation_->index=newIndex;
              neuralRelation_->synapticStrength=r3->Rndm();
              neuralRelation_->distance=r3->Rndm();
              neuron_->push_back_relation(neuralRelation_);
            }
          }
        }
      }
      // Update the distance histograms
      if (EEG)
      {
        for (unsigned int i=0; i<brain_->neurons.size(); ++i)
        {
          Neuron *neuron_=brain_->neurons.at(i);
          NeuralRelations *neuralRelations_=&(neuron_->neuralRelations_);
          for (unsigned int j=0; j<neuralRelations_->size(); ++j)
          {
            NeuralRelation *neuralRelation_=neuralRelations_->at(j);
            brain_->h_distances_->SetBinContent(i, neuralRelation_->index, neuralRelation_->distance);
          }
        }
      }
    } 
    
    ~Bot()
    {
      circle_->Delete();
      line1_->Delete();
      line2_->Delete();
      delete brain_;
    }
    
    void draw()
    {
      circle_->SetX1(x_);
      circle_->SetY1(y_);
      circle_->SetPhimin(theta_*180./pi+22.5);
      circle_->SetPhimax(360.+theta_*180./pi-22.5);
      line1_->SetX1(x_);
      line1_->SetY1(y_);
      line1_->SetX2(x_+20.*cos(theta_+pi/16.));
      line1_->SetY2(y_+20.*sin(theta_+pi/16.));
      line2_->SetX1(x_);
      line2_->SetY1(y_);
      line2_->SetX2(x_+20.*cos(theta_-pi/16.));
      line2_->SetY2(y_+20.*sin(theta_-pi/16.));
      
      circle_->Draw();
      line1_->Draw();
      line2_->Draw();
    }
    
    void moveForward()
    {
      x_=x_+0.2*cos(theta_);
      y_=y_+0.2*sin(theta_);
      bouncyBoundaries();
    }
    
    bool inSight(double x, double y)
    {
      double thetac=convertToZeroToPi(atan2((y-y_), (x-x_)));
      if (inBetween(thetac, theta_+pi/16., theta_-pi/16.)) return true;
      else return false;
    } 
    
    void see(std::vector<Food*> *foods) //,std::vector<Fire*> *fires) //, double &distance)
    {
      for (unsigned int i=0; i<foods->size(); ++i)
      {
        if (inSight(foods->at(i)->x_, foods->at(i)->y_)) 
        {
          double dist=pow(pow(x_-foods->at(i)->x_, 2)+pow(y_-foods->at(i)->y_, 2), 0.5);
          brain_->neurons.at(0)->receive(1);
          if (dist<30.) brain_->neurons.at(1)->receive(1);
          else if (dist<60.) brain_->neurons.at(2)->receive(1);
          else brain_->neurons.at(3)->receive(1);
          
          break;
        }
      }
      /*for (unsigned int i=0; i<fires->size(); ++i)
      {
        if (inSight(fires->at(i)->x_, fires->at(i)->y_)) 
        {
          
          double dist=pow(pow(x_-foods->at(i)->x_, 2)+pow(y_-foods->at(i)->y_, 2), 0.5);
          // brain_->neurons.at(1)->receive(1);
          if (dist<30.) brain_->neurons.at(1)->receive(1);
          //else if (dist<60.) brain_->neurons.at(2)->receive(1);
          //else if (dist>60.) brain_->neurons.at(3)->receive(1);
          
          break;
        }
      }*/
    }
    
    void stepInTime()
    {
      brain_->stepInTime();
      if (brain_->neurons.at(5)->potential()>0.8) moveForward();
      if (brain_->neurons.at(6)->potential()>0.8) turnLeft();
      if (brain_->neurons.at(7)->potential()>0.8) turnRight();
    }
    
    void printBrain()
    {
      brain_->print();
    } 
};

void BrainInWorld_factorized()
{ 
  r3->SetSeed(120);
  
  typedef std::vector<Bot*> Bots;
  Bots bots;
  unsigned int nBots=10;
  for (unsigned int i=0; i<nBots; ++i)
  {
    Bot *bot=new Bot(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi, 30);
    bots.push_back(bot);
  }
  std::cout<<"Made bots"<<std::endl;
  
  typedef std::vector<Fire*> Fires;
  Fires fires;
  unsigned int nFires=0;
  for (unsigned int i=0; i<nFires; ++i)
  {
    Fire *fire=new Fire(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi);
    fires.push_back(fire);
  }
  
  typedef std::vector<Food*> Foods;
  Foods foods;
  unsigned int nFoods=100;
  for (unsigned int i=0; i<nFoods; ++i)
  {
    Food *food=new Food(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi);
    foods.push_back(food);
  }
  std::cout<<"Made food"<<std::endl;
  
  std::vector <double> avgBrainSize_vector;
  std::vector <double> time_vector;
  
  gStyle->SetPalette(1);
  
  TCanvas *c_World=new TCanvas("c_World", "Natural Neural Network in Genetic Algorithm", 700, 700);
  c_World->Range(0,0,worldSize,worldSize);
  TCanvas *c_Potential_Histograms;
  TCanvas *c_SynapticStrength_Histograms;
  TCanvas *c_Distance_Histograms;
  if (EEG)
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
  while (foods.size()>0 && bots.size()>0 && generations<50000)
  {
    ++time;
    double avgBrainSize=0;
    for (unsigned int i=0; i<bots.size(); ++i)
    {
      bots.at(i)->see(&foods); //, &fires);
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
    for (unsigned int i=0; i<bots.size(); ++i)
    {
      int eatenFood=-1;
      for (unsigned int j=0; j<foods.size(); ++j)
      {
        double d2=pow(bots.at(i)->x_-foods.at(j)->x_, 2)+pow(bots.at(i)->y_-foods.at(j)->y_, 2);
        if (d2<13)
        {
          eatenFood=j;
          if (verb) std::cout<<"Bot "<<i<<" ate food "<<j<<std::endl;
          break;
        }
      }
      if (eatenFood>=0) 
      {
        ++nEaten;
        if (verb) std::cout<<"foods.size() = "<<foods.size()<<" and eatenFood = "<<eatenFood<<std::endl;
        delete *(foods.begin()+eatenFood);
        foods.erase(foods.begin()+eatenFood);
        if (r3->Rndm()>0.1)
        {
          Food *food=new Food(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*pi);
          foods.push_back(food);
        }
        Bot *bot=new Bot(bots.at(i)->x_, bots.at(i)->y_, bots.at(i)->theta_, bots.at(i)->brain_);
        bots.push_back(bot);
        ++generations;
        avgBrainSize_vector.push_back(bot->brain_->neurons.size());
        time_vector.push_back(time);
        if (verb) std::cout<<"removed from vector, foods.size() = "<<foods.size()<<std::endl;
      }
    }
    for (unsigned int i=0; i<nEaten; ++i)
    {
      delete *(bots.begin());
      bots.erase(bots.begin());
    }
    
    // check for fires eating bots
    for (unsigned int i=0; i<fires.size(); ++i)
    {
      int eatenBot=-1;
      for (unsigned int j=0; j<bots.size(); ++j)
      {
        double d2=pow(fires.at(i)->x_-bots.at(j)->x_, 2)+pow(fires.at(i)->y_-bots.at(j)->y_, 2);
        if (d2<29)
        {
          eatenBot=j;
          break;
        }
      }
      if (eatenBot>=0) 
      {
        delete *(bots.begin()+eatenBot);
        bots.erase(bots.begin()+eatenBot);
        Bot *bot=new Bot(r3->Rndm()*worldSize, r3->Rndm()*worldSize, r3->Rndm()*2.*pi, 30);
        bots.push_back(bot);
      }
    }
    
    if (time%timeStep==0)
    {
      // std::cout<<"Time = "<<time<<", amount of food left = "<<foods.size()<<std::endl;
      c_World->cd();
      for (unsigned int i=0; i<bots.size(); ++i) bots.at(i)->draw();
      for (unsigned int i=0; i<fires.size(); ++i) fires.at(i)->draw();
      for (unsigned int i=0; i<foods.size(); ++i) foods.at(i)->draw();
      c_World->Update();
    }
    
    if (EEG && time%timeStep==0)
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
  }
  
  TGraph *g_avgBrainSize=new TGraph(avgBrainSize_vector.size(), &time_vector[0], &avgBrainSize_vector[0]);
  g_avgBrainSize->SetTitle("Average size of brains against time; time steps; Average size of brains");
  TCanvas *c_avgBrainSize=new TCanvas("avgBrainSize", "Average Brain Size", 700, 700);
  g_avgBrainSize->Draw("AL*");
  c_avgBrainSize->SaveAs("c_avgBrainSize.png");
  
  TFile *file=new TFile("AnalyzeThis.root", "recreate");
  g_avgBrainSize->Write();
  file->Close();
  
}
    
