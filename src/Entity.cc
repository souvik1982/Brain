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

#include "../interface/Entity.h"
#include "../interface/ToolBox.h"

Entity::Entity(double worldSize)
{
  worldSize_=worldSize;
}

void Entity::turnLeft()
{
  theta_=checkAngle(theta_+0.1);
  // std::cout<<"turning left!"<<std::endl;
}

void Entity::turnRight()
{
  theta_=checkAngle(theta_-0.1);
  // std::cout<<"turning right!"<<std::endl;
}

void Entity::circularBoundaries()
{
  if (x_<0) x_=99;
  if (x_>worldSize_) x_=1;
  if (y_<0) y_=99;
  if (y_>worldSize_) y_=1;
}

void Entity::bouncyBoundaries()
{
  if (x_<0) {x_=2; theta_=pi-theta_;}
  if (x_>worldSize_) {x_=worldSize_-2; theta_=pi-theta_;}
  if (y_<0) {y_=2; theta_=-theta_;}
  if (y_>worldSize_) {y_=worldSize_-2; theta_=-theta_;}
}
