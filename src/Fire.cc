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

#include "TMath.h"

#include "../interface/Fire.h"

Fire::Fire(double x, double y, double theta, double worldSize, int debug): Entity(worldSize)
{
  x_=x;
  y_=y;
  theta_=theta;
  if (debug == 1)
  {
    circle_=new TEllipse(x, y, 5., 5.);
    circle_->SetLineColor(kRed);
    circle_->SetFillColor(kYellow);
  }
}

void Fire::draw()
{
  circle_->SetX1(x_);
  circle_->SetY1(y_);
  circle_->Draw();
}

void Fire::moveForward()
{
  x_=x_+0.01*cos(theta_);
  y_=y_+0.01*sin(theta_);
  bouncyBoundaries();
}
