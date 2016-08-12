/* 
==================================================
 Entity
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
Copyright (C) 11 November 2013 Souvik Das
ALL RIGHTS RESERVED
=================================================
*/

#pragma once

#include "TEllipse.h"

class Entity
{
  public:
  
    TEllipse *circle_;
    double worldSize_;
    double x_, y_, theta_;
    std::string type_;
    
  public:
  
    Entity(double worldSize);
    void turnLeft();
    void turnRight();
    void circularBoundaries();
    void bouncyBoundaries();
};
