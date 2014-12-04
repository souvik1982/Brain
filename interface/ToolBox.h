#include "TMath.h"

double pi=3.14159265358979;

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

std::string ftoa(double i) 
{
  char res[10];
  sprintf(res, "%f", i);
  std::string ret(res);
  return ret;
}
