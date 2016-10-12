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

Bot::Bot(std::string type, double x, double y, double theta, double visualAngle, int brainSize, int bodyColor, double speed, std::string name, double worldSize, int debug): Entity(worldSize)
{
  debug_=debug;
  worldSize_=worldSize;
  type_=type;
  name_=name;
  bodyColor_=bodyColor;
  speed_=speed;
  kids_=0;
  x_=x;
  y_=y;
  theta_=theta;
  visualAngle_=visualAngle;
  brain_=new Brain(brainSize, debug_, name_);
  if (decodeDebug(debug_, 0)==1)
  {
    circle_=new TEllipse(x, y, 2.);
    circle_->SetLineColor(kBlack);
    circle_->SetFillColor(bodyColor_);
    visRange1_=new TEllipse(x_, y_, 30, 30, 360.+(theta_-visualAngle_/2.)*180./pi, 360.+(theta_+visualAngle_/2.)*180./pi);
    visRange2_=new TEllipse(x_, y_, 60, 60, 360.+(theta_-visualAngle_/2.)*180./pi, 360.+(theta_+visualAngle_/2.)*180./pi);
    visRange3_=new TEllipse(x_, y_, 100, 100, 360.+(theta_-visualAngle_/2.)*180./pi, 360.+(theta_+visualAngle_/2.)*180./pi);
    visPeriphery1_=new TLine(x_, y_, x_+100.*cos(theta_+visualAngle_/6.), y_+100.*sin(theta_+visualAngle_/6.));
    visPeriphery2_=new TLine(x_, y_, x_+100.*cos(theta_-visualAngle_/6.), y_+100.*sin(theta_-visualAngle_/6.));
    visRange1_->SetFillStyle(0);
    visRange2_->SetFillStyle(0);
    visRange3_->SetFillStyle(0);
    visRange1_->SetLineColor(bodyColor_);
    visRange2_->SetLineColor(bodyColor_);
    visRange3_->SetLineColor(bodyColor_);
    visPeriphery1_->SetLineColor(bodyColor_);
    visPeriphery2_->SetLineColor(bodyColor_);
  }
}

Bot::Bot(Bot *parentBot, double mu_newNeuron, double mu_newConnection, double mu_modConnection, double mu_visualAngle): Entity(parentBot->worldSize_)
{
  ++(parentBot->kids_);
  debug_=parentBot->debug_;
  worldSize_=parentBot->worldSize_;
  type_=parentBot->type_;
  name_=parentBot->name_+"_"+itoa(parentBot->kids_);
  bodyColor_=parentBot->bodyColor_;
  speed_=parentBot->speed_;
  kids_=0;
  x_=parentBot->x_;
  y_=parentBot->y_;
  theta_=parentBot->theta_;
  // std::cout<<"old visual angle = "<<visualAngle_<<std::endl;
  visualAngle_=parentBot->visualAngle_+mu_visualAngle*(-0.5+r3->Uniform());
  // std::cout<<" new visual angle = "<<visualAngle_<<std::endl;
  if (visualAngle_<0) visualAngle_=0;
  if (visualAngle_>pi) visualAngle_=pi;
  if (decodeDebug(debug_, 0)==1)
  {
    circle_=new TEllipse(x_, y_, 2.);
    circle_->SetLineColor(kBlack);
    circle_->SetFillColor(bodyColor_);
    visRange1_=new TEllipse(x_, y_, 30, 30, 360.+(theta_-visualAngle_/2.)*180./pi, 360.+(theta_+visualAngle_/2.)*180./pi);
    visRange2_=new TEllipse(x_, y_, 60, 60, 360.+(theta_-visualAngle_/2.)*180./pi, 360.+(theta_+visualAngle_/2.)*180./pi);
    visRange3_=new TEllipse(x_, y_, 100, 100, 360.+(theta_-visualAngle_/2.)*180./pi, 360.+(theta_+visualAngle_/2.)*180./pi);
    visPeriphery1_=new TLine(x_, y_, x_+100.*cos(theta_+visualAngle_/6.), y_+100.*sin(theta_+visualAngle_/6.));
    visPeriphery2_=new TLine(x_, y_, x_+100.*cos(theta_-visualAngle_/6.), y_+100.*sin(theta_-visualAngle_/6.));
    visRange1_->SetFillStyle(0);
    visRange2_->SetFillStyle(0);
    visRange3_->SetFillStyle(0);
    visRange1_->SetLineColor(bodyColor_);
    visRange2_->SetLineColor(bodyColor_);
    visRange3_->SetLineColor(bodyColor_);
    visPeriphery1_->SetLineColor(bodyColor_);
    visPeriphery2_->SetLineColor(bodyColor_);
  }
  
  double rnd=r3->Rndm();
  int diffBrainSize=0;
  if (rnd<mu_newNeuron/2. && parentBot->brain_->neurons_.size()>20) diffBrainSize=-1;
  else if (rnd>1.-mu_newNeuron/2.) diffBrainSize=1;
  brain_=new Brain(parentBot->brain_, diffBrainSize, debug_, name_, mu_newConnection, mu_modConnection);
}

