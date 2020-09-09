//e+e- --> Pi+ D0 D-, D0 --> 0/1/3/4, D- --> 201/201/202/203/204
//by huyu
#include "PiD0DmAlg/PiD0Dm.h"
///////////////////////////////////////////////////////////////

VertexFit * vtxfit = VertexFit::instance();

DECLARE_ALGORITHM_FACTORY( PiD0Dm )
DECLARE_FACTORY_ENTRIES( PiD0DmAlg ) {
    DECLARE_ALGORITHM( PiD0Dm );
}

LOAD_FACTORY_ENTRIES( PiD0DmAlg )

PiD0Dm::PiD0Dm(const std::string& name, ISvcLocator* pSvcLocator) :
    Algorithm(name, pSvcLocator) {
        declareProperty("AddModesD0", m_modes_D0);
        declareProperty("AddModesDp", m_modes_Dp);
        declareProperty("Ecms", m_ecms);
        declareProperty("Debug", m_debug = false);
}

StatusCode PiD0Dm::initialize() {
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "in initialize()" << endmsg;
    StatusCode status;

    NTuplePtr nt1(ntupleSvc(), "FILE1/DTD");
    if (nt1) m_tuple1 = nt1;
    else {
        m_tuple1 = ntupleSvc()->book ("FILE1/DTD", CLID_ColumnWiseTuple, "double D tag method");
        if (m_tuple1) {
            status = m_tuple1->addItem("runNo",                  m_runNo);
            status = m_tuple1->addItem("evtNo",                  m_evtNo);
            status = m_tuple1->addItem("flag1",                  m_flag1);
            status = m_tuple1->addItem("flag2",                  m_flag2);
            status = m_tuple1->addItem("indexmc",                m_idxmc, 0, 100);
            status = m_tuple1->addIndexedItem("pdgid",           m_idxmc, m_pdgid);
            status = m_tuple1->addIndexedItem("trkidx",          m_idxmc, m_trkidx);
            status = m_tuple1->addIndexedItem("motherid",        m_idxmc, m_motherid);
            status = m_tuple1->addIndexedItem("motheridx",       m_idxmc, m_motheridx);
            status = m_tuple1->addItem("D0_mode",                m_D0_mode);
            status = m_tuple1->addItem("D0_type",                m_D0_type);
            status = m_tuple1->addItem("D0_dcharge",             m_D0_dcharge);
            status = m_tuple1->addItem("D0_charm",               m_D0_charm);
            status = m_tuple1->addItem("D0_mBC",                 m_D0_mBC);
            status = m_tuple1->addItem("D0_numofchildren",       m_D0_numofchildren);
            status = m_tuple1->addItem("D0_mass",                m_D0_mass);
            status = m_tuple1->addItem("D0_BE",                  m_D0_be);
            status = m_tuple1->addItem("D0_deltaE",              m_D0_deltae);
            status = m_tuple1->addItem("Dm_mode",                m_Dm_mode);
            status = m_tuple1->addItem("Dm_type",                m_Dm_type);
            status = m_tuple1->addItem("Dm_dcharge",             m_Dm_dcharge);
            status = m_tuple1->addItem("Dm_charm",               m_Dm_charm);
            status = m_tuple1->addItem("Dm_mBC",                 m_Dm_mBC);
            status = m_tuple1->addItem("Dm_numofchildren",       m_Dm_numofchildren);
            status = m_tuple1->addItem("Dm_mass",                m_Dm_mass);
            status = m_tuple1->addItem("Dm_BE",                  m_Dm_be);
            status = m_tuple1->addItem("Dm_deltaE",              m_Dm_deltae);
            status = m_tuple1->addItem("chisq_4c",               m_chisq_4c);
            status = m_tuple1->addItem("p4_D0_4c",               4, m_p4_D0_4c);
            status = m_tuple1->addItem("p4_Dm_4c",               4, m_p4_Dm_4c);
            status = m_tuple1->addItem("p4_pip_4c",              4, m_p4_pip_4c);
            status = m_tuple1->addItem("vtx_tagPi",              3, m_vtx_tagPi);
            status = m_tuple1->addItem("chisq_6c",               m_chisq_6c);
            status = m_tuple1->addItem("p4_D0_6c",               4, m_p4_D0_6c);
            status = m_tuple1->addItem("p4_Dm_6c",               4, m_p4_Dm_6c);
            status = m_tuple1->addItem("p4_pip_6c",              4, m_p4_pip_6c);
            status = m_tuple1->addItem("chisq_vf",               m_chisq_vf);
            status = m_tuple1->addItem("n_othershws",            m_n_othershws, 0, 50);
            status = m_tuple1->addIndexedItem("vtx_othershws",   m_n_othershws, 6, m_vtx_othershws);
            status = m_tuple1->addIndexedItem("rawp4_othershws", m_n_othershws, 4, m_rawp4_othershws);
            status = m_tuple1->addItem("n_pi0",                  m_n_pi0, 0, 200);
            status = m_tuple1->addIndexedItem("chi2_pi0",        m_n_pi0, m_chi2_pi0);
            status = m_tuple1->addIndexedItem("p4_pi0",          m_n_pi0, 4, m_p4_pi0);
        }
        else {
            log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple1) << endmsg;
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
    SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(), "/Event/EventHeader");
    runNo = eventHeader->runNumber();
    evtNo = eventHeader->eventNumber();
    log << MSG::DEBUG <<"run, evtnum = "
        << runNo << " , "
        << evtNo <<endreq;
    Ncut0++;
 
    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);
    log << MSG::DEBUG <<"ncharg, nneu, tottks = "
        << evtRecEvent->totalCharged() << " , "
        << evtRecEvent->totalNeutral() << " , "
        << evtRecEvent->totalTracks() <<endreq;
          
    SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol);

    flag1 = eventHeader->flag1();
    flag2 = eventHeader->flag2();
    int m_numParticle = 0;

    // save all MC Truth info
    if (eventHeader->runNumber() < 0) {
        SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");
        if (!mcParticleCol) {
            std::cout << "Could not retrieve McParticelCol" << std::endl;
            return StatusCode::FAILURE;
        }
        else {
            Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
            int pid = (*iter_mc)->particleProperty();
            unsigned int idx;
            unsigned int midx;
            idxmc = 0;
            // if (pid == 90022 || pid == 80022) {
            //     for (iter_mc++; iter_mc != mcParticleCol->end(); iter_mc++) {
            //         if (!(*iter_mc)->decayFromGenerator()) continue;
            //         pid = (*iter_mc)->particleProperty();
            //         idx = (*iter_mc)->trackIndex();
            //         midx  = ((*iter_mc)->mother()).trackIndex();   
            //         pdgid[idxmc] = pid;
            //         if (idx == midx || midx == 0) motheridx[idxmc] = idx - 1;
            //         else motheridx[idxmc] = midx - 1;
            //         idxmc++;
            //     }
            // }
            // else {
            //     for (; iter_mc != mcParticleCol->end(); iter_mc++) {
            //         if (!(*iter_mc)->decayFromGenerator()) continue;
            //         pdgid[idxmc] = (*iter_mc)->particleProperty();
            //         motheridx[idxmc]= ((*iter_mc)->mother()).trackIndex();
            //         idxmc++;
            //     }
            // }
            // if (m_debug) std::cout << " PDG.SIZE():  " << idxmc << std::endl;
            if (pid == 90022 || pid == 80022 || 9030443) {
                for (iter_mc++; iter_mc != mcParticleCol->end(); iter_mc++) {
                    if (!(*iter_mc)->decayFromGenerator()) continue;
                    int pdg = (*iter_mc)->particleProperty();
                    int motherpdg = ((*iter_mc)->mother()).particleProperty();
                    int idx = (*iter_mc)->trackIndex();
                    int midx  = ((*iter_mc)->mother()).trackIndex();
                    int mcidx;
                    if (idx == midx || midx == 0) mcidx = idx - 1;
                    else mcidx = midx -1;
                    pdgid[idxmc] = pdg;
                    trkidx[idxmc] = idx;
                    motherid[idxmc] = motherpdg;
                    motheridx[idxmc] = mcidx;
                    idxmc++;
                }
            }
        }
    }

    //use util tool to get dtaglist
    DTagTool dtagTool;
    if (dtagTool.isDTagListEmpty()) {
        return StatusCode::SUCCESS;
    }
    Ncut1++;
    DTagToolIterator iter_begin = dtagTool.modes_begin();
    DTagToolIterator iter_end = dtagTool.modes_end();
    HepLorentzVector ecms_lab = HepLorentzVector(sin(0.011)*m_ecms, 0. ,0., m_ecms);
    int ncharge = 0;
    double MassD0 = 1.86486;
    double MassDp = 1.86962;
    double MassPi0 = 0.13498;
    double CHISQ = 999.;
    HepLorentzVector besKpipp4 = 0;
    HepLorentzVector besKD0p4 = 0;
    HepLorentzVector besKDmp4 = 0;
    HepLorentzVector bestpip4 = 0;
    RecMdcKalTrack* bestpiTrkex = NULL;
    EvtRecDTagCol::iterator besttag1; 
    EvtRecDTagCol::iterator besttag2;
    
    //access all canddidates of one mode
    Hep3Vector xorigin(0, 0, 0);
    IVertexDbSvc* vtxsvc;
    Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
    if (vtxsvc->isVertexValid()) {
        double* dbv = vtxsvc->PrimaryVertex();
        xorigin.setX(dbv[0]);
        xorigin.setY(dbv[1]);
        xorigin.setZ(dbv[2]);
    }
    KalmanKinematicFit *kmfit = KalmanKinematicFit::instance();
    for (int modeA = 0; modeA != m_modes_D0.size(); modeA++ ) {
        for (int modeB = 0; modeB != m_modes_Dp.size(); modeB++) {
            Vint ipip, ipim;
            ipip.clear();
            ipim.clear();
            Vp4 ppip, ppim;
            ppip.clear();
            ppim.clear();
            bool dtagflag = false;
            dtagflag = dtagTool.findDDTag(m_modes_D0[modeA], m_modes_Dp[modeB], "inv");
            if (dtagflag == false) continue;
            EvtRecDTagCol::iterator tag1 = dtagTool.dtag1();
            EvtRecDTagCol::iterator tag2 = dtagTool.dtag2();
            Ncut2++;
            if (m_debug) {
                std::cout << "modeA = " << m_modes_D0[modeA] << " modeB= " << m_modes_Dp[modeB] << std::endl;
                std::cout << "Tag mode: " << (*tag1)->charm() << " " << (*tag1)->decayMode() << " " << (*tag2)->charm() << " " << (*tag2)->decayMode() << std::endl;
            }
            SmartRefVector<EvtRecTrack> Tracks1 = (*tag1)->tracks();
            SmartRefVector<EvtRecTrack> Tracks2 = (*tag2)->tracks();
            SmartRefVector<EvtRecTrack> Showers1 = (*tag1)->showers();
            SmartRefVector<EvtRecTrack> Showers2 = (*tag2)->showers();
            SmartRefVector<EvtRecTrack> OtherTracks1 = (*tag1)->otherTracks();
            SmartRefVector<EvtRecTrack> OtherTracks2 = (*tag2)->otherTracks();
            int pi_id = 0;
            for (int tk = 0; tk < OtherTracks1.size(); tk++) {
                int t = 0;
                for (int i = 0; i < Tracks2.size(); i++) {
                    if (OtherTracks1[tk]->trackId() == Tracks2[i]->trackId()) t = 1;
                }
                if (t == 1) continue;
                RecMdcKalTrack *mdcTrk = OtherTracks1[tk]->mdcKalTrack();
                if (dtagTool.isPion(OtherTracks1[tk]) && dtagTool.isGoodTrack(OtherTracks1[tk])) {
                    HepLorentzVector ptrk;
                    ptrk.setPx(mdcTrk->px());
                    ptrk.setPy(mdcTrk->py());
                    ptrk.setPz(mdcTrk->pz());
                    double p3 = ptrk.rho();
                    ptrk.setE(sqrt(p3*p3+mpi*mpi));
                    if (mdcTrk->charge() > 0) {
                        ppip.push_back(ptrk);
                        ipip.push_back(tk);
                        pi_id = tk;
                    }
                    if (mdcTrk->charge() < 0) {
                        ppim.push_back(ptrk);
                        ipim.push_back(tk);
                        pi_id = tk;
                    }
                }
            }
            bool isPipD0Dm = ppip.size() == 1 && ppim.size() == 0 && (*tag1)->charm() == 1 && (*tag2)->charm() == -1;
            bool isPimD0barDp = ppip.size() == 0 && ppim.size() == 1 && (*tag1)->charm() == -1 && (*tag2)->charm() == 1;
            if (m_debug) {
                std::cout << isPipD0Dm << " " << isPimD0barDp << std::endl;
            }
            if (!(isPipD0Dm || isPimD0barDp)) continue;
            VWTrkPara vwtrkpara_charge;
            vwtrkpara_charge.clear();
            for (int trk1 = 0; trk1 < Tracks1.size(); trk1++) {
                RecMdcKalTrack* KalTrk = Tracks1[trk1]->mdcKalTrack();
                if (dtagTool.isKaon(Tracks1[trk1])) {
                    KalTrk->setPidType(RecMdcKalTrack::kaon);
                    vwtrkpara_charge.push_back(WTrackParameter(mka, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                }
                else {
                    KalTrk->setPidType(RecMdcKalTrack::pion);
                    vwtrkpara_charge.push_back(WTrackParameter(mpi, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                }
            }
            for (int trk2 = 0; trk2 < Tracks2.size(); trk2++) {
                RecMdcKalTrack* KalTrk = Tracks2[trk2]->mdcKalTrack();
                if (dtagTool.isKaon(Tracks2[trk2])) {
                    KalTrk->setPidType(RecMdcKalTrack::kaon);
                    vwtrkpara_charge.push_back(WTrackParameter(mka, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                }
                else {
                    KalTrk->setPidType(RecMdcKalTrack::pion);
                    vwtrkpara_charge.push_back(WTrackParameter(mpi, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                }
            }
            RecMdcKalTrack *mdcPip = OtherTracks1[pi_id]->mdcKalTrack();
            mdcPip->setPidType(RecMdcKalTrack::pion);
            vwtrkpara_charge.push_back(WTrackParameter(mpi, mdcPip->getZHelixK(), mdcPip->getZErrorK()));
            chisq_vf = fitVertex(vwtrkpara_charge, birth);
            if (chisq_vf > 100) continue;
            Ncut8++;
            // Begain to fill the Kinematic Fitter
            kmfit->init();
            // Tracks of first D
            for (int i = 0; i < Tracks1.size(); i++) {
                RecMdcKalTrack *kapiTrk = Tracks1[i]->mdcKalTrack();
                WTrackParameter wvkapiTrk = WTrackParameter(mPart_D0[(*tag1)->decayMode()][i], kapiTrk->getZHelix(), kapiTrk->getZError());
                kmfit->AddTrack(i, wvkapiTrk);
            }
            // Showers of first D
            for (int i = 0; i < Showers1.size(); i++) {
                RecEmcShower* gamShower = Showers1[i]->emcShower();
                kmfit->AddTrack(i + Tracks1.size(), 0.0, gamShower);
            }
            // Tracks of secound D
            for (int i = 0; i < Tracks2.size(); i++) {
                RecMdcKalTrack *kapiTrk = Tracks2[i]->mdcKalTrack();
                WTrackParameter wvkapiTrk = WTrackParameter(mPart_Dp[(*tag2)->decayMode()-200][i], kapiTrk->getZHelix(), kapiTrk->getZError());
                kmfit->AddTrack(i + Tracks1.size() + Showers1.size(), wvkapiTrk);
            }
            // Showers of secound D
            for (int i = 0; i < Showers2.size(); i++) {
                RecEmcShower* gamShower = Showers2[i]->emcShower();
                kmfit->AddTrack(i + Tracks1.size() + Showers1.size() + Tracks2.size(), 0.0, gamShower);
            }
            // The pion out of D 
            RecMdcKalTrack* piex; 
            WTrackParameter wvpiexTrk;
            HepLorentzVector pipp4; 
            if ((*tag1)->charm() == 1) { 
                piex = OtherTracks1[ipip[0]]->mdcKalTrack();
                pipp4 = ppip[0]; 
                wvpiexTrk = WTrackParameter(mpi, piex->getZHelix(), piex->getZError());
            }
            else if ((*tag1)->charm() == -1) {
                piex = OtherTracks1[ipim[0]]->mdcKalTrack();
                pipp4 = ppim[0]; 
                wvpiexTrk = WTrackParameter(mpi, piex->getZHelix(), piex->getZError());
            }
            kmfit->AddTrack(Tracks1.size() + Showers1.size() + Tracks2.size() + Showers2.size(), wvpiexTrk);
            kmfit->AddFourMomentum(0, ecms_lab);
            bool oksq = kmfit->Fit();
            if (!oksq) continue;
            double chisq = kmfit->chisq();
            if (m_debug) {
                std::cout << "4C fit result: " << oksq << " " << kmfit->chisq() << std::endl;
            }
            //Get the P4 of two D and pion from Fitter
            HepLorentzVector kD0p4 = 0;
            for (int i = 0; i < Tracks1.size(); i++) {
                kD0p4 += kmfit->pfit(i);
            }
            for (int i = 0; i < Showers1.size(); i++) {
                kD0p4 += kmfit->pfit(i + Tracks1.size());
            }
            HepLorentzVector kDmp4 = 0;
            for (int i = 0; i < Tracks2.size(); i++) {
                kDmp4 += kmfit->pfit(i + Tracks1.size() + Showers1.size());
            }
            for (int i = 0; i < Showers2.size(); i++) {
                kDmp4 += kmfit->pfit(i + Tracks1.size() + Showers1.size() + Tracks2.size());
            }
            HepLorentzVector kpipp4 = kmfit->pfit(Tracks1.size() + Showers1.size() + Tracks2.size() + Showers2.size());
            // Select the best combination 
            if (chisq < CHISQ) {
                CHISQ = chisq;
                besKD0p4 = kD0p4;
                besKDmp4 = kDmp4;
                besKpipp4 = kpipp4;
                bestpip4 = pipp4;
                bestpiTrkex = piex;
                besttag1 = tag1;
                besttag2 = tag2;
                RecMdcKalTrack *mdcTrk = OtherTracks1[pi_id]->mdcKalTrack();
                HepVector a = mdcTrk->getZHelixK();
                HepSymMatrix Ea = mdcTrk->getZErrorK();
                HepPoint3D point0(0., 0., 0.);
                HepPoint3D IP(xorigin[0], xorigin[1], xorigin[2]);
                VFHelix helixip(point0, a, Ea);
                helixip.pivot(IP);
                HepVector vecipa = helixip.a();
                m_vtx_tagPi[0] = fabs(vecipa[0]);
                m_vtx_tagPi[1] = fabs(vecipa[3]);
                m_vtx_tagPi[2] = cos(mdcTrk->theta());
                SmartRefVector<EvtRecTrack> OtherShowers1 = (*tag1)->otherShowers();
                SmartRefVector<EvtRecTrack> OtherShowers2 = (*tag2)->otherShowers();
                SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), "/Event/EvtRec/EvtRecEvent");
                SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(),  EventModel::EvtRec::EvtRecTrackCol);
                if (!evtRecTrkCol) {
                    std::cout << MSG::FATAL << "Could not find EvtRecTrackCol" << std::endl;
                    return StatusCode::FAILURE;
                }
                m_n_othershws = 0;
                VWTrkPara vwtrkpara_othershws;
                vwtrkpara_othershws.clear();
                for (int shw = 0; shw < OtherShowers1.size(); shw++) {
                    int SHW = 1;
                    for (int sh = 0; sh < OtherShowers2.size(); sh++) {
                        if (OtherShowers1[shw]->trackId() == OtherShowers2[sh]->trackId()) SHW = 0;
                    }
                    if (SHW == 0) continue;
                    if (!(dtagTool.isGoodShower(OtherShowers1[shw]))) continue;
                    RecEmcShower *gTrk = OtherShowers1[shw]->emcShower();
                    Hep3Vector Gm_Vec(gTrk->x(), gTrk->y(), gTrk->z());
                    double dang = 200.;
                    for (int trk = 0; trk < evtRecEvent->totalCharged(); trk++) {
                        EvtRecTrackIterator jtTrk = evtRecTrkCol->begin() + trk;
                        if (!(*jtTrk)->isExtTrackValid()) continue;
                        RecExtTrack *extTrk = (*jtTrk)->extTrack();
                        if (extTrk->emcVolumeNumber() == -1) continue;
                        Hep3Vector extpos = extTrk->emcPosition();
                        double angd = extpos.angle(Gm_Vec);
                        if (angd < dang) {
                            dang = angd;
                        }
                    }
                    if (dang >= 200) continue;
                    double eraw = gTrk->energy();
                    double phi = gTrk->phi();
                    double the = gTrk->theta();
                    dang = dang * 180 / (CLHEP::pi);
                    int module = gTrk->module();
                    double Tdc = gTrk->time();
                    m_vtx_othershws[m_n_othershws][0] = eraw;
                    m_vtx_othershws[m_n_othershws][1] = phi;
                    m_vtx_othershws[m_n_othershws][2] = the;
                    m_vtx_othershws[m_n_othershws][3] = dang;
                    m_vtx_othershws[m_n_othershws][4] = module;
                    m_vtx_othershws[m_n_othershws][5] = Tdc;
                    Hep3Vector Gm_Mom = Gm_Vec - birth.vx();
                    Gm_Mom.setMag(gTrk->energy());
                    HepLorentzVector Gm_p4(Gm_Mom, gTrk->energy());
                    for (int m = 0; m < 4; m++) m_rawp4_othershws[m_n_othershws][m] = Gm_p4[m];
                    m_n_othershws++;
                    if (m_n_othershws >= 50) continue;
                    vwtrkpara_othershws.push_back(WTrackParameter(gTrk->position(), Gm_p4, gTrk->dphi(), gTrk->dtheta(), gTrk->dE()));
                }
                stat_fitpi0 = false;
                stat_fitpi0 = fitpi0(vwtrkpara_othershws, birth);
                if (!stat_fitpi0 && m_debug) std::cout << "Cannot find enough gamma to reconstruct pi0!" << std::endl;
            }
        } //***** mode B
    } //***** mode A
    if (CHISQ > 200) return StatusCode::SUCCESS;
    Ncut3++;
    m_D0_mode          = (*besttag1)->decayMode();
    m_D0_type          = (*besttag1)->type();
    m_D0_dcharge       = (*besttag1)->charge();
    m_D0_charm         = (*besttag1)->charm();
    m_D0_numofchildren = (*besttag1)->numOfChildren();
    m_D0_be            = (*besttag1)->beamE();
    m_D0_mass          = (*besttag1)->mass();
    m_D0_mBC           = (*besttag1)->mBC();
    m_D0_deltae        = (*besttag1)->deltaE();
    m_Dm_mode          = (*besttag2)->decayMode();
    m_Dm_type          = (*besttag2)->type();
    m_Dm_dcharge       = (*besttag2)->charge();
    m_Dm_charm         = (*besttag2)->charm();
    m_Dm_numofchildren = (*besttag2)->numOfChildren();
    m_Dm_be            = (*besttag2)->beamE();
    m_Dm_mass          = (*besttag2)->mass();
    m_Dm_mBC           = (*besttag2)->mBC();
    m_Dm_deltae        = (*besttag2)->deltaE();
    m_chisq_4c = CHISQ;
	m_p4_D0_4c[0] = besKD0p4.px();
    m_p4_D0_4c[1] = besKD0p4.py();
    m_p4_D0_4c[2] = besKD0p4.pz();
    m_p4_D0_4c[3] = besKD0p4.e();
    m_p4_Dm_4c[0] = besKDmp4.px();
    m_p4_Dm_4c[1] = besKDmp4.py();
    m_p4_Dm_4c[2] = besKDmp4.pz();
    m_p4_Dm_4c[3] = besKDmp4.e();
    m_p4_pip_4c[0] = besKpipp4.px();
    m_p4_pip_4c[1] = besKpipp4.py();
    m_p4_pip_4c[2] = besKpipp4.pz();
    m_p4_pip_4c[3] = besKpipp4.e();
    Ncut4++;

    SmartRefVector<EvtRecTrack> tracks1 = (*besttag1)->tracks();
    SmartRefVector<EvtRecTrack> tracks2 = (*besttag2)->tracks();
    SmartRefVector<EvtRecTrack> showers1 = (*besttag1)->showers();
    SmartRefVector<EvtRecTrack> showers2 = (*besttag2)->showers();
    // Begain to fill the Kinematic Fitter
    kmfit->init();
    // Tracks of first D
    for (int i = 0; i < tracks1.size(); i++) {
        RecMdcKalTrack *kapiTrk = tracks1[i]->mdcKalTrack();
        WTrackParameter wvkapiTrk = WTrackParameter(mPart_D0[(*besttag1)->decayMode()][i], kapiTrk->getZHelix(), kapiTrk->getZError());
        kmfit->AddTrack(i, wvkapiTrk);
    }
    // Showers of first D
    for (int i = 0; i < showers1.size(); i++) {
        RecEmcShower* gamShower = showers1[i]->emcShower();
        kmfit->AddTrack(i + tracks1.size(), 0.0, gamShower);
    }
    // Tracks of secound D
    for (int i = 0; i < tracks2.size(); i++) {
        RecMdcKalTrack *kapiTrk = tracks2[i]->mdcKalTrack();
        WTrackParameter wvkapiTrk = WTrackParameter(mPart_Dp[(*besttag2)->decayMode() - 200][i], kapiTrk->getZHelix(), kapiTrk->getZError());
        kmfit->AddTrack(i + tracks1.size() + showers1.size(), wvkapiTrk);
    }
    // Showers of secound D
    for (int i = 0; i < showers2.size(); i++) {
        RecEmcShower* gamShower = showers2[i]->emcShower();
        kmfit->AddTrack(i + tracks1.size() + showers1.size() + tracks2.size(), 0.0, gamShower);
    }
    // The pion out of D 
    WTrackParameter wvpiexTrk = WTrackParameter(mpi, bestpiTrkex->getZHelix(), bestpiTrkex->getZError());
    kmfit->AddTrack(tracks1.size() + showers1.size() + tracks2.size() + showers2.size(), wvpiexTrk);
    if (m_debug) {
        std::cout << "Tag1: " << (*besttag1)->decayMode() << " Track size: " << tracks1.size() << " shower size: " << showers1.size() << std::endl;
        std::cout << "Tag2: " << (*besttag2)->decayMode() << " Track size: " << tracks2.size() << " shower size: " << showers2.size() << std::endl;
    }
    // Add the mass constrain of D0 and D-
    if ((*besttag1)->decayMode() == 0)      kmfit->AddResonance(0, MassD0, 0, 1);
    else if ((*besttag1)->decayMode() == 1) kmfit->AddResonance(0, MassD0, 0, 1, 2, 3);
    else if ((*besttag1)->decayMode() == 3) kmfit->AddResonance(0, MassD0, 0, 1, 2, 3);
    else if ((*besttag1)->decayMode() == 4) kmfit->AddResonance(0, MassD0, 0, 1, 2, 3, 4, 5);
    else std::cout << "Attention! You need add more mode!" << std::endl;
    int nTr = tracks1.size() + showers1.size();
    if ((*besttag2)->decayMode() == 200)      kmfit->AddResonance(1, MassDp, nTr + 0, nTr + 1, nTr + 2);
    else if ((*besttag2)->decayMode() == 201) kmfit->AddResonance(1, MassDp, nTr + 0, nTr + 1, nTr + 2, nTr + 3, nTr + 4);
    else if ((*besttag2)->decayMode() == 202) kmfit->AddResonance(1, MassDp, nTr + 0, nTr + 1, nTr + 2);
    else if ((*besttag2)->decayMode() == 203) kmfit->AddResonance(1, MassDp, nTr + 0, nTr + 1, nTr + 2, nTr + 3, nTr + 4);
    else if ((*besttag2)->decayMode() == 204) kmfit->AddResonance(1, MassDp, nTr + 0, nTr + 1, nTr + 2, nTr + 3, nTr + 4);
    else std::cout << "Attention! You need add more mode!" << std::endl;
    kmfit->AddFourMomentum(2, ecms_lab);
    bool oksq = kmfit->Fit();
    double chisq = kmfit->chisq();
    if (m_debug) {
        std::cout << "6C fit result: " << oksq << " " << kmfit->chisq() << std::endl;
    }
    //Get the P4 of two D and pion from Fitter
    HepLorentzVector kD0p4 = 0;
    for (int i = 0; i < tracks1.size(); i++) {
        kD0p4 += kmfit->pfit(i);
    }
    for (int i = 0; i < showers1.size(); i++) {
        kD0p4 += kmfit->pfit(i + tracks1.size());
    }
    HepLorentzVector kDmp4 = 0;
    for (int i = 0; i < tracks2.size(); i++) {
        kDmp4 += kmfit->pfit(i + tracks1.size() + showers1.size());
    }
    for (int i = 0; i < showers2.size(); i++) {
        kDmp4 += kmfit->pfit(i + tracks1.size() + showers1.size() + tracks2.size());
    }
    HepLorentzVector kpipp4 = kmfit->pfit(tracks1.size() + showers1.size() + tracks2.size() + showers2.size());
    m_chisq_6c = chisq;
	m_p4_D0_6c[0] = kD0p4.px();
    m_p4_D0_6c[1] = kD0p4.py();
    m_p4_D0_6c[2] = kD0p4.pz();
    m_p4_D0_6c[3] = kD0p4.e();
    m_p4_Dm_6c[0] = kDmp4.px();
    m_p4_Dm_6c[1] = kDmp4.py();
    m_p4_Dm_6c[2] = kDmp4.pz();
    m_p4_Dm_6c[3] = kDmp4.e();
    m_p4_pip_6c[0] = kpipp4.px();
    m_p4_pip_6c[1] = kpipp4.py();
    m_p4_pip_6c[2] = kpipp4.pz();
    m_p4_pip_6c[3] = kpipp4.e();
    Ncut41++;
    recordVariables();
    dtagTool.clear();
} //end of execute()

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode PiD0Dm::finalize() {
    std::cout << "total number:         " << Ncut0  << std::endl;
    std::cout << "DTagList is not Empty " << Ncut1  << std::endl;
    std::cout << "Best tagD             " << Ncut2  << std::endl;
    std::cout << "Best tagDandpipi      " << Ncut8  << std::endl;
    std::cout << "Good pipi             " << Ncut3  << std::endl;
    std::cout << "Pass kfit             " << Ncut4  << std::endl;
    std::cout << "Pass 6C kfit          " << Ncut41 << std::endl;
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "in finalize()" << endmsg;
    return StatusCode::SUCCESS;
}

void PiD0Dm::recordVariables() {
    m_runNo = runNo;
    m_evtNo = evtNo;
    m_flag1 = flag1;
    m_flag2 = flag2;

    // save all McTruth info
    if (m_runNo < 0) {
        m_idxmc = idxmc;
        for (int i = 0; i< m_idxmc; i++) {
            m_motherid[i] = motherid[i];
            m_motheridx[i] = motheridx[i];
            m_pdgid[i] = pdgid[i];
            m_trkidx[i] = trkidx[i];
        }
    }
    m_chisq_vf = chisq_vf;

    m_tuple1->write();

}

double PiD0Dm::fitVertex(VWTrkPara &vwtrkpara, VertexParameter &birth) {
    VertexParameter vxpar;
    Hep3Vector xorigin(0, 0, 0);
    HepSymMatrix Exorigin(3, 0); // error matrix
    double bx = 1E+6, by = 1E+6, bz = 1E+6;
    Exorigin[0][0] = bx*bx; Exorigin[1][1] = by*by; Exorigin[2][2] = bz*bz;
    vxpar.setVx(xorigin); vxpar.setEvx(Exorigin); // vx: vertex
    vtxfit->init();
    for (int i = 0; i < vwtrkpara.size(); i++) vtxfit->AddTrack(i, vwtrkpara[i]);
    Vint trkId(vwtrkpara.size(), 0); // ????????????????
    for (int i = 0; i < vwtrkpara.size(); i++) trkId[i] = i;
    vtxfit->AddVertex(0, vxpar, trkId);
    if (!vtxfit->Fit(0)) return 9999; // 0: which vertex to fit
    double vf_chi2 = vtxfit->chisq(0);
    vtxfit->Swim(0); // adjust momentum according to errorr matrix
    for (int i = 0; i < vwtrkpara.size(); i++) vwtrkpara[i] = vtxfit->wtrk(i);
    if (vf_chi2 == 1) std::cout << "==========  VF chi2: " << vf_chi2 << std::endl;
    birth = vtxfit->vpar(0);
    return vf_chi2;
}

bool PiD0Dm::fitpi0(VWTrkPara &vwtrkpara_photons, VertexParameter &birth) {
    KalmanKinematicFit *kmfit = KalmanKinematicFit::instance();
    m_n_pi0 = 0;
    if (vwtrkpara_photons.size() <= 1) return false;
    for (int i = 0; i < vwtrkpara_photons.size() - 1; i++) {
        for (int j = i + 1; j < vwtrkpara_photons.size(); j++) {
            kmfit->init();
            kmfit->AddTrack(0, vwtrkpara_photons[i]);
            kmfit->AddTrack(1, vwtrkpara_photons[j]);
            kmfit->AddResonance(0, mpi0, 0, 1);
            bool oksq = kmfit->Fit();
            if (oksq) {
                double chi2 = kmfit->chisq();
                m_chi2_pi0[m_n_pi0] = chi2;
                HepLorentzVector ppi0 = kmfit->pfit(0) + kmfit->pfit(1);
                for (int k = 0; k < 4; k++) m_p4_pi0[m_n_pi0][k] = ppi0[k];
                m_n_pi0++;
            }
            else {
                continue;
            }
        }
    }
    return true;
}
