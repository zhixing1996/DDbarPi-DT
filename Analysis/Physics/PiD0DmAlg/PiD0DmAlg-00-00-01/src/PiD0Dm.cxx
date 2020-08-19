//e+e- --> Pi+ D0 D-, D0 --> 0/1/3/4, D- --> 201/201/202/203/204
//by huyu
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/Event.h"
#include "EventModel/EventHeader.h"
#include "McTruth/McParticle.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecDTag.h"
#include "EvtRecEvent/EvtRecVeeVertex.h"
#include "EvtRecEvent/EvtRecPi0.h"
#include "DstEvent/TofHitStatus.h"

#include "TMath.h"
#include "TRandom.h"
#include "GaudiKernel/INTupleSvc.h"
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IHistogramSvc.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/TwoVector.h"
#include "CLHEP/Geometry/Point3D.h"

using CLHEP::Hep3Vector;
using CLHEP::Hep2Vector;
using CLHEP::HepLorentzVector;
#ifndef ENABLE_BACKWARDS_COMPATIBILITY
typedef HepGeom::Point3D<double> HepPoint3D;
#endif

#include "DTagTool/DTagTool.h"
#include "PiD0DmAlg/PiD0Dm.h"
#include "MeasuredEcmsSvc/IMeasuredEcmsSvc.h"

//#include "VertexFit/KinematicFit.h"
#include "VertexFit/VertexFit.h"
#include "ParticleID/ParticleID.h"
#include "VertexFit/KalmanKinematicFit.h"
#include "VertexFit/Helix.h"
#include <vector>


const double velc = 299.792458;   // tof path unit in mm
typedef std::vector<int> Vint;
typedef std::vector<double> Vdouble;
typedef std::vector<RecMdcKalTrack*> VRecTrack;
typedef std::vector<RecEmcShower*> VRecShower;
typedef std::vector<WTrackParameter> VWTrack;
typedef std::vector<HepLorentzVector> Vp4;
int Ncut0,Ncut1,Ncut2,Ncut3,Ncut4,Ncut5,Ncut6,Ncut7,Ncut8,Ncut9,Ncut10,Ncut11,Ncut12,Ncut13,Ncut14,Ncut15,Ncut16,Ncut17,Ncut41;

///////////////////////////////////////////////////////////////

PiD0Dm::PiD0Dm(const std::string& name, ISvcLocator* pSvcLocator) :
  Algorithm(name, pSvcLocator){
	  declareProperty("AddModesD0",       m_modes_D0);
	  declareProperty("AddModesDp",       m_modes_Dp);
	  declareProperty("setecms",          setecms);
//	  declareProperty("setecms",        m_ecms);
}