Bot::~Bot()
{
  if (decodeDebug(debug_, 0)==1)
  {
    circle_->Delete();
    visRange1_->Delete();
    visRange2_->Delete();
    visRange3_->Delete();
    visPeriphery1_->Delete();
    visPeriphery2_->Delete();
  }
  delete brain_;
}

void Bot::draw()
{
  if (decodeDebug(debug_, 0)==1)
  {
    circle_->SetX1(x_);
    circle_->SetY1(y_);
    double angle1=360.+(theta_-visualAngle_/2.)*180./pi;
    double angle2=360.+(theta_+visualAngle_/2.)*180./pi;
    visRange1_->SetX1(x_);
    visRange1_->SetY1(y_);
    visRange1_->SetPhimin(angle1);
    visRange1_->SetPhimax(angle2);
    visRange2_->SetX1(x_);
    visRange2_->SetY1(y_);
    visRange2_->SetPhimin(angle1);
    visRange2_->SetPhimax(angle2);
    visRange3_->SetX1(x_);
    visRange3_->SetY1(y_);
    visRange3_->SetPhimin(angle1);
    visRange3_->SetPhimax(angle2);
    visPeriphery1_->SetX1(x_);
    visPeriphery1_->SetY1(y_);
    visPeriphery1_->SetX2(x_+100.*cos(theta_+visualAngle_/6.));
    visPeriphery1_->SetY2(y_+100.*sin(theta_+visualAngle_/6.));
    visPeriphery2_->SetX1(x_);
    visPeriphery2_->SetY1(y_);
    visPeriphery2_->SetX2(x_+100.*cos(theta_-visualAngle_/6.));
    visPeriphery2_->SetY2(y_+100.*sin(theta_-visualAngle_/6.));
    
    circle_->Draw();
    visRange1_->Draw();
    visRange2_->Draw();
    visRange3_->Draw();
    visPeriphery1_->Draw();
    visPeriphery2_->Draw();
  }
}

void Bot::moveForward()
{
  x_=x_+speed_*cos(theta_);
  y_=y_+speed_*sin(theta_);
  bouncyBoundaries();
}

void Bot::moveBackward()
{
  x_=x_-speed_*cos(theta_);
  y_=y_-speed_*sin(theta_);
  bouncyBoundaries();
}

void Bot::seeEntity(Entity *entity)
{
  if (entity!=this)
  {
    double x=entity->x_;
    double y=entity->y_;
    double distsq=pow(x_-x, 2)+pow(y_-y, 2);
    if (distsq<10000) // If beyond this, it doesn't see it
    {
      double thetac=convertToZeroToPi(atan2((y-y_), (x-x_)));
      bool seen=false;
      if (inBetween(thetac, theta_-visualAngle_/6., theta_-visualAngle_/2.))
      {
        brain_->neurons_.at(3)->receive(1);
        seen=true;
      }
      else if (inBetween(thetac, theta_+visualAngle_/6., theta_-visualAngle_/6.))
      {
        brain_->neurons_.at(4)->receive(1);
        seen=true;
      }
      else if (inBetween(thetac, theta_+visualAngle_/2., theta_+visualAngle_/6.))
      {
        brain_->neurons_.at(5)->receive(1);
        seen=true;
      }
      
      if (seen)
      {
        // std::cout<<"Seen within fov"<<std::endl;
        // std::cout<<"theta_ = "<<theta_<<", thetac = "<<thetac<<" (x = "<<x<<", y = "<<y<<", x_ = "<<x_<<", y_ = "<<y<<")"<<std::endl;
        if (distsq<900)
        {
          brain_->neurons_.at(0)->receive(1);
        }
        else if (distsq<3600)
        {
          brain_->neurons_.at(1)->receive(1);
        }
        else
        {
          brain_->neurons_.at(2)->receive(1);
        }
        
        // What kind of an entity is seen?
        std::string type=entity->type_;
        if (type=="Food")
        {
          brain_->neurons_.at(6)->receive(1);
        }
        else if (type=="Bot")
        {
          brain_->neurons_.at(7)->receive(1);
        }
        else if (type=="Predator")
        {
          brain_->neurons_.at(8)->receive(1);
        }
        else
        {
          std::cout<<"ERROR: Entity seen has no known type!"<<std::endl;
        }
      }
    }
  }
}

void Bot::seeFoods(std::vector<Food*> *foods)
{
  for (unsigned int i=0; i<foods->size(); ++i)
  {
    seeEntity((Entity*)foods->at(i));
  }
}

void Bot::seeBots(std::vector<Bot*> *bots)
{
  for (unsigned int i=0; i<bots->size(); ++i)
  {
    seeEntity((Entity*)bots->at(i));
  }
}

void Bot::stepInTime()
{
  brain_->stepInTime();
  if (brain_->neurons_.at(12)->potential()>0.4) moveForward();
  if (brain_->neurons_.at(13)->potential()>0.4) turnLeft();
  if (brain_->neurons_.at(14)->potential()>0.4) turnRight();
  if (brain_->neurons_.at(15)->potential()>0.4) moveBackward();
}

void Bot::printBrain()
{
  brain_->print();
} 
