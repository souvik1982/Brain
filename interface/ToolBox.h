#include "TMath.h"
#include "TRandom3.h"

#ifndef __GCCXML__

static const double pi=3.14159265358979;

inline int decodeDebug(int d, int i) 
{
  return (d>>i)&0x1;
}

inline double convertToZeroToPi(double angle)
{
  if (angle<0) return 2.*pi+angle;
  else return angle;
}

inline double checkAngle(double theta)
{
  if (theta<0) theta=2.*pi+theta;
  if (theta>2.*pi) theta=theta-2.*pi;
  return theta;
}

inline double pythDistance(double x1, double y1, double x2, double y2)
{
  return pow(pow(x1-x2, 2)+pow(y1-y2, 2), 0.5);
}

inline bool inBetween(double a, double x1, double x2)
{
  if (x1>a && a>x2) return true;
  else return false;
}

inline std::string itoa(int i) 
{
  char res[10];
  sprintf(res, "%d", i);
  std::string ret(res);
  return ret;
}

inline std::string ftoa(double i) 
{
  char res[10];
  sprintf(res, "%f", i);
  std::string ret(res);
  return ret;
}

static TRandom3 *r3=new TRandom3();

#endif
