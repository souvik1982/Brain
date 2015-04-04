/* 
==================================================
 Fire
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
Copyright (C) 11 November 2013 Souvik Das
ALL RIGHTS RESERVED
=================================================
*/

#pragma once

#include "Entity.h"

class Fire: public Entity
{
  public:
  
    Fire(double x, double y, double theta, double worldSize, int debug = 0);
    void draw();
    void moveForward();
};
