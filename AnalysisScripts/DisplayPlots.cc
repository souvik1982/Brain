#include <TH1F.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TFile.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <iostream>
#include <TStyle.h>
#include <TLegend.h>
#include <TArrow.h>

#include "tdrstyle.h"

void DisplayPlots(std::string filename)
{
  TFile *file=new TFile(filename.c_str());
  
  TGraph *g_avgBrainSize_time=(TGraph*)file->Get("g_avgBrainSize_time");
  TGraph *g_avgBrainSize_generation=(TGraph*)file->Get("g_avgBrainSize_generation");
  TGraph *g_dtime_generation=(TGraph*)file->Get("g_dtime_generation");
  TGraph *g_dtime_time=(TGraph*)file->Get("g_dtime_time");
  TGraph *g_avgBrainSize_predator_time=(TGraph*)file->Get("g_avgBrainSize_predator_time");
  TGraph *g_avgBrainSize_predator_generation=(TGraph*)file->Get("g_avgBrainSize_predator_generation");
  TGraph *g_dtime_predator_generation=(TGraph*)file->Get("g_dtime_predator_generation");
  TGraph *g_dtime_predator_time=(TGraph*)file->Get("g_dtime_predator_time");
  TH2F *h_distances=(TH2F*)file->Get("Brain/h_distances_matrix_6000"); 
  
  TStyle *tdrStyle=setTDRStyle();
    
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
  c_dtime_generation->SetLogy();
  h_dtime_generation->Draw("HIST E");
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
  c_dtime_predator_generation->SetLogy();
  h_dtime_predator_generation->Draw("HIST E");
  c_dtime_predator_generation->SaveAs("c_dtime_predator_generation.png");
  
  TH1F *h_dtime_time=new TH1F("h_dtime_time", "; time; Average time to next meal", 100, 0, 1500000);
  for (unsigned int i=0; i<g_dtime_time->GetN(); ++i)
  {
    double x, y;
    g_dtime_time->GetPoint(i, x, y);
    h_dtime_time->Fill(x, y);
  }
  // TH1F *h_dtime_predator_time=(TH1F*)h_dtime_time->Clone();
  // h_dtime_predator_time->Reset();
  // for (unsigned int i=0; i<g_dtime_predator_time->GetN(); ++i)
  // {
  //   double x_predator, y_predator;
  //   g_dtime_predator_time->GetPoint(i, x_predator, y_predator);
  //   h_dtime_predator_time->Fill(x_predator, y_predator);
  // }
  // rebin=100000;
  // h_dtime_time->Rebin(rebin);
  // h_dtime_time->Scale(1./double(rebin));
  h_dtime_time->SetLineColor(kBlue);
  //h_dtime_predator_time->Rebin(rebin);
  //h_dtime_predator_time->Scale(1./double(rebin));
  // h_dtime_predator_time->SetLineColor(kRed);
  TCanvas *c_dtime_time=new TCanvas("c_dtime_time", "c_dtime_time", 700, 700);
  // c_dtime_time->SetLogy();
  // h_dtime_predator_time->Draw("HIST");
  h_dtime_time->Draw("HIST");
  c_dtime_time->SaveAs("c_dtime_time.png");
  
  // h_distances->SetBinContent(1, 0);
  // h_distances->SetBinContent(2, 0);
  TCanvas *c_distances=new TCanvas("c_distances", "c_distances", 700, 700);
  h_distances->Draw("colz");
  leg=new TLegend(0.1, 0.9, 0.9, 1.0);
  leg->AddEntry(h_distances, "AI Brain", ""); leg->SetLineColor(kWhite); leg->SetFillColor(kWhite);
  leg->Draw();
  c_distances->SaveAs("c_distances.png");
}

  
