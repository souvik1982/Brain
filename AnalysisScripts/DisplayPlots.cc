#include <TH1F.h>
#include <TGraph.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <iostream>
#include <TStyle.h>
#include <TPaveText.h>
#include <TArrow.h>

#include "/Users/souvik/HbbHbb_Run2/Analysis/TDRStyle.h"

void DisplayPlots()
{
  TFile *file=new TFile("../AnalyzeThis.root");
  
  TGraph *g_avgBrainSize_time=(TGraph*)file->Get("g_avgBrainSize_time");
  TGraph *g_avgBrainSize_generation=(TGraph*)file->Get("g_avgBrainSize_generation");
  TGraph *g_dtime_generation=(TGraph*)file->Get("g_dtime_generation");
  
  TStyle *tdrStyle=setTDRStyle();
  tdrStyle->cd();
    
  TCanvas *c_avgBrainSize_time=new TCanvas("c_avgBrainSize_time", "c_avgBrainSize_time", 700, 700);
  g_avgBrainSize_time->Draw("AL");
  c_avgBrainSize_time->SaveAs("c_avgBrainSize_time.png");
  
  TCanvas *c_avgBrainSize_generation=new TCanvas("c_avgBrainSize_generation", "c_avgBrainSize_generation", 700, 700);
  g_avgBrainSize_generation->Draw("AL");
  c_avgBrainSize_generation->SaveAs("c_avgBrainSize_generation.png");
  
  unsigned int nGenerations=g_dtime_generation->GetN();
  TH1F *h_dtime_generation=new TH1F("h_dtime_generation", "; generations; Average time to next meal", nGenerations, 0, nGenerations);
  for (unsigned int i=0; i<nGenerations; ++i)
  {
    double x,y;
    g_dtime_generation->GetPoint(i, x, y);
    h_dtime_generation->Fill(x, y);
  }
  int rebin=100;
  h_dtime_generation->Rebin(rebin);
  h_dtime_generation->Scale(1./double(rebin));
  TCanvas *c_dtime_generation=new TCanvas("c_dtime_generation", "c_dtime_generation", 700, 700);
  h_dtime_generation->Draw("HIST");
  c_dtime_generation->SaveAs("c_dtime_generation.png");
}

  