StatusCode PiD0Dm::initialize(){

  std::cout << "initialize()" << endmsg;
  MsgStream log(msgSvc(), name());

  log << MSG::INFO << "in initialize()" << endmsg;

  StatusCode status;

  NTuplePtr nt4(ntupleSvc(), "FILE1/kDDbar");
  if ( nt4 ) m_tuple4 = nt4;
  else {
    m_tuple4 = ntupleSvc()->book ("FILE1/kDDbar", CLID_ColumnWiseTuple, "DTag N-Tuple example");
    if ( m_tuple4 ){
      status = m_tuple4->addItem ("ecms",               k_ecms);
      status = m_tuple4->addItem ("D0_mode",             k_D0_mode);
      status = m_tuple4->addItem ("D0_type",             k_D0_type);
      status = m_tuple4->addItem ("D0_dcharge",          k_D0_dcharge);
      status = m_tuple4->addItem ("D0_charm",            k_D0_charm);
      status = m_tuple4->addItem ("D0_mBC",              k_D0_mBC);
      status = m_tuple4->addItem ("D0_numofchildren",    k_D0_numofchildren);
      status = m_tuple4->addItem ("D0_mass",             k_D0_mass);
      status = m_tuple4->addItem ("D0_BE",               k_D0_be);
      status = m_tuple4->addItem ("D0_deltaE",           k_D0_deltae);
      status = m_tuple4->addItem ("Dm_mode",             k_Dm_mode);
      status = m_tuple4->addItem ("Dm_type",             k_Dm_type);
      status = m_tuple4->addItem ("Dm_dcharge",          k_Dm_dcharge);
      status = m_tuple4->addItem ("Dm_charm",            k_Dm_charm);
      status = m_tuple4->addItem ("Dm_mBC",              k_Dm_mBC);
      status = m_tuple4->addItem ("Dm_numofchildren",    k_Dm_numofchildren);
      status = m_tuple4->addItem ("Dm_mass",             k_Dm_mass);
      status = m_tuple4->addItem ("Dm_BE",               k_Dm_be);
      status = m_tuple4->addItem ("Dm_deltaE",           k_Dm_deltae);
      status = m_tuple4->addItem ("chisq_4c",               k_chisq);
      status = m_tuple4->addItem ("D0_px_4c",               k_D0_px);
      status = m_tuple4->addItem ("D0_py_4c",               k_D0_py);
      status = m_tuple4->addItem ("D0_pz_4c",               k_D0_pz);
      status = m_tuple4->addItem ("D0_p_4c",                k_D0_p);
      status = m_tuple4->addItem ("D0_E_4c",                k_D0_e);
      status = m_tuple4->addItem ("D0_m_4c",                k_D0_m);
      status = m_tuple4->addItem ("Dm_px_4c",            k_Dm_px);
      status = m_tuple4->addItem ("Dm_py_4c",            k_Dm_py);
      status = m_tuple4->addItem ("Dm_pz_4c",            k_Dm_pz);
      status = m_tuple4->addItem ("Dm_p_4c",             k_Dm_p);
      status = m_tuple4->addItem ("Dm_E_4c",             k_Dm_e);
      status = m_tuple4->addItem ("Dm_m_4c",             k_Dm_m);
      status = m_tuple4->addItem ("DDbar_px_4c",         k_DDbar_px);
      status = m_tuple4->addItem ("DDbar_py_4c",         k_DDbar_py);
      status = m_tuple4->addItem ("DDbar_pz_4c",         k_DDbar_pz);
      status = m_tuple4->addItem ("DDbar_p_4c",          k_DDbar_p);
      status = m_tuple4->addItem ("DDbar_E_4c",          k_DDbar_e);
      status = m_tuple4->addItem ("DDbar_m_4c",          k_DDbar_m);
      status = m_tuple4->addItem ("DDbarrecoil_m_4c",    k_DDbarrecoil_m);
      status = m_tuple4->addItem ("piprecoil_m_4c",      k_piprecoil_m);
      status = m_tuple4->addItem ("pip_px_4c",           k_pip_px); 
      status = m_tuple4->addItem ("pip_py_4c",           k_pip_py);
      status = m_tuple4->addItem ("pip_pz_4c",           k_pip_pz);
      status = m_tuple4->addItem ("pip_p_4c",            k_pip_p);
      status = m_tuple4->addItem ("pip_E_4c",            k_pip_e);
      status = m_tuple4->addItem ("pip_m_4c",            k_pip_m);
      status = m_tuple4->addItem ("D0pip_m_4c",          k_D0pip_m);
      status = m_tuple4->addItem ("Dmpip_m_4c",          k_Dmpip_m);
      status = m_tuple4->addItem ("chisq_6c",               C_chisq);
      status = m_tuple4->addItem ("D0_px_6c",               C_D0_px);
      status = m_tuple4->addItem ("D0_py_6c",               C_D0_py);
      status = m_tuple4->addItem ("D0_pz_6c",               C_D0_pz);
      status = m_tuple4->addItem ("D0_p_6c",                C_D0_p);
      status = m_tuple4->addItem ("D0_E_6c",                C_D0_e);
      status = m_tuple4->addItem ("D0_m_6c",                C_D0_m);
      status = m_tuple4->addItem ("Dm_px_6c",            C_Dm_px);
      status = m_tuple4->addItem ("Dm_py_6c",            C_Dm_py);
      status = m_tuple4->addItem ("Dm_pz_6c",            C_Dm_pz);
      status = m_tuple4->addItem ("Dm_p_6c",             C_Dm_p);
      status = m_tuple4->addItem ("Dm_E_6c",             C_Dm_e);
      status = m_tuple4->addItem ("Dm_m_6c",             C_Dm_m);
      status = m_tuple4->addItem ("DDbar_px_6c",         C_DDbar_px);
      status = m_tuple4->addItem ("DDbar_py_6c",         C_DDbar_py);
      status = m_tuple4->addItem ("DDbar_pz_6c",         C_DDbar_pz);
      status = m_tuple4->addItem ("DDbar_p_6c",          C_DDbar_p);
      status = m_tuple4->addItem ("DDbar_E_6c",          C_DDbar_e);
      status = m_tuple4->addItem ("DDbar_m_6c",          C_DDbar_m);
      status = m_tuple4->addItem ("DDbarrecoil_m_6c",    C_DDbarrecoil_m);
      status = m_tuple4->addItem ("piprecoil_m_6c",      C_piprecoil_m);
      status = m_tuple4->addItem ("pip_px_6c",           C_pip_px); 
      status = m_tuple4->addItem ("pip_py_6c",           C_pip_py);
      status = m_tuple4->addItem ("pip_pz_6c",           C_pip_pz);
      status = m_tuple4->addItem ("pip_p_6c",            C_pip_p);
      status = m_tuple4->addItem ("pip_E_6c",            C_pip_e);
      status = m_tuple4->addItem ("pip_m_6c",            C_pip_m);
      status = m_tuple4->addItem ("D0pip_m_6c",          C_D0pip_m);
      status = m_tuple4->addItem ("Dmpip_m_6c",          C_Dmpip_m);
      status = m_tuple4->addItem ("indexmc",          k_idxmc,0,200);
      status = m_tuple4->addIndexedItem("pdgid",      k_idxmc,    k_pdgid);
      status = m_tuple4->addIndexedItem("trkidx",     k_idxmc,    k_trkIdx);
      status = m_tuple4->addIndexedItem("motheridx",  k_idxmc,    k_motherIdx);
      status = m_tuple4->addIndexedItem("motherid",   k_idxmc,    k_motherid);
      status = m_tuple4->addIndexedItem("mcpx",       k_idxmc,    k_mc_px);
      status = m_tuple4->addIndexedItem("mcpy",       k_idxmc,    k_mc_py);
      status = m_tuple4->addIndexedItem("mcpz",       k_idxmc,    k_mc_pz);
      status = m_tuple4->addIndexedItem("mcPt",       k_idxmc,    k_mc_pt);
      status = m_tuple4->addIndexedItem("mcP",        k_idxmc,    k_mc_p);
      status = m_tuple4->addIndexedItem("mcTheta",    k_idxmc,    k_mc_theta);
      status = m_tuple4->addIndexedItem("mcPhi",      k_idxmc,    k_mc_phi);
      status = m_tuple4->addIndexedItem("mcE",        k_idxmc,    k_mc_e);
      status = m_tuple4->addIndexedItem("mcM",        k_idxmc,    k_mc_m);
       
    }
       else       {
         log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple4) << endmsg;
         return StatusCode::FAILURE;
       }
  }

  //
  //--------end of book--------
  //

  log << MSG::INFO << "successfully return from initialize()" <<endmsg;
  return StatusCode::SUCCESS;
}
//***********************************************************************
StatusCode PiD0Dm::execute() {

  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in execute()" << endreq;
  SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
  int runNo=eventHeader->runNumber();
  int event=eventHeader->eventNumber();
  log << MSG::DEBUG <<"run, evtnum = "
      << runNo << " , "
      << event <<endreq;
  Ncut0++;
 
  SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);
  log << MSG::DEBUG <<"ncharg, nneu, tottks = "
  << evtRecEvent->totalCharged() << " , "
      << evtRecEvent->totalNeutral() << " , "
  << evtRecEvent->totalTracks() <<endreq;
          
  SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(),  EventModel::EvtRec::EvtRecTrackCol);
  ecms = setecms;
  m_ecms = ecms;
  k_ecms = ecms;  

  int flag1 = eventHeader->flag1();
  int flag2 = eventHeader->flag2();
  m_run = runNo;
  m_event = event;
  m_flag1 = flag1;
  m_flag2 = flag2;
  m_flag1_mc = flag1;
  m_flag2_mc = flag2;
  int m_numParticle = 0;
  if (eventHeader->runNumber()<0) {
      SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");
      if (!mcParticleCol) {
         std::cout << "Could not retrieve McParticelCol" << std::endl;
         return StatusCode::FAILURE;
      }
      else {
         bool psipDecay = false;
         bool strange = false;
         int rootIndex = -1;
         int mcorigin = 9030443; //4260
         // int mcorigin = 9020443;    //4415
       Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
       int pid = (*iter_mc)->particleProperty();

       if (pid == 90022 || pid == 80022) {
	       for (iter_mc++; iter_mc != mcParticleCol->end(); iter_mc++) {
		       if (!(*iter_mc)->decayFromGenerator()) continue;
		       int pdg = (*iter_mc)->particleProperty();
		       int motherpdg = ((*iter_mc)->mother()).particleProperty();
		       int idx = (*iter_mc)->trackIndex();
		       int midx  = ((*iter_mc)->mother()).trackIndex();   
		       int mcidx; 
		       if (idx == midx || midx == 0) mcidx = idx - 1;
		       else mcidx = midx -1;
		       double px = (*iter_mc)->initialFourMomentum().x();
		       double py = (*iter_mc)->initialFourMomentum().y();
		       double pz = (*iter_mc)->initialFourMomentum().z();
		       double theta = (*iter_mc)->initialFourMomentum().theta();
		       double phi = (*iter_mc)->initialFourMomentum().phi();
		       double e = (*iter_mc)->initialFourMomentum().e();
		       double m = (*iter_mc)->initialFourMomentum().m();
		       double p = sqrt(px*px+py*py+pz*pz);
		       double pt = sqrt(px*px+py*py);
		       k_pdgid[m_numParticle] = pdg;
		       k_trkIdx[m_numParticle]=rootIndex;
		       k_motherIdx[m_numParticle]=mcidx;
		       k_motherid[m_numParticle]=motherpdg;
		       k_mc_px[m_numParticle] = px;
		       k_mc_py[m_numParticle] = py;
		       k_mc_pz[m_numParticle] = pz;
		       k_mc_p[m_numParticle] = p;
		       k_mc_pt[m_numParticle] = pt;
		       k_mc_theta[m_numParticle] = theta;
		       k_mc_phi[m_numParticle] = phi;
		       k_mc_e[m_numParticle]=e;
		       k_mc_m[m_numParticle]=m;
		       m_numParticle += 1;
		}
       }
       k_idxmc = m_numParticle;
    }
  }

  //use util tool to get dtaglist
  DTagTool dtagTool;
  if( dtagTool.isDTagListEmpty() ){
    return StatusCode::SUCCESS;
  }
  Ncut1++;
  DTagToolIterator iter_begin = dtagTool.modes_begin();
  DTagToolIterator iter_end   = dtagTool.modes_end();
  HepLorentzVector ecms_lab = HepLorentzVector(sin(0.011)*ecms, 0. ,0., ecms);
  int ncharge = 0;
  double MassD0 = 1.86486;
  double MassDp = 1.86962;
  double CHISQ = 999.;
  HepLorentzVector besKpipp4 = 0;
  HepLorentzVector besKD0p4 = 0;
  HepLorentzVector besKDmp4 = 0;
  HepLorentzVector bestpip4 = 0;
  RecMdcKalTrack* bestpiTrkex = NULL;
  EvtRecDTagCol::iterator besttag1; 
  EvtRecDTagCol::iterator besttag2;

  KalmanKinematicFit *kmfit = KalmanKinematicFit::instance();
  //access all canddidates of one mode
  for ( int modeA = 0; modeA != m_modes_D0.size(); modeA++ ){
	  for ( int modeB = 0; modeB != m_modes_Dp.size(); modeB++ ){ 
		  Vint ipip, ipim;
		  ipip.clear();
		  ipim.clear();
		  Vp4 ppip, ppim;
		  ppip.clear();
		  ppim.clear();

		  bool dtagflag = false;
		  dtagflag = dtagTool.findDDTag(m_modes_D0[modeA], m_modes_Dp[modeB], "inv");
		  if(dtagflag == false)  continue;

		  EvtRecDTagCol::iterator tag1 = dtagTool.dtag1();
		  EvtRecDTagCol::iterator tag2 = dtagTool.dtag2();

		  Ncut2++;
		  //cout<<"modeA =  "<<m_modes_D0[modeA]<<"  modeB= "<<m_modes_Dp[modeB]<<endl;
		  //cout<<"Tag mode:   "<<(*tag1)->charm()<<"  "<<(*tag1)->decayMode ()<<"  "<<(*tag2)->charm()<<"  "<<(*tag2)->decayMode ()<<endl;

		  //if(dtagTool.shareTracks(tag1, tag2)) continue;

		  SmartRefVector<EvtRecTrack> Tracks1  = (*tag1)->tracks();
		  SmartRefVector<EvtRecTrack> Tracks2  = (*tag2)->tracks();
		  SmartRefVector<EvtRecTrack> Showers1 = (*tag1)->showers();
		  SmartRefVector<EvtRecTrack> Showers2 = (*tag2)->showers();
		  SmartRefVector<EvtRecTrack> OtherTracks1  = (*tag1)->otherTracks();
		  SmartRefVector<EvtRecTrack> OtherTracks2  = (*tag2)->otherTracks();

		  for(int tk=0; tk<OtherTracks1.size(); tk++){
			  int t = 0;
			  for(int i=0; i<Tracks2.size(); i++){
				  if(OtherTracks1[tk]->trackId() == Tracks2[i]->trackId()) t=1;
			  }
			  if (t==1) continue;
			  RecMdcKalTrack *mdcTrk = OtherTracks1[tk]->mdcKalTrack();
			  if(dtagTool.isPion(OtherTracks1[tk])&&dtagTool.isGoodTrack(OtherTracks1[tk])){
				  HepLorentzVector ptrk;
				  ptrk.setPx(mdcTrk->px());
				  ptrk.setPy(mdcTrk->py());
				  ptrk.setPz(mdcTrk->pz());
				  double p3 = ptrk.rho();
				  ptrk.setE(sqrt(p3*p3+mpi*mpi));
				  if(mdcTrk->charge() >0) {
					  ppip.push_back(ptrk);
					  ipip.push_back(tk);
				  }
				  if(mdcTrk->charge() <0){
					  ppim.push_back(ptrk);
					  ipim.push_back(tk);
				  }
			  }
		  }
		  bool isPipD0Dm = ppip.size()==1 && ppim.size()==0 && (*tag1)->charm()==1 && (*tag2)->charm()==-1;
		  bool isPimD0barDp = ppip.size()==0 && ppim.size()==1 && (*tag1)->charm()==-1 && (*tag2)->charm()==1;
		  //cout<<isPipD0Dm<<"   "<< isPimD0barDp<<endl;
		  if(!(isPipD0Dm || isPimD0barDp)) continue;

		  Ncut8++;

		  // Begain to fill the Kinematic Fitter
		  kmfit->init();
		  // Tracks of first D
		  for(int i=0; i<Tracks1.size(); i++){
			  RecMdcKalTrack *kapiTrk = Tracks1[i]->mdcKalTrack();
			  WTrackParameter wvkapiTrk = WTrackParameter(mPart_D0[(*tag1)->decayMode()][i], kapiTrk->getZHelix(), kapiTrk->getZError());
			  kmfit->AddTrack(i, wvkapiTrk);
		  }
		  // Showers of first D
		  for(int i=0; i<Showers1.size(); i++){
			  RecEmcShower* gamShower = Showers1[i]->emcShower();
			  kmfit->AddTrack(i+Tracks1.size(), 0.0, gamShower);
		  }

		  // Tracks of secound D
		  for(int i=0; i<Tracks2.size(); i++){
			  RecMdcKalTrack *kapiTrk = Tracks2[i]->mdcKalTrack();
			  WTrackParameter wvkapiTrk = WTrackParameter(mPart_Dp[(*tag2)->decayMode()-200][i], kapiTrk->getZHelix(), kapiTrk->getZError());
			  kmfit->AddTrack(i+Tracks1.size()+Showers1.size(), wvkapiTrk);
		  }
		  // Showers of secound D
		  for(int i=0; i<Showers2.size(); i++){
			  RecEmcShower* gamShower = Showers2[i]->emcShower();
			  kmfit->AddTrack(i+Tracks1.size()+Showers1.size()+Tracks2.size(), 0.0, gamShower);
		  }

		  // The pion out of D 
		  RecMdcKalTrack* piex; 
		  WTrackParameter wvpiexTrk;
	      HepLorentzVector pipp4; 
		  if((*tag1)->charm()==1){ 
			  piex = OtherTracks1[ipip[0]]->mdcKalTrack();
			  pipp4 = ppip[0]; 
			  wvpiexTrk = WTrackParameter(mpi, piex->getZHelix(), piex->getZError());
		  }
		  else if((*tag1)->charm()==-1) {
			  piex = OtherTracks1[ipim[0]]->mdcKalTrack();
			  pipp4 = ppim[0]; 
			  wvpiexTrk = WTrackParameter(mpi, piex->getZHelix(), piex->getZError());
		  }
		  kmfit->AddTrack(Tracks1.size()+Showers1.size()+Tracks2.size()+Showers2.size(), wvpiexTrk);
		  kmfit->AddFourMomentum(0, ecms_lab);

		  bool oksq = kmfit->Fit();
		  if(!oksq) continue;
		  double chisq = kmfit->chisq();
		  //cout<<"4C fit result: "<<oksq<<"  "<<kmfit->chisq()<<endl;
 
		  //Get the P4 of two D and pion from Fitter
		  HepLorentzVector kD0p4 = 0;
		  for(int i=0; i<Tracks1.size(); i++){
	              kD0p4+=kmfit->pfit(i);
		  }
		  for(int i=0; i<Showers1.size(); i++){
	              kD0p4+=kmfit->pfit(i+Tracks1.size());
		  }
		  HepLorentzVector kDmp4 = 0;
		  for(int i=0; i<Tracks2.size(); i++){
	              kDmp4+=kmfit->pfit(i+Tracks1.size()+Showers1.size());
		  }
		  for(int i=0; i<Showers2.size(); i++){
	              kDmp4+=kmfit->pfit(i+Tracks1.size()+Showers1.size()+Tracks2.size());
		  }
		  HepLorentzVector kpipp4 = kmfit->pfit(Tracks1.size()+Showers1.size()+Tracks2.size()+Showers2.size());

		  // Select the best combination 
		  if(chisq<CHISQ){
			  CHISQ = chisq;
			  besKD0p4 = kD0p4;
			  besKDmp4 = kDmp4;
			  besKpipp4 = kpipp4;
			  bestpip4 = pipp4;
			  bestpiTrkex = piex;
			  besttag1 = tag1;
			  besttag2 = tag2;
		  }
	  }//***** mode B
  }//***** mode A

  if (CHISQ > 200) return StatusCode::SUCCESS;
  k_chisq = CHISQ;
  k_D0_mode          = (*besttag1)->decayMode();
  k_D0_type          = (*besttag1)->type();
  k_D0_dcharge       = (*besttag1)->charge();
  k_D0_charm         = (*besttag1)->charm();
  k_D0_numofchildren = (*besttag1)->numOfChildren();
  k_D0_be            = (*besttag1)->beamE();
  k_D0_mass          = (*besttag1)->mass();
  k_D0_mBC           = (*besttag1)->mBC();
  k_D0_deltae        = (*besttag1)->deltaE();

  k_Dm_mode          = (*besttag2)->decayMode();
  k_Dm_type          = (*besttag2)->type();
  k_Dm_dcharge       = (*besttag2)->charge();
  k_Dm_charm         = (*besttag2)->charm();
  k_Dm_numofchildren = (*besttag2)->numOfChildren();
  k_Dm_be            = (*besttag2)->beamE();
  k_Dm_mass          = (*besttag2)->mass();
  k_Dm_mBC           = (*besttag2)->mBC();
  k_Dm_deltae        = (*besttag2)->deltaE();

  HepLorentzVector D0P4 = (*besttag1)->p4();
  HepLorentzVector DmP4 = (*besttag2)->p4();
  Ncut3++;

  saveDTInfok(besKD0p4, besKDmp4, besKpipp4);
  Ncut4++;

  SmartRefVector<EvtRecTrack> tracks1  = (*besttag1)->tracks();
  SmartRefVector<EvtRecTrack> tracks2  = (*besttag2)->tracks();
  SmartRefVector<EvtRecTrack> showers1 = (*besttag1)->showers();
  SmartRefVector<EvtRecTrack> showers2 = (*besttag2)->showers();
  SmartRefVector<EvtRecTrack> othertracks1  = (*besttag1)->otherTracks();
  SmartRefVector<EvtRecTrack> othertracks2  = (*besttag2)->otherTracks();

  // Begain to fill the Kinematic Fitter
  kmfit->init();
  // Tracks of first D
  for(int i=0; i<tracks1.size(); i++){
	  RecMdcKalTrack *kapiTrk = tracks1[i]->mdcKalTrack();
	  WTrackParameter wvkapiTrk = WTrackParameter(mPart_D0[(*besttag1)->decayMode()][i], kapiTrk->getZHelix(), kapiTrk->getZError());
	  kmfit->AddTrack(i, wvkapiTrk);
  }
  // Showers of first D
  for(int i=0; i<showers1.size(); i++){
	  RecEmcShower* gamShower = showers1[i]->emcShower();
	  kmfit->AddTrack(i+tracks1.size(), 0.0, gamShower);
  }

  // Tracks of secound D
  for(int i=0; i<tracks2.size(); i++){
	  RecMdcKalTrack *kapiTrk = tracks2[i]->mdcKalTrack();
	  WTrackParameter wvkapiTrk = WTrackParameter(mPart_Dp[(*besttag2)->decayMode()-200][i], kapiTrk->getZHelix(), kapiTrk->getZError());
	  kmfit->AddTrack(i+tracks1.size()+showers1.size(), wvkapiTrk);
  }
  // Showers of secound D
  for(int i=0; i<showers2.size(); i++){
	  RecEmcShower* gamShower = showers2[i]->emcShower();
	  kmfit->AddTrack(i+tracks1.size()+showers1.size()+tracks2.size(), 0.0, gamShower);
  }

  // The pion out of D 
  WTrackParameter wvpiexTrk = WTrackParameter(mpi, bestpiTrkex->getZHelix(), bestpiTrkex->getZError());
  kmfit->AddTrack(tracks1.size()+showers1.size()+tracks2.size()+showers2.size(), wvpiexTrk);

  //cout<<"Tag1: "<<(*besttag1)->decayMode()<<"  Track size: "<<tracks1.size()<<" shower size: "<<showers1.size()<<endl;
  //Add the mass constrain of D0 and D-
  if((*besttag1)->decayMode() == 0)      kmfit->AddResonance(0, MassD0, 0, 1);
  else if((*besttag1)->decayMode() == 1) kmfit->AddResonance(0, MassD0, 0, 1, 2, 3);
  else if((*besttag1)->decayMode() == 3) kmfit->AddResonance(0, MassD0, 0, 1, 2, 3);
  else if((*besttag1)->decayMode() == 4) kmfit->AddResonance(0, MassD0, 0, 1, 2, 3, 4, 5);
  else cout<<"Attention! You need add more mode!"<<endl;

  //cout<<"Tag2: "<<(*besttag2)->decayMode()<<"  Track size: "<<tracks2.size()<<" shower size: "<<showers2.size()<<endl;
  int nTr = tracks1.size()+showers1.size();
  if((*besttag2)->decayMode() == 200)      kmfit->AddResonance(1, MassDp, nTr+0, nTr+1, nTr+2);
  else if((*besttag2)->decayMode() == 201) kmfit->AddResonance(1, MassDp, nTr+0, nTr+1, nTr+2, nTr+3, nTr+4);
  else if((*besttag2)->decayMode() == 202) kmfit->AddResonance(1, MassDp, nTr+0, nTr+1, nTr+2);
  else if((*besttag2)->decayMode() == 203) kmfit->AddResonance(1, MassDp, nTr+0, nTr+1, nTr+2, nTr+3, nTr+4);
  else if((*besttag2)->decayMode() == 204) kmfit->AddResonance(1, MassDp, nTr+0, nTr+1, nTr+2, nTr+3, nTr+4);
  else cout<<"Attention! You need add more mode!"<<endl;

  kmfit->AddFourMomentum(2, ecms_lab);

  bool oksq = kmfit->Fit();
  //if(!oksq) continue;
  double chisq = kmfit->chisq();
  C_chisq = chisq;
  cout<<"6C fit result: "<<oksq<<"  "<<kmfit->chisq()<<endl<<endl;

  //Get the P4 of two D and pion from Fitter
  HepLorentzVector kD0p4 = 0;
  for(int i=0; i<tracks1.size(); i++){
	  kD0p4+=kmfit->pfit(i);
  }
  for(int i=0; i<showers1.size(); i++){
	  kD0p4+=kmfit->pfit(i+tracks1.size());
  }
  HepLorentzVector kDmp4 = 0;
  for(int i=0; i<tracks2.size(); i++){
	  kDmp4+=kmfit->pfit(i+tracks1.size()+showers1.size());
  }
  for(int i=0; i<showers2.size(); i++){
	  kDmp4+=kmfit->pfit(i+tracks1.size()+showers1.size()+tracks2.size());
  }
  HepLorentzVector kpipp4 = kmfit->pfit(tracks1.size()+showers1.size()+tracks2.size()+showers2.size());

  saveDTInfo6C(kD0p4, kDmp4, kpipp4);
  Ncut41++;
  m_tuple4->write();

  dtagTool.clear();
} //end of execute()



// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode PiD0Dm::finalize() {
  cout<<"total number:         "<<Ncut0<<endl;
  cout<<"DTagList is not Empty "<<Ncut1<<endl;
  cout<<"Best tagD             "<<Ncut2<<endl;
  cout<<"Best tagDandpipi      "<<Ncut8<<endl;
  cout<<"Best tagDtwo          "<<Ncut9<<endl;
  cout<<"Best tagD0            "<<Ncut7<<endl;
  cout<<"Best D0to00           "<<Ncut5<<endl;
  cout<<"Best D0to01           "<<Ncut6<<endl;
  cout<<"Best D0to03           "<<Ncut10<<endl;
  cout<<"Best D0to11           "<<Ncut11<<endl;
  cout<<"Best D0to13           "<<Ncut12<<endl;
  cout<<"Best D0to33           "<<Ncut13<<endl;
  cout<<"Best D0to04           "<<Ncut14<<endl;
  cout<<"Best D0to14           "<<Ncut15<<endl;
  cout<<"Best D0to34           "<<Ncut16<<endl;
  cout<<"Best D0to44           "<<Ncut17<<endl;
  cout<<"Good pipi             "<<Ncut3<<endl;
  cout<<"Pass kfit             "<<Ncut4<<endl;
  cout<<"Pass 6C kfit          "<<Ncut41<<endl;
  MsgStream log(msgSvc(), name());
  log << MSG::INFO << "in finalize()" << endmsg;
  return StatusCode::SUCCESS;
}

