#include<cmath>
#include<vector>

class particle {
	private:
		double eta, phi, pt, mass, numTracks;
		int type;
			
	public:
		particle(){	// Default Constructor
			eta = 0.0; phi = 0.0; pt = 0.0; mass = 0.0; type = -1; numTracks = 0.0;
		}
		
		double getType() {return type;}
		double getEta() {return eta;}
		double getPhi() {return phi;}
		double getPT() {return pt;}
		double getMass() {return mass;}
		double getNumberTracks() {return numTracks;}
		double getEnergy() {return sqrt(pt*cos(phi)*pt*cos(phi) + pt*sin(phi)*pt*sin(phi) + pt*sinh(eta)*pt*sinh(eta) + mass*mass);}
		double getXMomentum() {return pt*cos(phi);}
		double getYMomentum() {return pt*sin(phi);}
		double getZMomentum() {return pt*sinh(eta);}
	
		void setType(double typ) {type = typ;}
		void setPT(double pT) {pt = pT;}
		void setEta(double Eta) {eta = Eta;}
		void setPhi(double Phi) {phi = Phi;}
		void setMass(double m) {mass = m;}
		void setNumberTracks(double nTracks) {numTracks = nTracks;}
	
};

class event {
	private:
		std::vector<particle> jetList;	// Light Jets 
		std::vector<particle> bJetList;	// Bottom Jets 
		std::vector<particle> photonList;	// Photons
		std::vector<particle> leptonList;	// Leptons
		std::vector<particle> tauList;	// Tau
		particle missingEnergy;	// Missing pT
		
	public:
		std::vector<particle> getJetList() {return jetList;}
		std::vector<particle> getBJetList() {return bJetList;}
		std::vector<particle> getPhotonList() {return photonList;}
		std::vector<particle> getLeptonList() {return leptonList;}
		std::vector<particle> getTauList() {return tauList;}
		
		int getNumberJets() {return jetList.size();}
		int getNumberBJets() {return bJetList.size();}
		int getNumberPhotons() {return photonList.size();}
		int getNumberLeptons() {return leptonList.size();}
		int getNumberTaus() {return tauList.size();}
		double getMET() {return missingEnergy.getPT();}
	
		void addJet(particle jet) {jetList.push_back(jet);}
		void addBJet(particle bjet) {bJetList.push_back(bjet);}
		void addPhoton(particle photon) {photonList.push_back(photon);}
		void addLepton(particle lepton) {leptonList.push_back(lepton);}
		void addTau(particle tau) {tauList.push_back(tau);}
		void addMissingEnergy(particle me) {missingEnergy = me;}

		void sortJets();
		void sortPhotons();
		void sortLeptons();
		void sortTaus();
		void sortBJets();
		
};

bool sortByPT(particle lhs, particle rhs) {
	return lhs.getPT() > rhs.getPT();
};

void event::sortJets(){
	sort( jetList.begin(), jetList.end() , sortByPT );
};

std::string identifyParticle(particle Particle) {
	std::string particleIdentification = "Not a particle";
	if (Particle.getType() == 0 && fabs(Particle.getEta()) < 1.1 ){particleIdentification = "Photon";}
	else if (Particle.getType() == 4 && fabs(Particle.getEta()) < 2.4 && Particle.getPT() > 20 ){particleIdentification = "Jet";}
	else if ((Particle.getType() == 1 && fabs(Particle.getEta()) < 2.5 && Particle.getPT() > 20) || (Particle.getType() ==2 && Particle.getPT() > 20 && fabs(Particle.getEta()) < 2.5)){particleIdentification = "Lepton";}
	else if (Particle.getType() == 3){particleIdentification = "Tau";}
	else if (Particle.getType() == 6){particleIdentification = "Missing Energy";}
	return particleIdentification;
};
