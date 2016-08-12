/* 
==================================================
 Food
 
 An original simulation of a non-traditional 
 biology-inspired neural network evolving in 
 a naturally selective environment to demonstrate
 the emergence of directed survival behavior.
 
Copyright (C) 11 November 2013 Souvik Das
ALL RIGHTS RESERVED
=================================================
*/

#include "TMath.h"

#include "../interface/Food.h"

Food::Food(double x, double y, double theta, double worldSize, int debug): Entity(worldSize)
{
  type_="Food";
  x_=x;
  y_=y;
  theta_=theta;
  circle_=new TEllipse(x, y, 3., 3.);
  circle_->SetLineColor(kGreen+2);
  circle_->SetFillColor(kGreen);
}

Food::~Food()
{
  circle_->Delete();
}

void Food::draw()
{
  circle_->SetX1(x_);
  circle_->SetY1(y_);
  circle_->Draw();
}

void Food::moveForward()
{
  x_=x_+0.01*cos(theta_);
  y_=y_+0.01*sin(theta_);
  bouncyBoundaries();
}