void PiD0Dm::saveDTInfok(HepLorentzVector kp4, HepLorentzVector krep4, HepLorentzVector goodpip){
          HepLorentzVector ecms_lab = HepLorentzVector(sin(0.011)*ecms, 0. ,0., ecms);
          HepLorentzVector DDbarrecoil = ecms_lab - kp4 - krep4;
          HepLorentzVector piprecoil = ecms_lab - goodpip ;
	  k_D0_px = kp4.px();
          k_D0_py = kp4.py();
          k_D0_pz = kp4.pz();
          k_D0_p  = kp4.rho();
          k_D0_e  = kp4.e();
          k_D0_m  = kp4.m();
          k_Dm_px = krep4.px();
          k_Dm_py = krep4.py();
          k_Dm_pz = krep4.pz();
          k_Dm_p  = krep4.rho();
          k_Dm_e  = krep4.e();
          k_Dm_m  = krep4.m();
          k_pip_px = goodpip.px();
          k_pip_py = goodpip.py();
          k_pip_pz = goodpip.pz();
          k_pip_p  = goodpip.rho();
          k_pip_e  = goodpip.e();
          k_pip_m  = goodpip.m();
          k_DDbar_px = (kp4+krep4).px();
          k_DDbar_py = (kp4+krep4).py();
          k_DDbar_pz = (kp4+krep4).pz();
          k_DDbar_p  = (kp4+krep4).rho();
          k_DDbar_e  = (kp4+krep4).e();
          k_DDbar_m  = (kp4+krep4).m();
          k_DDbarrecoil_m  = DDbarrecoil.m();
          k_piprecoil_m  = piprecoil.m();
          k_D0pip_m    = (kp4+goodpip).m();
          k_Dmpip_m  = (krep4+goodpip).m();

}


