#include <TH1F.h>
#include <TGraph.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <iostream>
#include <TStyle.h>
#include <TLegend.h>
#include <TArrow.h>

#include "/Users/souvik/HbbHbb_Run2/Analysis/TDRStyle.h"

void DisplayPlots()
{
  TFile *file=new TFile("../AnalyzeThis.root");
  
  TGraph *g_avgBrainSize_time=(TGraph*)file->Get("g_avgBrainSize_time");
  TGraph *g_avgBrainSize_generation=(TGraph*)file->Get("g_avgBrainSize_generation");
  TGraph *g_dtime_generation=(TGraph*)file->Get("g_dtime_generation");
  TGraph *g_avgBrainSize_predator_time=(TGraph*)file->Get("g_avgBrainSize_predator_time");
  TGraph *g_avgBrainSize_predator_generation=(TGraph*)file->Get("g_avgBrainSize_predator_generation");
  TGraph *g_dtime_predator_generation=(TGraph*)file->Get("g_dtime_predator_generation");
  TH1F *h_distances=(TH1F*)file->Get("h_distances"); 
  
  TStyle *tdrStyle=setTDRStyle();
  // tdrStyle->cd();
    
  TCanvas *c_avgBrainSize_time=new TCanvas("c_avgBrainSize_time", "c_avgBrainSize_time", 700, 700);
  g_avgBrainSize_time->SetLineColor(kBlue); g_avgBrainSize_time->Draw("AL");
  g_avgBrainSize_predator_time->SetLineColor(kRed); g_avgBrainSize_predator_time->Draw("SAME");
  TLegend *leg=new TLegend(0.15, 0.70, 0.55, 0.85);
  leg->AddEntry(g_avgBrainSize_time, "Prey", "l");
  leg->AddEntry(g_avgBrainSize_predator_time, "Predator", "l");
  leg->SetFillColor(kWhite);
  leg->Draw();
  c_avgBrainSize_time->SaveAs("c_avgBrainSize_time.png");
  
  TCanvas *c_avgBrainSize_generation=new TCanvas("c_avgBrainSize_generation", "c_avgBrainSize_generation", 700, 700);
  g_avgBrainSize_generation->SetLineColor(kBlue);
  g_avgBrainSize_generation->Draw("AL");
  c_avgBrainSize_generation->SaveAs("c_avgBrainSize_generation.png");
  
  TCanvas *c_avgBrainSize_predator_generation=new TCanvas("c_avgBrainSize_predator_generation", "c_avgBrainSize_predator_generation", 700, 700);
  g_avgBrainSize_predator_generation->SetLineColor(kRed);
  g_avgBrainSize_predator_generation->Draw("AL");
  c_avgBrainSize_predator_generation->SaveAs("c_avgBrainSize_predator_generation.png");
  
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
  h_dtime_generation->SetLineColor(kBlue);
  TCanvas *c_dtime_generation=new TCanvas("c_dtime_generation", "c_dtime_generation", 700, 700);
  h_dtime_generation->Draw("HIST");
  c_dtime_generation->SaveAs("c_dtime_generation.png");
  
  unsigned int nGenerations_predator=g_dtime_predator_generation->GetN();
  TH1F *h_dtime_predator_generation=new TH1F("h_dtime_predator_generation", "; generations; Average time for predator to next meal", nGenerations_predator, 0, nGenerations_predator);
  for (unsigned int i=0; i<nGenerations_predator; ++i)
  {
    double x,y;
    g_dtime_predator_generation->GetPoint(i, x, y);
    h_dtime_predator_generation->Fill(x, y);
  }
  rebin=100;
  h_dtime_predator_generation->Rebin(rebin);
  h_dtime_predator_generation->Scale(1./double(rebin));
  h_dtime_predator_generation->SetLineColor(kRed);
  TCanvas *c_dtime_predator_generation=new TCanvas("c_dtime_predator_generation", "c_dtime_predator_generation", 700, 700);
  h_dtime_predator_generation->Draw("HIST");
  c_dtime_predator_generation->SaveAs("c_dtime_predator_generation.png");
  
  h_distances->Rebin(2);
  // h_distances->SetBinContent(1, 0);
  // h_distances->SetBinContent(2, 0);
  TCanvas *c_distances=new TCanvas("c_distances", "c_distances", 700, 700);
  c_distances->SetLogy();
  h_distances->Draw("hist");
  leg=new TLegend(0.5, 0.6, 0.7, 0.8);
  leg->AddEntry(h_distances, "AI Brain", ""); leg->SetLineColor(kWhite); leg->SetFillColor(kWhite);
  leg->Draw();
  c_distances->SaveAs("c_distances.png");
}

  
