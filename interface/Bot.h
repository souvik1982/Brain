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

#include "Brain.h"
#include "Food.h"

#include "TLine.h"

class Bot: public Entity
{
  public:
  
    Brain *brain_;
    std::string name_;
    TLine *line1_, *line2_;
    double worldSize_;
    int debug_;
    int kids_;
    
  public:
  
    Bot(double x, double y, double theta, int brainSize, std::string name, double worldSize, int debug = 1);
    Bot(double x, double y, double theta, Bot *parentBot);
    ~Bot();
    
    void draw();
    void moveForward();
    bool inSight(double x, double y);
    void seeFood(std::vector<Food*> *foods);
    void stepInTime();
    
    void printBrain();
};  