void PiD0Dm::saveDTInfo6C(HepLorentzVector kp4, HepLorentzVector krep4, HepLorentzVector goodpip){
          HepLorentzVector ecms_lab = HepLorentzVector(sin(0.011)*ecms, 0. ,0., ecms);
          HepLorentzVector DDbarrecoil = ecms_lab - kp4 - krep4;
          HepLorentzVector piprecoil = ecms_lab - goodpip ;
	  C_D0_px = kp4.px();
          C_D0_py = kp4.py();
          C_D0_pz = kp4.pz();
          C_D0_p  = kp4.rho();
          C_D0_e  = kp4.e();
          C_D0_m  = kp4.m();
          C_Dm_px = krep4.px();
          C_Dm_py = krep4.py();
          C_Dm_pz = krep4.pz();
          C_Dm_p  = krep4.rho();
          C_Dm_e  = krep4.e();
          C_Dm_m  = krep4.m();
          C_pip_px = goodpip.px();
          C_pip_py = goodpip.py();
          C_pip_pz = goodpip.pz();
          C_pip_p  = goodpip.rho();
          C_pip_e  = goodpip.e();
          C_pip_m  = goodpip.m();
          C_DDbar_px = (kp4+krep4).px();
          C_DDbar_py = (kp4+krep4).py();
          C_DDbar_pz = (kp4+krep4).pz();
          C_DDbar_p  = (kp4+krep4).rho();
          C_DDbar_e  = (kp4+krep4).e();
          C_DDbar_m  = (kp4+krep4).m();
          C_DDbarrecoil_m  = DDbarrecoil.m();
          C_piprecoil_m  = piprecoil.m();
          C_D0pip_m    = (kp4+goodpip).m();
          C_Dmpip_m  = (krep4+goodpip).m();

}
