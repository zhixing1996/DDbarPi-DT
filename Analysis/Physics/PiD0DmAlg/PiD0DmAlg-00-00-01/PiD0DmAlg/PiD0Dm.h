#ifndef Physics_Analysis_PiD0Dm_H
#define Physics_Analysis_PiD0Dm_H 


#include "GaudiKernel/Algorithm.h"
// #include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/DeclareFactoryEntries.h"
#include "GaudiKernel/LoadFactoryEntries.h"
#include "GaudiKernel/INTupleSvc.h"
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/PropertyMgr.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/MsgStream.h"
#include "DatabaseSvc/IDatabaseSvc.h"

#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "MeasuredEcmsSvc/IMeasuredEcmsSvc.h"

#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IHistogramSvc.h"
#include "EventModel/Event.h"
/////////////////////////////

#include "EventModel/EventModel.h"
#include "EventModel/EventHeader.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecDTag.h"
#include "EvtRecEvent/EvtRecPi0.h"
#include "EvtRecEvent/EvtRecVeeVertex.h"
#include "EvtRecEvent/EvtRecPi0.h"
#include "MdcRecEvent/RecMdcKalTrack.h"
#include "DTagTool/DTagTool.h"

#include "VertexFit/KalmanKinematicFit.h"
#include "VertexFit/KinematicFit.h"
#include "VertexFit/VertexFit.h"
#include "VertexFit/IVertexDbSvc.h"
#include "VertexFit/SecondVertexFit.h"
#include "VertexFit/Helix.h"
#include "VertexFit/WTrackParameter.h"
#include "ParticleID/ParticleID.h"

#include "McTruth/McParticle.h"

#include "DstEvent/TofHitStatus.h"

#include "TMath.h"
#include "TRandom.h"
#include "TLorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/TwoVector.h"
#include "CLHEP/Geometry/Point3D.h"
#include <vector>
#include <TTree.h>

using CLHEP::Hep3Vector;
using CLHEP::Hep2Vector;
using CLHEP::HepLorentzVector;
#ifndef ENABLE_BACKWARDS_COMPATIBILITY
typedef HepGeom::Point3D<double> HepPoint3D;
#endif

const double velc = 299.792458;   // tof path unit in mm
typedef std::vector<int> Vint;
typedef std::vector<double> Vdouble;
typedef std::vector<RecMdcKalTrack*> VRecTrack;
typedef std::vector<RecEmcShower*> VRecShower;
typedef std::vector<WTrackParameter> VWTrack;
typedef std::vector<HepLorentzVector> Vp4;
typedef std::vector<WTrackParameter> VWTrkPara;
int Ncut0,Ncut1,Ncut2,Ncut3,Ncut4,Ncut5,Ncut6,Ncut7,Ncut8,Ncut9,Ncut10,Ncut11,Ncut12,Ncut13,Ncut14,Ncut15,Ncut16,Ncut17,Ncut41;

class PiD0Dm : public Algorithm {
    public:
        PiD0Dm(const std::string& name, ISvcLocator* pSvcLocator);
        StatusCode initialize();
        StatusCode execute();
        StatusCode finalize();  

    private:
        // properties
        std::vector<int> m_modes_D0;
        std::vector<int> m_modes_Dp;
        double m_ecms;
        bool m_debug;
        int runNo;
        int evtNo;
        int flag1;
        int flag2;
        VertexParameter birth;
        double chisq_vf;
        bool stat_fitpi0;
        
        // All McTruth info
        int pdgid[100];
        int trkidx[100];
        int motherid[100];
        int motheridx[100];
        int idxmc;

        // define Ntuples here
        NTuple::Tuple*  m_tuple1;    //tagD
        NTuple::Item<int>       m_runNo;
        NTuple::Item<int>       m_evtNo;
        NTuple::Item<int>       m_flag1;
        NTuple::Item<int>       m_flag2;
        NTuple::Item<int>       m_idxmc;
        NTuple::Array<int>      m_pdgid;
        NTuple::Array<int>      m_trkidx;
        NTuple::Array<int>      m_motherid;
        NTuple::Array<int>      m_motheridx;
        NTuple::Item<double>    m_D0_mode;
        NTuple::Item<double>    m_D0_type;
        NTuple::Item<int>       m_D0_dcharge;
        NTuple::Item<double>    m_D0_charm;
        NTuple::Item<double>    m_D0_mBC;
        NTuple::Item<double>    m_D0_numofchildren;
        NTuple::Item<double>    m_D0_mass;
        NTuple::Item<double>    m_D0_be;
        NTuple::Item<double>    m_D0_deltae;
        NTuple::Item<double>    m_Dm_mode;
        NTuple::Item<double>    m_Dm_type;
        NTuple::Item<int>       m_Dm_dcharge;
        NTuple::Item<double>    m_Dm_charm;
        NTuple::Item<double>    m_Dm_mBC;
        NTuple::Item<double>    m_Dm_numofchildren;
        NTuple::Item<double>    m_Dm_mass;
        NTuple::Item<double>    m_Dm_be;
        NTuple::Item<double>    m_Dm_deltae;
        NTuple::Item<double>    m_chisq_4c;
        NTuple::Array<double>   m_p4_D0_4c;
        NTuple::Array<double>   m_p4_Dm_4c;
        NTuple::Array<double>   m_p4_pip_4c;
        NTuple::Array<double>   m_vtx_tagPi;
        NTuple::Item<double>    m_chisq_6c;
        NTuple::Array<double>   m_p4_D0_6c;
        NTuple::Array<double>   m_p4_Dm_6c;
        NTuple::Array<double>   m_p4_pip_6c;
        NTuple::Item<double>    m_chisq_vf;
        NTuple::Item<int>      m_n_othershws;
        NTuple::Matrix<double>  m_vtx_othershws;
        NTuple::Matrix<double>  m_rawp4_othershws;
        NTuple::Item<int> m_n_pi0;
        NTuple::Array<double> m_chi2_pi0;
        NTuple::Matrix<double> m_p4_pi0;
        
        void recordVariables();
        double fitVertex(VWTrkPara &vwtrkpara, VertexParameter &birth_photon);
        bool fitpi0(VWTrkPara &vwtrkpara_photons, VertexParameter &birth);
};

const double mpi = 0.13957;
const double mka = 0.493677;
const double mpi0 = 0.1349766;
const double mPart_D0[5][4] = {
	 {mka, mpi},
	 {mka, mpi},
	 {mka, mpi},
	 {mka, mpi, mpi, mpi},
	 {mka, mpi, mpi, mpi}
};

const double mPart_Dp[5][5] = {
	 {mka, mpi, mpi},
	 {mka, mpi, mpi},
	 {mpi, mpi, mpi},
	 {mpi, mpi, mpi},
	 {mpi, mpi, mpi, mpi, mpi}
};
#endif 
