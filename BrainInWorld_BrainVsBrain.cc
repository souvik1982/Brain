/* 
==================================================
 BrainInWorld.cc
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
 Two species, predator and prey are made to compete
 
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

#include <TCanvas.h>
#include <TEllipse.h>
#include <TLine.h>
#include <TRandom3.h>
#include <TTimer.h>

#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>

bool verb=false;
bool EEG=false;

double pi=3.14159265358979;

TRandom3 *r3=new TRandom3();

double convertToZeroToPi(double angle)
{
  if (angle<0) return 2.*pi+angle;
  else return angle;
}

double checkAngle(double theta)
{
  if (theta<0) theta=2.*pi+theta;
  if (theta>2.*pi) theta=theta-2.*pi;
  return theta;
}

double pythDistance(double x1, double y1, double x2, double y2)
{
  return pow(pow(x1-x2, 2)+pow(y1-y2, 2), 0.5);
}

bool inBetween(double a, double x1, double x2)
{
  if (x1>a && a>x2) return true;
  else return false;
}

std::string itoa(int i) 
{
  char res[10];
  sprintf(res, "%d", i);
  std::string ret(res);
  return ret;
}

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
    double potential_;
    double potential_buffer_;
    NeuralRelations neuralRelations_;
    
  public:
    Neuron()
    {
      potential_=0;
      potential_buffer_=0;
    }
    
    void push_back_relation(NeuralRelation *relation)
    {
      neuralRelations_.push_back(relation);
    }
    
    void receive(double charge)
    {
      potential_buffer_+=charge;
      if (potential_buffer_>2.) potential_buffer_=2.;
    }
    
    void stepInTime1(Neurons *neurons);
    
    void stepInTime2()
    {
      potential_+=potential_buffer_;
      potential_buffer_=0;
    }
    
    double potential()
    {
      return potential_;
    }
    
    void print()
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
    
    void printSimple()
    {
      std::cout<<potential_<<", ";
    }
};

void Neuron::stepInTime1(Neurons *neurons)
{
  if (potential_>=0.8 || r3->Rndm()>0.95) // fire
  {
    double totalSynapticWeight=0;
    for (unsigned int i=0; i<neuralRelations_.size(); ++i) totalSynapticWeight+=neuralRelations_.at(i)->synapticStrength;
    for (unsigned int i=0; i<neuralRelations_.size(); ++i)
    {
      Neuron *targetNeuron=neurons->at(neuralRelations_.at(i)->index);
      double synapticStrength=neuralRelations_.at(i)->synapticStrength;
      targetNeuron->receive((synapticStrength/totalSynapticWeight)*(neuralRelations_.at(i)->distance));
      neuralRelations_.at(i)->synapticStrength=synapticStrength+0.8*(1.-synapticStrength);
    }
    potential_=0;
  }
  else
  {
    for (unsigned int i=0; i<neuralRelations_.size(); ++i)
    {
      (neuralRelations_.at(i)->synapticStrength)*=0.8;
      if (neuralRelations_.at(i)->synapticStrength<1e-6) (neuralRelations_.at(i)->synapticStrength)=1e-6;
    }
  }
}

class Brain
{
  public:
  
    Neurons neurons;
    TH1F *h_potentials_;
    TH2F *h_synapticStrengths_, *h_distances_;
    TH1F *h_connectivities_;
    // std::vector<double> x_neuron_, y_neuron_;
    
    Brain(int size)
    {
      int rand=r3->Rndm()*1000000;
      h_potentials_=new TH1F(("h_potentials_"+itoa(rand)).c_str(), "Neural Action Potentials", size, 0, size);
      h_synapticStrengths_=new TH2F(("h_synapticStrengths_"+itoa(rand)).c_str(), "Neural Synaptic Strengths", size, size, 0, size, 0, size);
      h_distances_=new TH2F(("h_distances_"+itoa(rand)).c_str(), "Neural Distances", size, size, 0, size, 0, size);
      h_connectivities_=new TH1F(("h_connectivities_"+itoa(rand)).c_str(), "Neural Connectivities", size, 0, size);
      // double theta=0;
      for (int i=0; i<size; ++i)
      {
        Neuron *neuron=new Neuron;
        neurons.push_back(neuron);
        double connectivity=0;
        for (int j=0; j<size; ++j)
        {
          if (i!=j && r3->Rndm()>0.5)
          {
            NeuralRelation *n=new NeuralRelation;
            n->index=j;
            n->synapticStrength=r3->Rndm(); // Should be read from file
            n->distance=r3->Rndm(); // Should be read from file
            neurons.at(i)->push_back_relation(n);
            
            h_synapticStrengths_->Fill(i, j, n->synapticStrength);
            h_distances_->Fill(i, j, n->distance);
            
            connectivity+=n->distance;
          }
          h_connectivities_->Fill(connectivity);
        }
        h_potentials_->Fill(i+1);
        
        // Arrange the neurons initially on a circle of radius 100
        // x_neuron_=100.+100.*cos(theta);
        // y_neuron_=100.+100.*sin(theta);
        // theta+=2.*pi/double(size);
      }
    }
    
    ~Brain()
    {
      for (unsigned int i=0; i<neurons.size(); ++i)
      {
        delete neurons.at(i);
      }
      delete h_potentials_;
      delete h_synapticStrengths_;
      delete h_distances_;
      delete h_connectivities_;
    }
    
    void stepInTime()
    {
      for (unsigned int i=0; i<neurons.size(); ++i)
      {
        neurons.at(i)->stepInTime1(&neurons);
      }
      for (unsigned int i=0; i<neurons.size(); ++i)
      {
        neurons.at(i)->stepInTime2();
        if (EEG)
        {
          h_potentials_->SetBinContent(i+1, neurons.at(i)->potential_);
          NeuralRelations *neuralRelations=&(neurons.at(i)->neuralRelations_);
          for (unsigned int j=0; j<neuralRelations->size(); ++j)
          {
            h_synapticStrengths_->SetBinContent(i, neuralRelations->at(j)->index, neuralRelations->at(j)->synapticStrength);
          }
        }
      }
    }
    
    void print()
    {
      for (unsigned int i=0; i<neurons.size(); ++i)
      {
        neurons.at(i)->printSimple();
        // std::cout<<" === Neuron # "<<i<<" === "<<std::endl;
        // neurons.at(i)->print();
      }
      std::cout<<std::endl;
    }
    
    void drawPotentials()
    {
      h_potentials_->Draw("CL");
    }
    void drawSynapticStrengths()
    {
      h_synapticStrengths_->Draw("colz");
    }
    void drawDistances()
    {
      h_distances_->Draw("colz");
    }
    void drawConnectivities()
    {
      h_connectivities_->Draw();
    }
    
    void drawBrainMap()
    {
    }
      
    
};

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
      if (x_>100) x_=1;
      if (y_<0) y_=99;
      if (y_>100) y_=1;
    }
    
    void bouncyBoundaries()
    {
      if (x_<0) {x_=2; theta_=pi-theta_;}
      if (x_>100) {x_=98; theta_=pi-theta_;}
      if (y_<0) {y_=2; theta_=-theta_;}
      if (y_>100) {y_=98; theta_=-theta_;}
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
    
    // Quantities to characterize performance
    double time1_, x1_, y1_;
  
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
      
      time1_=0;
      x1_=x;
      y1_=y;
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
      else if (rnd>0.75) diffBrainSize=1;
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
      // Update the distance & connectivity histograms
      if (EEG)
      {
        brain_->h_connectivities_->Reset();
        for (unsigned int i=0; i<brain_->neurons.size(); ++i)
        {
          Neuron *neuron_=brain_->neurons.at(i);
          NeuralRelations *neuralRelations_=&(neuron_->neuralRelations_);
          double connectivity=0;
          for (unsigned int j=0; j<neuralRelations_->size(); ++j)
          {
            NeuralRelation *neuralRelation_=neuralRelations_->at(j);
            brain_->h_distances_->SetBinContent(i, neuralRelation_->index, neuralRelation_->distance);
            connectivity+=neuralRelation_->distance;
          }
          brain_->h_connectivities_->Fill(connectivity);
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
    
    void see(std::vector<Entity*> *entities) //,std::vector<Fire*> *fires) //, double &distance)
    {
      for (unsigned int i=0; i<entities->size(); ++i)
      {
        if (inSight(entities->at(i)->x_, entities->at(i)->y_)) 
        {
          // moveForward();
          
          double dist=pow(pow(x_-entities->at(i)->x_, 2)+pow(y_-entities->at(i)->y_, 2), 0.5);
          brain_->neurons.at(0)->receive(1);
          if (dist<30.) brain_->neurons.at(1)->receive(1);
          else if (dist<60.) brain_->neurons.at(2)->receive(1);
          else if (dist>60.) brain_->neurons.at(3)->receive(1);
          
          break;
        }
      }
    }
    
    void stepInTime()
    {
      brain_->stepInTime();
      if (brain_->neurons.at(5)->potential()>0.8) moveForward();
      if (brain_->neurons.at(6)->potential()>0.8) turnLeft();
      if (brain_->neurons.at(7)->potential()>0.8) turnRight();
      
      ++time1_;
    }
    
    void printBrain()
    {
      brain_->print();
    }
    
    double eaten()
    {
      double dist=pythDistance(x_, y_, x1_, y1_);
      double speed=dist/time1_;
      x1_=x_;
      y1_=y_;
      time1_=0;
      return speed;
    }
};


void BrainInWorld_BrainVsBrain()
{ 
  r3->SetSeed(120);
  
  typedef std::vector<Bot*> Bots;
  Bots preys;
  unsigned int nPrey=10;
  for (unsigned int i=0; i<nPrey; ++i)
  {
    Bot *bot=new Bot(r3->Rndm()*100., r3->Rndm()*100., r3->Rndm()*2.*pi, 30);
    preys.push_back(bot);
  }
  
  Bots predators;
  unsigned int nPredators=10;
  for (unsigned int i=0; i<nPredators; ++i)
  {
    Bot *bot=new Bot(r3->Rndm()*100., r3->Rndm()*100., r3->Rndm()*2.*pi, 30);
    predators.push_back(bot);
  }
  
  std::vector <double> avgBrainSize_vector;
  std::vector <double> time_vector;
  
  gStyle->SetPalette(1);
  
  TCanvas *c_World=new TCanvas("c_World", "Natural Neural Network in Genetic Algorithm", 700, 700);
  c_World->Range(0,0,100,100);
  TCanvas *c_Potential_Histograms;
  TCanvas *c_SynapticStrength_Histograms;
  TCanvas *c_Distance_Histograms;
  TCanvas *c_Connectivity_Histograms;
  if (EEG)
  {
    c_Potential_Histograms=new TCanvas("c_Potential_Histograms", "Brain Data - Neural Potentials", 700, 700);
    c_SynapticStrength_Histograms=new TCanvas("c_SynapticStrength_Histograms", "Brain Data - Synaptic Strengths", 700, 700);
    c_Distance_Histograms=new TCanvas("c_Distance_Histograms", "Brain Data - Neural Distances", 700, 700);
    c_Connectivity_Histograms=new TCanvas("c_Connectivity_Histograms", "Brain Data - Neural Proximity Connectivities", 700, 700);
    c_Potential_Histograms->Divide(ceil(bots.size()/3.), 3);
    c_SynapticStrength_Histograms->Divide(ceil(bots.size()/3.), 3);
    c_Distance_Histograms->Divide(ceil(bots.size()/3.), 3);
    c_Connectivity_Histograms->Divide(ceil(bots.size()/3.), 3);
  }
  int time=0;
  while (preys.size()>0 && predators.size()>0)
  {
    ++time;
    // std::cout<<"=== Time ==="<<time<<std::endl;
    // std::cout<<" Number of bots = "<<bots.size()<<std::endl;
    double avgBrainSize=0;
    for (unsigned int i=0; i<preys.size(); ++i)
    {
      preys.at(i)->see(&predators);
      preys.at(i)->stepInTime();
      // avgBrainSize=avgBrainSize+bots.at(i)->brain_->neurons.size();
    }
    // avgBrainSize_vector.push_back(avgBrainSize/bots.size());
    // time_vector.push_back(time);
    for (unsigned int i=0; i<predators.size(); ++i)
    {
      predators.at(i)->see(&preys);
      predators.at(i)->stepInTime();
    }
    
    // check for predators eating prey
    int nEatenPrey=0;
    for (unsigned int i=0; i<predators.size(); ++i)
    {
      int eatenPrey=-1;
      for (unsigned int j=0; j<preys.size(); ++j)
      {
        double d2=pow(preys.at(i)->x_-predators.at(j)->x_, 2)+pow(preys.at(i)->y_-predators.at(j)->y_, 2);
        if (d2<16)
        {
          eatenPrey=j;
          break;
        }
      }
      if (eatenPrey>=0)
      {
        ++nEatenPrey;
        delete *(preys.begin()+eatenPrey);
        preys.erase(preys.begin()+eatenPrey);
        Bot *predator=new Bot(predators.at(i)->x_, predators.at(i)->y_, predators.at(i)->theta_, predators.at(i)->brain_);
        predators.push_back(predator);
      }
    }
    // Oldest predators die
    for (unsigned int i=0; i<nEaten; ++i)
    {
      delete *(predators.begin());
      predators.erase(predators.begin());
    }
    // Newest preys reproduce
    for (unsigned int i=0; i<nEaten; ++i)
    {
      Bot *prey=new Bot(preys.at(i)->x_, preys.at(i)->y_, preys.at(i)->theta_, preys.at(i)->brain_);
      prey
      
      
      
      
    
    if (time%100==0)
    {
      c_World->cd();
      for (unsigned int i=0; i<bots.size(); ++i) bots.at(i)->draw();
      for (unsigned int i=0; i<fires.size(); ++i) fires.at(i)->draw();
      for (unsigned int i=0; i<foods.size(); ++i) foods.at(i)->draw();
      c_World->Update();
      // c_World->SaveAs("c_World.png");
    }
    
    if (EEG && time%100==0)
    {
      for (unsigned int i=0; i<bots.size(); ++i)
      {
        c_Potential_Histograms->cd(i+1);
        bots.at(i)->brain_->drawPotentials();
        c_Distance_Histograms->cd(i+1);
        bots.at(i)->brain_->drawDistances();
        c_SynapticStrength_Histograms->cd(i+1);
        bots.at(i)->brain_->drawSynapticStrengths();
        c_Connectivity_Histograms->cd(i+1);
        bots.at(i)->brain_->drawConnectivities();
      }
      c_Potential_Histograms->Modified();
      c_Potential_Histograms->Update();
      c_SynapticStrength_Histograms->Modified();
      c_SynapticStrength_Histograms->Update();
      c_Distance_Histograms->Modified();
      c_Distance_Histograms->Update();
      c_Connectivity_Histograms->Modified();
      c_Connectivity_Histograms->Update();
      
      // c_Potential_Histograms->SaveAs("c_Potential_Histograms.png");
      // c_SynapticStrength_Histograms->SaveAs("c_SynapticStrength_Histograms.png");
      // c_Distance_Histograms->SaveAs("c_Distance_Histograms.png");
    }
    
    
    // std::cout<<"=== ==="<<std::endl;
  }
  TGraph *g_avgBrainSize=new TGraph(avgBrainSize_vector.size(), &time_vector[0], &avgBrainSize_vector[0]);
  g_avgBrainSize->SetTitle("Average size of brains against time; time steps; Average size of brains");
  TCanvas *c_avgBrainSize=new TCanvas("avgBrainSize", "Average Brain Size", 700, 700);
  g_avgBrainSize->Draw("AL*");
  
  /*TGraph *g_speed=new TGraph(speed_vector.size(), &speed_time_vector[0], &speed_vector[0]);
  g_speed->SetTitle("Eating speed against time; time steps; Eating speed");
  TCanvas *c_speed=new TCanvas("c_speed", "c_speed", 700, 700);
  g_speed->Draw("AL*");*/
  
}
    
