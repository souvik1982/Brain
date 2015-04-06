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

#pragma once

#include "Brain.h"
#include "Food.h"

#include "TLine.h"

class Bot: public Entity
{
  public:
  
    Brain *brain_;
    std::string name_;
    TLine *line1_, *line2_;
    int bodyColor_;
    double speed_;
    double worldSize_;
    int debug_;
    int kids_;
    
  public:
  
    Bot(double x, double y, double theta, int brainSize, int bodyColor, double speed, std::string name, double worldSize, int debug = 1);
    Bot(Bot *parentBot, double mu_newNeuron, double mu_newConnection, double mu_modConnection);
    ~Bot();
    
    void draw();
    void moveForward();
    void moveBackward();
    bool inSight(double x, double y);
    void seeFood(std::vector<Food*> *foods);
    void seeBots(std::vector<Bot*> *bots);
    void stepInTime();
    
    void printBrain();
};  
