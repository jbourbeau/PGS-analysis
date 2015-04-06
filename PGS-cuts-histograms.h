//#include "Event.h"
#include <TH1F.h>


bool passCuts(event Event){
	bool passcuts = true;
	if (Event.getNumberJets() < 1){ passcuts = false; }
	//else if (Event.getJetList()[0].getPT() < 100){ passcuts = false; }
	if (Event.getMET() < 500){ passcuts = false; }
	//else if (Event.getNumberLeptons() > 0){ passcuts = false; }
	return passcuts;
}

class eventsTracker {
	private:
		int eventCounter;
		int eventCounterPassCuts;
		double crossSection;
		TH1F* nTracksHist;
		TH1F* jetMassHist;
	
	public:
		eventsTracker(){	// Default constructor
			eventCounter = 0; eventCounterPassCuts = 0; crossSection = 0.0;
			nTracksHist = new TH1F("Number of Tracks","# of Tracks",30,-0.5,29.5);
			jetMassHist = new TH1F("Jet Mass","Fraction of Events vs. Jet Mass [GeV]",30,0,60);
		}	
		~eventsTracker(){	// Destructor
			delete nTracksHist;
			delete jetMassHist;
		}
		
		int getEventCounter() {return eventCounter;}
		int getPassedCutsEventCounter() {return eventCounterPassCuts;}
		double getCrossSection() {return crossSection;}
		TH1F* getNTracksHist() {return nTracksHist;}
		TH1F* getJetMassHist() {return jetMassHist;}
	
		void setEventCounter(int counter) {eventCounter = counter;}
		void setPassedCutsEventCounter(int counter) {eventCounterPassCuts = counter;}
		void setCrossSection(double xsection) {crossSection = xsection;}
		
		void fillJetMassHist(double value, double weight) { jetMassHist->Fill(value,weight); }
		void fillNTracksHist(double value, double weight) { nTracksHist->Fill(value,weight); }
		
		void printJetMassHist(){
			jetMassHist->GetXaxis()->SetTitle("Jet Mass (GeV)");
			jetMassHist->GetYaxis()->SetTitle("Fraction of Events");
			jetMassHist->GetYaxis()->SetTitleOffset(1.6);	
			jetMassHist->GetXaxis()->SetTitleOffset(1.1);
			jetMassHist->Scale(1/(double)eventCounterPassCuts);
			jetMassHist->GetYaxis()->SetRangeUser(0., 0.25);
			jetMassHist->GetXaxis()->SetRangeUser(0., 55.0);
			jetMassHist->Draw("HIST SAME");
		}
		
		void printNTracksHist(){
			nTracksHist->GetXaxis()->SetTitle("# of Tracks");
			nTracksHist->GetYaxis()->SetTitle("Fraction of Events");
			nTracksHist->GetYaxis()->SetTitleOffset(1.6);	
			nTracksHist->GetXaxis()->SetTitleOffset(1.1);
			nTracksHist->Scale(1/(double)eventCounterPassCuts);
			nTracksHist->GetYaxis()->SetRangeUser(0., 0.8);
			nTracksHist->GetXaxis()->SetRangeUser(0., 16.0);
			nTracksHist->Draw("HIST SAME");
		}
};