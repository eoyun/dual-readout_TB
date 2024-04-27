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
    utility.loadped( ("/pnfs/knu.ac.kr/data/cms/store/user/sungwon/2022_DRC_TB_analysis/ped/mean/Run" + std::to_string(runNum) + "_pedestalHist_mean.root").c_str() );
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
    float S_ped[200];
    float C_ped[200];
    for (int i =0;i<200;i++){
         SiPM_s[i] = utility.getcid(TBdetector::detid::SiPM,2,5,(i/10+1)%2+i%10*2+21,i/10+21,false);
         SiPM_c[i] = utility.getcid(TBdetector::detid::SiPM,2,5,(i/10)%2+i%10*2+21,i/10+21,true);
	 S_ped[i] = utility.retrievePed(SiPM_s[i]);
	 C_ped[i] = utility.retrievePed(SiPM_c[i]);
	 //std::cout<<(i/10+1)%2+i%10*2+21<<" | "<<i/10+21<<std::endl;
    }
    TH1F *FWHM_S[200];
    TH1F *FWHM_C[200];
    for (int i=0;i<200;i++){
	FWHM_S[i] = new TH1F(Form("FWHM_%d_plate_%d_column_S",(i/10+1)%2+i%10*2+21,i/10+21),";bin;evt",1000,0,1000);
	FWHM_C[i] = new TH1F(Form("FWHM_%d_plate_%d_column_C",(i/10)%2+i%10*2+21,i/10+21),";bin;evt",1000,0,1000);

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
	    SiPM_s_waveform = SiPM_s_data[i].waveform();
    	    SiPM_c_waveform = SiPM_c_data[i].waveform();
	    short min_S = *min_element(SiPM_s_waveform.begin()+100,SiPM_s_waveform.begin()+300);
	    short min_C = *min_element(SiPM_c_waveform.begin()+100,SiPM_c_waveform.begin()+300);
	    short minindex_S = min_element(SiPM_s_waveform.begin()+100,SiPM_s_waveform.begin()+300)-SiPM_s_waveform.begin();
	    short minindex_C = min_element(SiPM_c_waveform.begin()+100,SiPM_c_waveform.begin()+300)-SiPM_c_waveform.begin();
	    short cut_s = (short)(S_ped[i]-min_S)*0.5;
	    short cut_c = (short)(C_ped[i]-min_C)*0.5;
	    int first_s=0;
	    int first_c=0;
	    int last_s=0;
	    int last_c=0;
	    int flag_s_first = 0;
	    int flag_c_first = 0;
	    int flag_s_last = 0;
	    int flag_c_last = 0;
	    int bin;
	    for (bin =0; bin < 200; bin++){
		if (minindex_S>100 && minindex_S<300){
		    if (cut_s>S_ped[i]-SiPM_s_waveform[minindex_S-bin]){
		        if (flag_s_first == 0){ first_s = minindex_S-bin; flag_s_first=1;}
		    }
		    if (cut_s>S_ped[i]-SiPM_s_waveform[minindex_S+bin]){
		        if (flag_s_last == 0){ last_s = minindex_S+bin; flag_s_last=1;}
		    }
		}
		if (minindex_C>100 && minindex_C<300){
		    if (cut_c>C_ped[i]-SiPM_c_waveform[minindex_C-bin]){
		        if (flag_c_first == 0){ first_c = minindex_C-bin; flag_c_first=1;}
		    }
		    if (cut_c>C_ped[i]-SiPM_c_waveform[minindex_C+bin]){
		        if (flag_c_last == 0){ last_c = minindex_C+bin; flag_c_last=1;}
		    }
		}
  	    }
	    //std::cout<<first_s<<" | "<<minindex_S<<" | "<<last_s<<std::endl;        
	   if (min_element(SiPM_s_waveform.begin(),SiPM_s_waveform.end()-23) - SiPM_s_waveform.begin()>100 && min_element(SiPM_s_waveform.begin(),SiPM_s_waveform.end()) - SiPM_s_waveform.begin()<300)FWHM_S[i]->Fill(last_s-first_s);
	   if (min_element(SiPM_c_waveform.begin(),SiPM_c_waveform.end()-23) - SiPM_c_waveform.begin()>100 && min_element(SiPM_c_waveform.begin(),SiPM_c_waveform.end()) - SiPM_c_waveform.begin()<300)FWHM_C[i]->Fill(last_c-first_c);
	   //FWHM_S[i]->Fill(last_s-first_s);
	   //FWHM_C[i]->Fill(last_c-first_c);
	   flag_s_first=0;
	   flag_c_first=0;
	   flag_s_last=0;
	   flag_c_last=0;
           first_s = 0;
           first_c = 0;
           last_s =0;
           last_c =0;
	}
        // Exercise 3 : Get data of M1T1C and M1T1S waveforms

        // Here we loop over waveform vectors, and fill histograms to draw waveform
        // To draw waveform, one can use weighted histogram of ROOT : histogram->Fill( bin number to fill weight, weight value to fill)
        // For example, to draw pre-shower detector waveform histogram, one can do : histogram->Fill(bin, psWaveform[bin + 1]);

            //Exercise 4 : Fill M1T1C and M1T1S waveforms in M1T1C, M1T1S histograms
        
        // Here we save the waveform plots in ./waveform/PS/ , ./waveform/C/ , ./waveform/S/ directories

        printProgress(iEvt + 1, totalEntry);
    }
    TFile * root_fp = new TFile(Form("FWHM_%d_SiPM_all.root",runNum),"recreate");
    for (int i=0;i<200;i++){
        FWHM_S[i]->Write();
        FWHM_C[i]->Write();
    }
    root_fp->Close();
}
