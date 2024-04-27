#include <stdexcept>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>

#include "TBevt.h"
#include "TButility.h"
#include "functions.cc"

#include "TROOT.h"
#include "TStyle.h"
#include <TChain.h>
#include <TFile.h>
#include <TTree.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

// This macro will draw single event waveform (without any PID cuts) from waveform ntuple
// This macro will store waveform plots of pre-shower, module1 tower1 C and S channels in .png format event by event

// How to execute
// On local or batch, run the following command :
// ./TBdrawWave.exe <run number> <max # of events to process>

int main(int argc, char** argv) {
    gStyle->SetOptStat(0);

    int runNum = std::stoi(argv[1]);
    int maxEntry = std::stoi(argv[2]);

    TCanvas* c = new TCanvas("c1", "c1");

    // Load ntuples using TChain. Need to change "getNtupleChain()" in function.cc to set proper path to your ntuples
    TChain* evtChain = getNtupleChain(runNum);
    TBevt<TBwaveform>* anEvt = new TBevt<TBwaveform>(); 
    evtChain->SetBranchAddress("TBevt", &anEvt);
    
    // Check total # of events to process
    int totalEntry = evtChain->GetEntries();
    std::cout << "Total entries : " << totalEntry << std::endl;
    if ( (maxEntry > 0) && (maxEntry < totalEntry) ) {
        totalEntry = maxEntry;
        std::cout << "Will process maximum " << std::to_string(totalEntry) << " events" << std::endl;
    }

    // Here we load mapping information to get cid (channel ID)
    TButility utility = TButility();
    utility.loading("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_analysis/mapping/mapping_Aug2022TB.root");
    //utility.loading("/gatbawi/dream/mapping/mapping_Aug2022TB.root");

    // Exercise starts here
    /*
    To read data from ntuples, we need to specify which channels we want to read from ntuple
    This can be done by providing the cid (channel ID) you want to read to ntuple event data
    Functions for getting cid is defined in `../include/TButility.h`
    Students are most likely to use `getcid(TBdetector::detid detid)` and `getcid(TBdetector::detid detid, int module_number, int tower_number, bool is_Cerenkov_channel)`
    List of TBdetector::detid can be found in `../include/TBdetector.h`
    
    To get cid of auxiliary detector, one can use getcid(TBdetector::detid detid)
    For example, to get pre-shower detector cid, one can do : TBcid ps_cid = utility.getcid(TBdetector::detid::preshower);
    
    To get module's cid, one can use getcid(TBdetector::detid detid, int module_number, int tower_number, bool is_Cerenkov_channel)
    For example, to get Module 2 Tower 1 Cerenkov channel (generic PMT), one can do : TBcid M2T1C_cid = utility.getcid(TBdetector::detid::PMT, 2, 1, true);
    For example, to get Module 1 Tower 4 Scintillation channel (generic PMT), one can do : TBcid M1T4S_cid = utility.getcid(TBdetector::detid::PMT, 1, 4, false);
    */
    
    // Exercise 1 : Define cid of both Module 1 Tower 1 Cerenkov channel and Scintillation channel (generic PMT)
    TBcid SiPM_s[200];
    TBcid SiPM_c[200];
    for (int i =0;i<200;i++){
         SiPM_s[i] = utility.getcid(TBdetector::detid::SiPM,2,5,(i/10+1)%2+i%10*2+21,i/10+21,false);
         SiPM_c[i] = utility.getcid(TBdetector::detid::SiPM,2,5,(i/10)%2+i%10*2+21,i/10+21,true);
	 //std::cout<<(i/10+1)%2+i%10*2+21<<" | "<<i/10+21<<std::endl;
    }
    TH2F *SiPM_s_hist[200];
    TH2F *SiPM_c_hist[200];
    for (int i=0;i<200;i++){
	SiPM_s_hist[i] = new TH2F(Form("%d_plate_%d_column_s",(i/10+1)%2+i%10*2+21,i/10+21),"bin;ADC",1000,0,1000,5000,-1000,4000);
	SiPM_c_hist[i] = new TH2F(Form("%d_plate_%d_column_c",(i/10)%2+i%10*2+21,i/10+21),"bin;ADC",1000,0,1000,5000,-1000,4000);
    }
    // Starting Evt Loop
    for (int iEvt = 0; iEvt < totalEntry; iEvt++) {
        // Get entry from ntuple TChain
        // Event data can be accessed from TBevt<TBwaveform>* anEvt
        evtChain->GetEntry(iEvt);

        // Here we define histograms to draw waveform plots of pre-shower, module 1 tower 1 ceren. and scint. channel
        //TH1F* psWaveformHist = new TH1F( ( "psWaveform_" + std::to_string(iEvt) ).c_str(), ("psWaveform_" + std::to_string(iEvt) + ";bin;ADC").c_str() , 1000, 0, 1000);
        //psWaveformHist->GetYaxis()->SetRangeUser(-100, 4096);
	//TH1F* SiPM_s_hist = new TH1F ("",";bin;ADC",1000,0,1000);
	//SiPM_s_hist->GetYaxis()->SetRangeUser(3000,4000);
	//TH1F* SiPM_c_hist = new TH1F ("",";bin;ADC",1000,0,1000);
	//SiPM_c_hist->GetYaxis()->SetRangeUser(3000,4000);
        // To get data (TBwaveform class object) from event, one cat use data(TBcid cid) function of TBevt class
        // anEvt->data(TBcid cid) will return TBwaveform class object of corresponding channel ID
        // For example, to get TBwaveform data of pre-shower detector, one cat do :
	TBwaveform SiPM_s_data[200];
	TBwaveform SiPM_c_data[200];
	for (int i = 0; i<200;i++){
	    SiPM_s_data[i] = anEvt->data(SiPM_s[i]);
	    SiPM_c_data[i] = anEvt->data(SiPM_c[i]);
	}
        // Exercise 2 : Get data of M1T1 C and S channel using cid


        // From TBwaveform data, one can get waveform vector
        // All waveforms are stored in std::vector<short> with size of 1024
        // We'll only use waveform[1] ~ waveform[1000] to remove weird waveforms
        // To get waveform vector, one can do : TBwaveform data; data.waveform();
        // For example, to get pre-shower detector waveform vector, one can do :
        std::vector<short> SiPM_s_waveform; // Your answer here
        std::vector<short> SiPM_c_waveform; // Your answer here
	for (int i=0;i<200;i++){
	    SiPM_s_waveform.clear();
	    SiPM_c_waveform.clear();
    	    //SiPM_s_hist->Reset();
    	    //SiPM_c_hist->Reset();
    	    //std::cout<<"hello"<<std::endl;
    	    double ped_s=0.;
    	    double ped_c=0.;
	    SiPM_s_waveform = SiPM_s_data[i].waveform();
    	    SiPM_c_waveform = SiPM_c_data[i].waveform();
    	    for (int bin=1;bin<101;bin++){
		ped_s+=(double)SiPM_s_waveform[bin+1]/100.;
		ped_c+=(double)SiPM_c_waveform[bin+1]/100.;
	    }
	    for (int bin =0; bin < 1000; bin++){
            	int waveformBin = bin + 1; // To loop over waveform[1] ~ waveform[1000]
	 	SiPM_s_hist[i]->Fill(bin,ped_s-SiPM_s_waveform[waveformBin]);   	
	 	SiPM_c_hist[i]->Fill(bin,ped_c-SiPM_c_waveform[waveformBin]);   	
  	    }

	}
        // Exercise 3 : Get data of M1T1C and M1T1S waveforms

        // Here we loop over waveform vectors, and fill histograms to draw waveform
        // To draw waveform, one can use weighted histogram of ROOT : histogram->Fill( bin number to fill weight, weight value to fill)
        // For example, to draw pre-shower detector waveform histogram, one can do : histogram->Fill(bin, psWaveform[bin + 1]);

            //Exercise 4 : Fill M1T1C and M1T1S waveforms in M1T1C, M1T1S histograms
        
        // Here we save the waveform plots in ./waveform/PS/ , ./waveform/C/ , ./waveform/S/ directories

        printProgress(iEvt + 1, totalEntry);
    }
    TFile * root_fp = new TFile(Form("%d_SiPm.root",runNum),"recreate");
    for (int i=0;i<200;i++){
        SiPM_s_hist[i]->Write();
        SiPM_c_hist[i]->Write();
    }
    root_fp->Close();
}
