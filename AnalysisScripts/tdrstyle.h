#include "TStyle.h"

TStyle* setTDRStyle()
{
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  // For the canvas
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(700); //Height of canvas
  tdrStyle->SetCanvasDefW(700); //Width of canvas
  tdrStyle->SetPalette(1,0); 
  
  // For the Pad
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadLeftMargin(0.14);
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  // For the frame
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);
  
  // For the Axis Titles
  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(62, "XYZ");
  tdrStyle->SetTitleSize(0.04, "XYZ");
  tdrStyle->SetTitleXOffset(1.0);
  tdrStyle->SetTitleYOffset(1.52);
  
  // For plots
  // tdrStyle->SetOptTitle(0);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetHistLineWidth(2);
  // tdrStyle->SetErrorX(0.);
  
  // For Legends
  tdrStyle->SetLegendFillColor(0);
  tdrStyle->SetLegendFont(42);
  tdrStyle->SetLegendBorderSize(0);
  
  return tdrStyle;
}
