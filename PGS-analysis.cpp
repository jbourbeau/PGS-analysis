//
// PGS analysis code written by James Bourbeau, UW-Madison, April 2015.
//

// C++ libraries
#include <iostream>
#include <fstream>

#include "PGS-classes.h"
#include "PGS-cuts-histograms.h"

// ROOT libraries
#include <TH2F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TRint.h>
#include <TError.h>
#include <TStyle.h>

using namespace std;

void analyzeDataFile(char* dataFileName, eventsTracker &runInfo){
// Read in events from .lhco file, extracts any pertinant information (cross-section, number of events, etc.), 
// applies any cuts that are specified, fills any specified histograms appropriately. 
	
	ifstream lhcoFile;
	lhcoFile.open(dataFileName,ios::in);

	if (! lhcoFile) { cout << "Error openning lhco file...quitting..." << endl; }
	else { cout << "lhco file " << dataFileName << " opened..." <<  "\n" << endl; }

	char line[1000];
	bool foundFirstEvent = false; // Use this boolean to determine when the first event in the lhco is found
	bool foundCrossSection = false; // Use this boolean to determine when the cross-section in the lhco is found
	double crossSection;
		
	while(!foundFirstEvent)
	{
		lhcoFile.getline(line,999);

		if (strstr(line,"ntrk")) foundFirstEvent = true; // "ntrk" occurs in the line before the first event
		if (strstr(line,"Integrated") && !foundCrossSection)
		{  
			foundCrossSection = true;
			char* xSecChar = (char*)string(line).erase(0, 29).c_str();
			char* pEnd;
			crossSection = strtod(xSecChar,&pEnd);
		}
	}
	
	// Define variables to be used when parsing lhco file. 
	double itemp, eventNumber, trigger;
	int totalEventCounter = 0;
	int eventCounterPassCuts = 0;
	int n, particletype;
	double eta, phi, pT, mass, numTrack, bgTag, hadEM, dummy1, dummy2;
	
	for(int iEvent = 0 ; iEvent < 100000; iEvent++){
	
  		if (lhcoFile.eof()) {
			break; // Breaks iEvent for loop.
		}

		lhcoFile >>  itemp >> eventNumber >> trigger;
		if (lhcoFile.eof()) {
			break; // Breaks iEvent for loop.
		}

		event newEvent;
		
		/* 
		particle "type" decoder:
		particletype = 0 --> Photon
		particletype = 1 --> Electron
		particletype = 2 --> Muon
		particletype = 3 --> Hadronically-decaying Tau
		particletype = 4 --> Jet
		particletype = 5 --> Missing Transverse Energy
		*/
		
		particletype = 0;
		while (particletype!=6){	// type = 6 indicated the MET line, which is the last line for each event 
			
			lhcoFile >> n >> particletype >> eta >> phi >> pT >> mass >> numTrack >> bgTag >> hadEM >> dummy1 >> dummy2;

			particle tmpParticle;
			tmpParticle.setType(particletype);
			tmpParticle.setEta(eta);
			tmpParticle.setPhi(phi);
			tmpParticle.setPT(pT);
			tmpParticle.setMass(mass);
			tmpParticle.setNumberTracks(numTrack);
									
			if (identifyParticle(tmpParticle) == "Photon") {newEvent.addPhoton(tmpParticle);}
			else if (identifyParticle(tmpParticle) == "Jet") {newEvent.addJet(tmpParticle);}
			else if (identifyParticle(tmpParticle) == "Lepton") {newEvent.addLepton(tmpParticle);}
			else if (identifyParticle(tmpParticle) == "Tau") {newEvent.addTau(tmpParticle);}
			else if (identifyParticle(tmpParticle) == "Missing Energy") {newEvent.addMissingEnergy(tmpParticle);}
				
		}
		
		newEvent.sortJets();

		totalEventCounter++;
		if (passCuts(newEvent)) { 
			eventCounterPassCuts++;
			runInfo.fillNTracksHist(newEvent.getJetList()[0].getNumberTracks(),1.0);
			runInfo.fillJetMassHist(newEvent.getJetList()[0].getMass(),1.0);
		}
		
	}
	
	runInfo.setEventCounter(totalEventCounter);
	runInfo.setPassedCutsEventCounter(eventCounterPassCuts);
	runInfo.setCrossSection(crossSection);
	
	cout << "Parton-level Cross Section = " << runInfo.getCrossSection() << " pb" << "\n" << endl;
	cout << "Total number of events = " << runInfo.getEventCounter() << "\n" << endl;
	cout << "Number of events after cuts = " << runInfo.getPassedCutsEventCounter() << "\n" << endl;
	cout << "Acceptance = " << (runInfo.getPassedCutsEventCounter()/(double)runInfo.getEventCounter())*100 << " %" << "\n" << endl;
	cout << "Cross-section after cuts = " << (runInfo.getPassedCutsEventCounter()/(double)runInfo.getEventCounter())*100*runInfo.getCrossSection() << " pb" << "\n" << endl;
	cout << "----------------------------------------------------" << "\n" << endl;

	lhcoFile.close();

}

//-----------------------------------------------------------------------------------------------------------

int main(int argc, char* argv[]){

	TRint* r = new TRint("theApp",0,0); // Opens up an interactive session of ROOT
	gErrorIgnoreLevel = 1001; // Ignore ROOT info messages	
	
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	
 	TCanvas *c1 = new TCanvas ("c1","c1",1100,800);
  	c1->Divide(2,1);
  	c1->cd(1);
  	
	vector<eventsTracker> eventsInfo(argc-1);
	
	for (int j = 0; j < argc-1; j++){	// Loop over each file entered into the command line
    	analyzeDataFile(argv[j+1], eventsInfo[j]);
    	eventsInfo[j].printJetMassHist();
	}

	c1->cd(2);	
	for (int j = 0; j < argc-1; j++){	// Loop over each file entered into the command line
    	eventsInfo[j].printNTracksHist();
	}
	
	/*TLegend* jetMassLegend = new TLegend(0.65,0.75,0.89,0.88);
	jetMassLegend->AddEntry(jetMassHistQCD,"QCD Jet","l");
	jetMassLegend->AddEntry(jetMassHistZp,"Z' Jet","l");
	jetMassLegend->Draw();

	TLegend* nTracksLegend = new TLegend(0.65,0.75,0.89,0.88);
	nTracksLegend->AddEntry(jetMassHistQCD,"QCD Jet","l");
	nTracksLegend->AddEntry(jetMassHistZp,"Z' Jet","l");
	nTracksLegend->Draw();*/
	
	r->Run(); 
	
	return 0;
}
