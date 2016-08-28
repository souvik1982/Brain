#include "TH2F.h"
#include "TCanvas.h"
#include "TEllipse.h"
#include "TPaveText.h"
#include "stdlib.h"

#include "../../interface/ToolBox.h"

// double pi=3.14159265358979;

void DisplayNetwork(std::string filename)
{
  TFile *file=new TFile(filename.c_str());
  TH2F *h_distances=(TH2F*)file->Get("Brain/h_distances_matrix_6000");
  
  TCanvas *c_network=new TCanvas("c_network", "c_network", 900, 900);
  c_network->Range(-200, -200, 200, 200);
  
  int nNeurons=h_distances->GetNbinsX();
  for (unsigned int i=0; i<=nNeurons; ++i)
  {
    TEllipse *neuron=new TEllipse(100.*cos(2.*i*pi/double(nNeurons)), 100.*sin(2.*i*pi/double(nNeurons)), 5);
    neuron->Draw();
    TPaveText *txt=new TPaveText(120.*cos(2.*i*pi/double(nNeurons)), 120.*sin(2.*i*pi/double(nNeurons)), 120.*cos(2.*i*pi/double(nNeurons))+10, 120.*sin(2.*i*pi/double(nNeurons))+10);
    txt->AddText(itoa(i).c_str());
    txt->Draw();
  }
  for (unsigned int i=0; i<=nNeurons; ++i)
  {
    for (unsigned int j=0; j<=nNeurons; ++j)
    {
      TArrow *arrow=new TArrow(100.*cos(2.*i*pi/double(nNeurons)), 100.*sin(2.*i*pi/double(nNeurons)), 100.*cos(2.*j*pi/double(nNeurons)), 100.*sin(2.*j*pi/double(nNeurons)), 0.01);
      double distance=int(h_distances->GetBinContent(i, j)*100.)-2;
      std::cout<<"distance = "<<distance<<std::endl;
      arrow->SetLineWidth(distance);
      if (distance>0) arrow->Draw();
    }
  }
  
  c_network->Draw();
}
