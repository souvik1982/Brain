/* 
==================================================
 Bot
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
Copyright (C) 11 November 2013 Souvik Das
ALL RIGHTS RESERVED
=================================================
*/

#include "../interface/Bot.h"
#include "../interface/ToolBox.h"

#include <iostream>

Bot::Bot(double x, double y, double theta, int brainSize, std::string name, double worldSize, int debug): Entity(worldSize)
{
  debug_=debug;
  worldSize_=worldSize;
  name_=name;
  kids_=0;
  x_=x;
  y_=y;
  theta_=theta;
  brain_=new Brain(brainSize, debug_, name_);
  if (decodeDebug(debug_, 0)==1)
  {
    circle_=new TEllipse(x, y, 2., 2., theta*180./pi+22.5, 360.+theta*180./pi-22.5);
    circle_->SetLineColor(kBlack);
    circle_->SetFillColor(kBlue);
    line1_=new TLine(x, y, x+20.*cos(theta+pi/16.), y+20.*sin(theta+pi/16.));
    line2_=new TLine(x, y, x+20.*cos(theta-pi/16.), y+20.*sin(theta-pi/16.));
    line1_->SetLineColor(kCyan);
    line2_->SetLineColor(kCyan);
  }
}

Bot::Bot(double x, double y, double theta, Bot *parentBot, double mu_newNeuron, double mu_newConnection, double mu_modConnection): Entity(parentBot->worldSize_)
{
  ++(parentBot->kids_);
  debug_=parentBot->debug_;
  worldSize_=parentBot->worldSize_;
  name_=parentBot->name_+"_"+itoa(parentBot->kids_);
  kids_=0;
  x_=x;
  y_=y;
  theta_=theta;
  if (decodeDebug(debug_, 0)==1)
  {
    circle_=new TEllipse(x, y, 2., 2., theta*180./pi+22.5, 360.+theta*180./pi-22.5);
    circle_->SetLineColor(kBlack);
    circle_->SetFillColor(kBlue);
    line1_=new TLine(x, y, x+20.*cos(theta+pi/16.), y+20.*sin(theta+pi/16.));
    line2_=new TLine(x, y, x+20.*cos(theta-pi/16.), y+20.*sin(theta-pi/16.));
    line1_->SetLineColor(kCyan);
    line2_->SetLineColor(kCyan);
  }
  
  double rnd=r3->Rndm();
  int diffBrainSize=0;
  if (rnd<mu_newNeuron/2.) diffBrainSize=-1;
  else if (rnd>1.-mu_newNeuron/2.) diffBrainSize=1;
  brain_=new Brain(parentBot->brain_, diffBrainSize, debug_, name_, mu_newConnection, mu_modConnection);
}

Bot::~Bot()
{
  if (decodeDebug(debug_, 0)==1)
  {
    circle_->Delete();
    line1_->Delete();
    line2_->Delete();
  }
  delete brain_;
}

void Bot::draw()
{
  if (decodeDebug(debug_, 0)==1)
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
}

void Bot::moveForward()
{
  x_=x_+1.*cos(theta_);
  y_=y_+1.*sin(theta_);
  bouncyBoundaries();
}

bool Bot::inSight(double x, double y)
{
  double thetac=convertToZeroToPi(atan2((y-y_), (x-x_)));
  if (inBetween(thetac, theta_+pi/16., theta_-pi/16.)) return true;
  else return false;
}

void Bot::seeFood(std::vector<Food*> *foods) //,std::vector<Fire*> *fires) //, double &distance)
{
  for (unsigned int i=0; i<foods->size(); ++i)
  {
    if (inSight(foods->at(i)->x_, foods->at(i)->y_)) 
    {
      double dist=pow(pow(x_-foods->at(i)->x_, 2)+pow(y_-foods->at(i)->y_, 2), 0.5);
      brain_->neurons_.at(0)->receive(1);
      if (dist<30.) brain_->neurons_.at(1)->receive(1);
      else if (dist<60.) brain_->neurons_.at(2)->receive(1);
      else brain_->neurons_.at(3)->receive(1);
    }
  }
}

void Bot::stepInTime()
{
  brain_->stepInTime();
  if (brain_->neurons_.at(5)->potential()>0.8) moveForward();
  if (brain_->neurons_.at(6)->potential()>0.8) turnLeft();
  if (brain_->neurons_.at(7)->potential()>0.8) turnRight();
}

void Bot::printBrain()
{
  brain_->print();
} 
