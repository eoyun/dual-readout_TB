#!/bin/bash

for i in {674..681}
do
	#./TBdwc.exe $i -1
	#./TBauxPID.exe $i -1
	#./SiPM_FWHM_PID.exe $i -1
	#./SiPM_FWHM_PID_w.exe $i -1
	./SiPM_Int_PID_w_over_evt.exe $i -1
	#./SiPM_Peak_PID.exe $i -1
	#./SiPM_Peak_PID_w.exe $i -1
done	
