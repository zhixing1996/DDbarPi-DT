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
        std::vector<int> m_modes_Dm;
        std::vector<string> m_sample_types;
        double m_ecms;
        bool m_debug;
        VertexParameter birth;
        bool stat_fitpi0;
        double m_W_D0;
        double m_W_Dm;
        bool m_pid;
        
        int runNo;
        int evtNo;
        int flag1;
        int flag2;
        // All McTruth info
        int pdgid[100];
        int trkidx[100];
        int motherid[100];
        int motheridx[100];
        int idxmc;
        double chisq_vf;
        double vtx_tagPi[3];
        double rawp4_tagPi[7];
        int n_othershws;
        double vtx_othershws[100][6];
        double rawp4_othershws[100][4];
        int n_pi0;
        double chi2_pi0[100];
        double p4_pi0[100][4];

        // define Ntuples here
        NTuple::Tuple*          m_tuple_signal;
        NTuple::Item<int>       m_runNo_signal;
        NTuple::Item<int>       m_evtNo_signal;
        NTuple::Item<int>       m_flag1_signal;
        NTuple::Item<int>       m_flag2_signal;
        NTuple::Item<int>       m_idxmc_signal;
        NTuple::Array<int>      m_pdgid_signal;
        NTuple::Array<int>      m_trkidx_signal;
        NTuple::Array<int>      m_motherid_signal;
        NTuple::Array<int>      m_motheridx_signal;
        NTuple::Item<double>    m_D0_mode_signal;
        NTuple::Item<double>    m_D0_type_signal;
        NTuple::Item<int>       m_D0_dcharge_signal;
        NTuple::Item<double>    m_D0_charm_signal;
        NTuple::Item<double>    m_D0_mBC_signal;
        NTuple::Item<double>    m_D0_numofchildren_signal;
        NTuple::Item<double>    m_D0_mass_signal;
        NTuple::Item<double>    m_D0_be_signal;
        NTuple::Item<double>    m_D0_deltae_signal;
        NTuple::Item<double>    m_Dm_mode_signal;
        NTuple::Item<double>    m_Dm_type_signal;
        NTuple::Item<int>       m_Dm_dcharge_signal;
        NTuple::Item<double>    m_Dm_charm_signal;
        NTuple::Item<double>    m_Dm_mBC_signal;
        NTuple::Item<double>    m_Dm_numofchildren_signal;
        NTuple::Item<double>    m_Dm_mass_signal;
        NTuple::Item<double>    m_Dm_be_signal;
        NTuple::Item<double>    m_Dm_deltae_signal;
        NTuple::Item<double>    m_chisq_4c_signal;
        NTuple::Array<double>   m_p4_D0_4c_signal;
        NTuple::Array<double>   m_p4_Dm_4c_signal;
        NTuple::Array<double>   m_p4_pip_4c_signal;
        NTuple::Array<double>   m_vtx_tagPi_signal;
        NTuple::Array<double>   m_rawp4_tagPi_signal;
        NTuple::Item<double>    m_chisq_vf_signal;
        NTuple::Item<int>       m_n_othershws_signal;
        NTuple::Matrix<double>  m_vtx_othershws_signal;
        NTuple::Matrix<double>  m_rawp4_othershws_signal;
        NTuple::Item<int>       m_n_pi0_signal;
        NTuple::Array<double>   m_chi2_pi0_signal;
        NTuple::Matrix<double>  m_p4_pi0_signal;
        NTuple::Item<int>       m_n_Dmtrks_signal;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_signal;
        NTuple::Matrix<double>  m_vtx_Dmtrks_signal;
        NTuple::Matrix<double>  m_vtx_Dmshws_signal;
        NTuple::Item<int>       m_n_D0trks_signal;
        NTuple::Matrix<double>  m_rawp4_D0trks_signal;
        NTuple::Matrix<double>  m_vtx_D0trks_signal;
        NTuple::Matrix<double>  m_vtx_D0shws_signal;
        NTuple::Item<int>       m_n_Dmshws_signal;
        NTuple::Matrix<double>  m_rawp4_Dmshws_signal;
        NTuple::Item<int>       m_n_D0shws_signal;
        NTuple::Matrix<double>  m_rawp4_D0shws_signal;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_signal;
        NTuple::Matrix<double>  m_p4_D0trks_4c_signal;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_signal;
        NTuple::Matrix<double>  m_p4_D0shws_4c_signal;
        NTuple::Item<double>    m_chisq_6c_signal;
        NTuple::Array<double>   m_p4_D0_6c_signal;
        NTuple::Array<double>   m_p4_Dm_6c_signal;
        NTuple::Array<double>   m_p4_pip_6c_signal;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_signal;
        NTuple::Matrix<double>  m_p4_D0trks_6c_signal;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_signal;
        NTuple::Matrix<double>  m_p4_D0shws_6c_signal;
        
        NTuple::Tuple*          m_tuple_side1_low;
        NTuple::Item<int>       m_runNo_side1_low;
        NTuple::Item<int>       m_evtNo_side1_low;
        NTuple::Item<int>       m_flag1_side1_low;
        NTuple::Item<int>       m_flag2_side1_low;
        NTuple::Item<int>       m_idxmc_side1_low;
        NTuple::Array<int>      m_pdgid_side1_low;
        NTuple::Array<int>      m_trkidx_side1_low;
        NTuple::Array<int>      m_motherid_side1_low;
        NTuple::Array<int>      m_motheridx_side1_low;
        NTuple::Item<double>    m_D0_mode_side1_low;
        NTuple::Item<double>    m_D0_type_side1_low;
        NTuple::Item<int>       m_D0_dcharge_side1_low;
        NTuple::Item<double>    m_D0_charm_side1_low;
        NTuple::Item<double>    m_D0_mBC_side1_low;
        NTuple::Item<double>    m_D0_numofchildren_side1_low;
        NTuple::Item<double>    m_D0_mass_side1_low;
        NTuple::Item<double>    m_D0_be_side1_low;
        NTuple::Item<double>    m_D0_deltae_side1_low;
        NTuple::Item<double>    m_Dm_mode_side1_low;
        NTuple::Item<double>    m_Dm_type_side1_low;
        NTuple::Item<int>       m_Dm_dcharge_side1_low;
        NTuple::Item<double>    m_Dm_charm_side1_low;
        NTuple::Item<double>    m_Dm_mBC_side1_low;
        NTuple::Item<double>    m_Dm_numofchildren_side1_low;
        NTuple::Item<double>    m_Dm_mass_side1_low;
        NTuple::Item<double>    m_Dm_be_side1_low;
        NTuple::Item<double>    m_Dm_deltae_side1_low;
        NTuple::Item<double>    m_chisq_4c_side1_low;
        NTuple::Array<double>   m_p4_D0_4c_side1_low;
        NTuple::Array<double>   m_p4_Dm_4c_side1_low;
        NTuple::Array<double>   m_p4_pip_4c_side1_low;
        NTuple::Array<double>   m_vtx_tagPi_side1_low;
        NTuple::Array<double>   m_rawp4_tagPi_side1_low;
        NTuple::Item<double>    m_chisq_vf_side1_low;
        NTuple::Item<int>       m_n_othershws_side1_low;
        NTuple::Matrix<double>  m_vtx_othershws_side1_low;
        NTuple::Matrix<double>  m_rawp4_othershws_side1_low;
        NTuple::Item<int>       m_n_pi0_side1_low;
        NTuple::Array<double>   m_chi2_pi0_side1_low;
        NTuple::Matrix<double>  m_p4_pi0_side1_low;
        NTuple::Item<int>       m_n_Dmtrks_side1_low;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_side1_low;
        NTuple::Matrix<double>  m_vtx_Dmtrks_side1_low;
        NTuple::Matrix<double>  m_vtx_Dmshws_side1_low;
        NTuple::Item<int>       m_n_D0trks_side1_low;
        NTuple::Matrix<double>  m_rawp4_D0trks_side1_low;
        NTuple::Matrix<double>  m_vtx_D0trks_side1_low;
        NTuple::Matrix<double>  m_vtx_D0shws_side1_low;
        NTuple::Item<int>       m_n_Dmshws_side1_low;
        NTuple::Matrix<double>  m_rawp4_Dmshws_side1_low;
        NTuple::Item<int>       m_n_D0shws_side1_low;
        NTuple::Matrix<double>  m_rawp4_D0shws_side1_low;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_side1_low;
        NTuple::Matrix<double>  m_p4_D0trks_4c_side1_low;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_side1_low;
        NTuple::Matrix<double>  m_p4_D0shws_4c_side1_low;
        NTuple::Item<double>    m_chisq_6c_side1_low;
        NTuple::Array<double>   m_p4_D0_6c_side1_low;
        NTuple::Array<double>   m_p4_Dm_6c_side1_low;
        NTuple::Array<double>   m_p4_pip_6c_side1_low;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_side1_low;
        NTuple::Matrix<double>  m_p4_D0trks_6c_side1_low;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_side1_low;
        NTuple::Matrix<double>  m_p4_D0shws_6c_side1_low;

        NTuple::Tuple*          m_tuple_side1_up;
        NTuple::Item<int>       m_runNo_side1_up;
        NTuple::Item<int>       m_evtNo_side1_up;
        NTuple::Item<int>       m_flag1_side1_up;
        NTuple::Item<int>       m_flag2_side1_up;
        NTuple::Item<int>       m_idxmc_side1_up;
        NTuple::Array<int>      m_pdgid_side1_up;
        NTuple::Array<int>      m_trkidx_side1_up;
        NTuple::Array<int>      m_motherid_side1_up;
        NTuple::Array<int>      m_motheridx_side1_up;
        NTuple::Item<double>    m_D0_mode_side1_up;
        NTuple::Item<double>    m_D0_type_side1_up;
        NTuple::Item<int>       m_D0_dcharge_side1_up;
        NTuple::Item<double>    m_D0_charm_side1_up;
        NTuple::Item<double>    m_D0_mBC_side1_up;
        NTuple::Item<double>    m_D0_numofchildren_side1_up;
        NTuple::Item<double>    m_D0_mass_side1_up;
        NTuple::Item<double>    m_D0_be_side1_up;
        NTuple::Item<double>    m_D0_deltae_side1_up;
        NTuple::Item<double>    m_Dm_mode_side1_up;
        NTuple::Item<double>    m_Dm_type_side1_up;
        NTuple::Item<int>       m_Dm_dcharge_side1_up;
        NTuple::Item<double>    m_Dm_charm_side1_up;
        NTuple::Item<double>    m_Dm_mBC_side1_up;
        NTuple::Item<double>    m_Dm_numofchildren_side1_up;
        NTuple::Item<double>    m_Dm_mass_side1_up;
        NTuple::Item<double>    m_Dm_be_side1_up;
        NTuple::Item<double>    m_Dm_deltae_side1_up;
        NTuple::Item<double>    m_chisq_4c_side1_up;
        NTuple::Array<double>   m_p4_D0_4c_side1_up;
        NTuple::Array<double>   m_p4_Dm_4c_side1_up;
        NTuple::Array<double>   m_p4_pip_4c_side1_up;
        NTuple::Array<double>   m_vtx_tagPi_side1_up;
        NTuple::Array<double>   m_rawp4_tagPi_side1_up;
        NTuple::Item<double>    m_chisq_vf_side1_up;
        NTuple::Item<int>       m_n_othershws_side1_up;
        NTuple::Matrix<double>  m_vtx_othershws_side1_up;
        NTuple::Matrix<double>  m_rawp4_othershws_side1_up;
        NTuple::Item<int>       m_n_pi0_side1_up;
        NTuple::Array<double>   m_chi2_pi0_side1_up;
        NTuple::Matrix<double>  m_p4_pi0_side1_up;
        NTuple::Item<int>       m_n_Dmtrks_side1_up;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_side1_up;
        NTuple::Matrix<double>  m_vtx_Dmtrks_side1_up;
        NTuple::Matrix<double>  m_vtx_Dmshws_side1_up;
        NTuple::Item<int>       m_n_D0trks_side1_up;
        NTuple::Matrix<double>  m_rawp4_D0trks_side1_up;
        NTuple::Matrix<double>  m_vtx_D0trks_side1_up;
        NTuple::Matrix<double>  m_vtx_D0shws_side1_up;
        NTuple::Item<int>       m_n_Dmshws_side1_up;
        NTuple::Matrix<double>  m_rawp4_Dmshws_side1_up;
        NTuple::Item<int>       m_n_D0shws_side1_up;
        NTuple::Matrix<double>  m_rawp4_D0shws_side1_up;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_side1_up;
        NTuple::Matrix<double>  m_p4_D0trks_4c_side1_up;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_side1_up;
        NTuple::Matrix<double>  m_p4_D0shws_4c_side1_up;
        NTuple::Item<double>    m_chisq_6c_side1_up;
        NTuple::Array<double>   m_p4_D0_6c_side1_up;
        NTuple::Array<double>   m_p4_Dm_6c_side1_up;
        NTuple::Array<double>   m_p4_pip_6c_side1_up;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_side1_up;
        NTuple::Matrix<double>  m_p4_D0trks_6c_side1_up;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_side1_up;
        NTuple::Matrix<double>  m_p4_D0shws_6c_side1_up;

        NTuple::Tuple*          m_tuple_side2_low;
        NTuple::Item<int>       m_runNo_side2_low;
        NTuple::Item<int>       m_evtNo_side2_low;
        NTuple::Item<int>       m_flag1_side2_low;
        NTuple::Item<int>       m_flag2_side2_low;
        NTuple::Item<int>       m_idxmc_side2_low;
        NTuple::Array<int>      m_pdgid_side2_low;
        NTuple::Array<int>      m_trkidx_side2_low;
        NTuple::Array<int>      m_motherid_side2_low;
        NTuple::Array<int>      m_motheridx_side2_low;
        NTuple::Item<double>    m_D0_mode_side2_low;
        NTuple::Item<double>    m_D0_type_side2_low;
        NTuple::Item<int>       m_D0_dcharge_side2_low;
        NTuple::Item<double>    m_D0_charm_side2_low;
        NTuple::Item<double>    m_D0_mBC_side2_low;
        NTuple::Item<double>    m_D0_numofchildren_side2_low;
        NTuple::Item<double>    m_D0_mass_side2_low;
        NTuple::Item<double>    m_D0_be_side2_low;
        NTuple::Item<double>    m_D0_deltae_side2_low;
        NTuple::Item<double>    m_Dm_mode_side2_low;
        NTuple::Item<double>    m_Dm_type_side2_low;
        NTuple::Item<int>       m_Dm_dcharge_side2_low;
        NTuple::Item<double>    m_Dm_charm_side2_low;
        NTuple::Item<double>    m_Dm_mBC_side2_low;
        NTuple::Item<double>    m_Dm_numofchildren_side2_low;
        NTuple::Item<double>    m_Dm_mass_side2_low;
        NTuple::Item<double>    m_Dm_be_side2_low;
        NTuple::Item<double>    m_Dm_deltae_side2_low;
        NTuple::Item<double>    m_chisq_4c_side2_low;
        NTuple::Array<double>   m_p4_D0_4c_side2_low;
        NTuple::Array<double>   m_p4_Dm_4c_side2_low;
        NTuple::Array<double>   m_p4_pip_4c_side2_low;
        NTuple::Array<double>   m_vtx_tagPi_side2_low;
        NTuple::Array<double>   m_rawp4_tagPi_side2_low;
        NTuple::Item<double>    m_chisq_vf_side2_low;
        NTuple::Item<int>       m_n_othershws_side2_low;
        NTuple::Matrix<double>  m_vtx_othershws_side2_low;
        NTuple::Matrix<double>  m_rawp4_othershws_side2_low;
        NTuple::Item<int>       m_n_pi0_side2_low;
        NTuple::Array<double>   m_chi2_pi0_side2_low;
        NTuple::Matrix<double>  m_p4_pi0_side2_low;
        NTuple::Item<int>       m_n_Dmtrks_side2_low;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_side2_low;
        NTuple::Matrix<double>  m_vtx_Dmtrks_side2_low;
        NTuple::Matrix<double>  m_vtx_Dmshws_side2_low;
        NTuple::Item<int>       m_n_D0trks_side2_low;
        NTuple::Matrix<double>  m_rawp4_D0trks_side2_low;
        NTuple::Matrix<double>  m_vtx_D0trks_side2_low;
        NTuple::Matrix<double>  m_vtx_D0shws_side2_low;
        NTuple::Item<int>       m_n_Dmshws_side2_low;
        NTuple::Matrix<double>  m_rawp4_Dmshws_side2_low;
        NTuple::Item<int>       m_n_D0shws_side2_low;
        NTuple::Matrix<double>  m_rawp4_D0shws_side2_low;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_side2_low;
        NTuple::Matrix<double>  m_p4_D0trks_4c_side2_low;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_side2_low;
        NTuple::Matrix<double>  m_p4_D0shws_4c_side2_low;
        NTuple::Item<double>    m_chisq_6c_side2_low;
        NTuple::Array<double>   m_p4_D0_6c_side2_low;
        NTuple::Array<double>   m_p4_Dm_6c_side2_low;
        NTuple::Array<double>   m_p4_pip_6c_side2_low;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_side2_low;
        NTuple::Matrix<double>  m_p4_D0trks_6c_side2_low;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_side2_low;
        NTuple::Matrix<double>  m_p4_D0shws_6c_side2_low;

        NTuple::Tuple*          m_tuple_side2_up;
        NTuple::Item<int>       m_runNo_side2_up;
        NTuple::Item<int>       m_evtNo_side2_up;
        NTuple::Item<int>       m_flag1_side2_up;
        NTuple::Item<int>       m_flag2_side2_up;
        NTuple::Item<int>       m_idxmc_side2_up;
        NTuple::Array<int>      m_pdgid_side2_up;
        NTuple::Array<int>      m_trkidx_side2_up;
        NTuple::Array<int>      m_motherid_side2_up;
        NTuple::Array<int>      m_motheridx_side2_up;
        NTuple::Item<double>    m_D0_mode_side2_up;
        NTuple::Item<double>    m_D0_type_side2_up;
        NTuple::Item<int>       m_D0_dcharge_side2_up;
        NTuple::Item<double>    m_D0_charm_side2_up;
        NTuple::Item<double>    m_D0_mBC_side2_up;
        NTuple::Item<double>    m_D0_numofchildren_side2_up;
        NTuple::Item<double>    m_D0_mass_side2_up;
        NTuple::Item<double>    m_D0_be_side2_up;
        NTuple::Item<double>    m_D0_deltae_side2_up;
        NTuple::Item<double>    m_Dm_mode_side2_up;
        NTuple::Item<double>    m_Dm_type_side2_up;
        NTuple::Item<int>       m_Dm_dcharge_side2_up;
        NTuple::Item<double>    m_Dm_charm_side2_up;
        NTuple::Item<double>    m_Dm_mBC_side2_up;
        NTuple::Item<double>    m_Dm_numofchildren_side2_up;
        NTuple::Item<double>    m_Dm_mass_side2_up;
        NTuple::Item<double>    m_Dm_be_side2_up;
        NTuple::Item<double>    m_Dm_deltae_side2_up;
        NTuple::Item<double>    m_chisq_4c_side2_up;
        NTuple::Array<double>   m_p4_D0_4c_side2_up;
        NTuple::Array<double>   m_p4_Dm_4c_side2_up;
        NTuple::Array<double>   m_p4_pip_4c_side2_up;
        NTuple::Array<double>   m_vtx_tagPi_side2_up;
        NTuple::Array<double>   m_rawp4_tagPi_side2_up;
        NTuple::Item<double>    m_chisq_vf_side2_up;
        NTuple::Item<int>       m_n_othershws_side2_up;
        NTuple::Matrix<double>  m_vtx_othershws_side2_up;
        NTuple::Matrix<double>  m_rawp4_othershws_side2_up;
        NTuple::Item<int>       m_n_pi0_side2_up;
        NTuple::Array<double>   m_chi2_pi0_side2_up;
        NTuple::Matrix<double>  m_p4_pi0_side2_up;
        NTuple::Item<int>       m_n_Dmtrks_side2_up;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_side2_up;
        NTuple::Matrix<double>  m_vtx_Dmtrks_side2_up;
        NTuple::Matrix<double>  m_vtx_Dmshws_side2_up;
        NTuple::Item<int>       m_n_D0trks_side2_up;
        NTuple::Matrix<double>  m_rawp4_D0trks_side2_up;
        NTuple::Matrix<double>  m_vtx_D0trks_side2_up;
        NTuple::Matrix<double>  m_vtx_D0shws_side2_up;
        NTuple::Item<int>       m_n_Dmshws_side2_up;
        NTuple::Matrix<double>  m_rawp4_Dmshws_side2_up;
        NTuple::Item<int>       m_n_D0shws_side2_up;
        NTuple::Matrix<double>  m_rawp4_D0shws_side2_up;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_side2_up;
        NTuple::Matrix<double>  m_p4_D0trks_4c_side2_up;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_side2_up;
        NTuple::Matrix<double>  m_p4_D0shws_4c_side2_up;
        NTuple::Item<double>    m_chisq_6c_side2_up;
        NTuple::Array<double>   m_p4_D0_6c_side2_up;
        NTuple::Array<double>   m_p4_Dm_6c_side2_up;
        NTuple::Array<double>   m_p4_pip_6c_side2_up;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_side2_up;
        NTuple::Matrix<double>  m_p4_D0trks_6c_side2_up;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_side2_up;
        NTuple::Matrix<double>  m_p4_D0shws_6c_side2_up;

        NTuple::Tuple*          m_tuple_side3_low;
        NTuple::Item<int>       m_runNo_side3_low;
        NTuple::Item<int>       m_evtNo_side3_low;
        NTuple::Item<int>       m_flag1_side3_low;
        NTuple::Item<int>       m_flag2_side3_low;
        NTuple::Item<int>       m_idxmc_side3_low;
        NTuple::Array<int>      m_pdgid_side3_low;
        NTuple::Array<int>      m_trkidx_side3_low;
        NTuple::Array<int>      m_motherid_side3_low;
        NTuple::Array<int>      m_motheridx_side3_low;
        NTuple::Item<double>    m_D0_mode_side3_low;
        NTuple::Item<double>    m_D0_type_side3_low;
        NTuple::Item<int>       m_D0_dcharge_side3_low;
        NTuple::Item<double>    m_D0_charm_side3_low;
        NTuple::Item<double>    m_D0_mBC_side3_low;
        NTuple::Item<double>    m_D0_numofchildren_side3_low;
        NTuple::Item<double>    m_D0_mass_side3_low;
        NTuple::Item<double>    m_D0_be_side3_low;
        NTuple::Item<double>    m_D0_deltae_side3_low;
        NTuple::Item<double>    m_Dm_mode_side3_low;
        NTuple::Item<double>    m_Dm_type_side3_low;
        NTuple::Item<int>       m_Dm_dcharge_side3_low;
        NTuple::Item<double>    m_Dm_charm_side3_low;
        NTuple::Item<double>    m_Dm_mBC_side3_low;
        NTuple::Item<double>    m_Dm_numofchildren_side3_low;
        NTuple::Item<double>    m_Dm_mass_side3_low;
        NTuple::Item<double>    m_Dm_be_side3_low;
        NTuple::Item<double>    m_Dm_deltae_side3_low;
        NTuple::Item<double>    m_chisq_4c_side3_low;
        NTuple::Array<double>   m_p4_D0_4c_side3_low;
        NTuple::Array<double>   m_p4_Dm_4c_side3_low;
        NTuple::Array<double>   m_p4_pip_4c_side3_low;
        NTuple::Array<double>   m_vtx_tagPi_side3_low;
        NTuple::Array<double>   m_rawp4_tagPi_side3_low;
        NTuple::Item<double>    m_chisq_vf_side3_low;
        NTuple::Item<int>       m_n_othershws_side3_low;
        NTuple::Matrix<double>  m_vtx_othershws_side3_low;
        NTuple::Matrix<double>  m_rawp4_othershws_side3_low;
        NTuple::Item<int>       m_n_pi0_side3_low;
        NTuple::Array<double>   m_chi2_pi0_side3_low;
        NTuple::Matrix<double>  m_p4_pi0_side3_low;
        NTuple::Item<int>       m_n_Dmtrks_side3_low;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_side3_low;
        NTuple::Matrix<double>  m_vtx_Dmtrks_side3_low;
        NTuple::Matrix<double>  m_vtx_Dmshws_side3_low;
        NTuple::Item<int>       m_n_D0trks_side3_low;
        NTuple::Matrix<double>  m_rawp4_D0trks_side3_low;
        NTuple::Matrix<double>  m_vtx_D0trks_side3_low;
        NTuple::Matrix<double>  m_vtx_D0shws_side3_low;
        NTuple::Item<int>       m_n_Dmshws_side3_low;
        NTuple::Matrix<double>  m_rawp4_Dmshws_side3_low;
        NTuple::Item<int>       m_n_D0shws_side3_low;
        NTuple::Matrix<double>  m_rawp4_D0shws_side3_low;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_side3_low;
        NTuple::Matrix<double>  m_p4_D0trks_4c_side3_low;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_side3_low;
        NTuple::Matrix<double>  m_p4_D0shws_4c_side3_low;
        NTuple::Item<double>    m_chisq_6c_side3_low;
        NTuple::Array<double>   m_p4_D0_6c_side3_low;
        NTuple::Array<double>   m_p4_Dm_6c_side3_low;
        NTuple::Array<double>   m_p4_pip_6c_side3_low;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_side3_low;
        NTuple::Matrix<double>  m_p4_D0trks_6c_side3_low;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_side3_low;
        NTuple::Matrix<double>  m_p4_D0shws_6c_side3_low;

        NTuple::Tuple*          m_tuple_side3_up;
        NTuple::Item<int>       m_runNo_side3_up;
        NTuple::Item<int>       m_evtNo_side3_up;
        NTuple::Item<int>       m_flag1_side3_up;
        NTuple::Item<int>       m_flag2_side3_up;
        NTuple::Item<int>       m_idxmc_side3_up;
        NTuple::Array<int>      m_pdgid_side3_up;
        NTuple::Array<int>      m_trkidx_side3_up;
        NTuple::Array<int>      m_motherid_side3_up;
        NTuple::Array<int>      m_motheridx_side3_up;
        NTuple::Item<double>    m_D0_mode_side3_up;
        NTuple::Item<double>    m_D0_type_side3_up;
        NTuple::Item<int>       m_D0_dcharge_side3_up;
        NTuple::Item<double>    m_D0_charm_side3_up;
        NTuple::Item<double>    m_D0_mBC_side3_up;
        NTuple::Item<double>    m_D0_numofchildren_side3_up;
        NTuple::Item<double>    m_D0_mass_side3_up;
        NTuple::Item<double>    m_D0_be_side3_up;
        NTuple::Item<double>    m_D0_deltae_side3_up;
        NTuple::Item<double>    m_Dm_mode_side3_up;
        NTuple::Item<double>    m_Dm_type_side3_up;
        NTuple::Item<int>       m_Dm_dcharge_side3_up;
        NTuple::Item<double>    m_Dm_charm_side3_up;
        NTuple::Item<double>    m_Dm_mBC_side3_up;
        NTuple::Item<double>    m_Dm_numofchildren_side3_up;
        NTuple::Item<double>    m_Dm_mass_side3_up;
        NTuple::Item<double>    m_Dm_be_side3_up;
        NTuple::Item<double>    m_Dm_deltae_side3_up;
        NTuple::Item<double>    m_chisq_4c_side3_up;
        NTuple::Array<double>   m_p4_D0_4c_side3_up;
        NTuple::Array<double>   m_p4_Dm_4c_side3_up;
        NTuple::Array<double>   m_p4_pip_4c_side3_up;
        NTuple::Array<double>   m_vtx_tagPi_side3_up;
        NTuple::Array<double>   m_rawp4_tagPi_side3_up;
        NTuple::Item<double>    m_chisq_vf_side3_up;
        NTuple::Item<int>       m_n_othershws_side3_up;
        NTuple::Matrix<double>  m_vtx_othershws_side3_up;
        NTuple::Matrix<double>  m_rawp4_othershws_side3_up;
        NTuple::Item<int>       m_n_pi0_side3_up;
        NTuple::Array<double>   m_chi2_pi0_side3_up;
        NTuple::Matrix<double>  m_p4_pi0_side3_up;
        NTuple::Item<int>       m_n_Dmtrks_side3_up;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_side3_up;
        NTuple::Matrix<double>  m_vtx_Dmtrks_side3_up;
        NTuple::Matrix<double>  m_vtx_Dmshws_side3_up;
        NTuple::Item<int>       m_n_D0trks_side3_up;
        NTuple::Matrix<double>  m_rawp4_D0trks_side3_up;
        NTuple::Matrix<double>  m_vtx_D0trks_side3_up;
        NTuple::Matrix<double>  m_vtx_D0shws_side3_up;
        NTuple::Item<int>       m_n_Dmshws_side3_up;
        NTuple::Matrix<double>  m_rawp4_Dmshws_side3_up;
        NTuple::Item<int>       m_n_D0shws_side3_up;
        NTuple::Matrix<double>  m_rawp4_D0shws_side3_up;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_side3_up;
        NTuple::Matrix<double>  m_p4_D0trks_4c_side3_up;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_side3_up;
        NTuple::Matrix<double>  m_p4_D0shws_4c_side3_up;
        NTuple::Item<double>    m_chisq_6c_side3_up;
        NTuple::Array<double>   m_p4_D0_6c_side3_up;
        NTuple::Array<double>   m_p4_Dm_6c_side3_up;
        NTuple::Array<double>   m_p4_pip_6c_side3_up;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_side3_up;
        NTuple::Matrix<double>  m_p4_D0trks_6c_side3_up;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_side3_up;
        NTuple::Matrix<double>  m_p4_D0shws_6c_side3_up;

        NTuple::Tuple*          m_tuple_side4_low;
        NTuple::Item<int>       m_runNo_side4_low;
        NTuple::Item<int>       m_evtNo_side4_low;
        NTuple::Item<int>       m_flag1_side4_low;
        NTuple::Item<int>       m_flag2_side4_low;
        NTuple::Item<int>       m_idxmc_side4_low;
        NTuple::Array<int>      m_pdgid_side4_low;
        NTuple::Array<int>      m_trkidx_side4_low;
        NTuple::Array<int>      m_motherid_side4_low;
        NTuple::Array<int>      m_motheridx_side4_low;
        NTuple::Item<double>    m_D0_mode_side4_low;
        NTuple::Item<double>    m_D0_type_side4_low;
        NTuple::Item<int>       m_D0_dcharge_side4_low;
        NTuple::Item<double>    m_D0_charm_side4_low;
        NTuple::Item<double>    m_D0_mBC_side4_low;
        NTuple::Item<double>    m_D0_numofchildren_side4_low;
        NTuple::Item<double>    m_D0_mass_side4_low;
        NTuple::Item<double>    m_D0_be_side4_low;
        NTuple::Item<double>    m_D0_deltae_side4_low;
        NTuple::Item<double>    m_Dm_mode_side4_low;
        NTuple::Item<double>    m_Dm_type_side4_low;
        NTuple::Item<int>       m_Dm_dcharge_side4_low;
        NTuple::Item<double>    m_Dm_charm_side4_low;
        NTuple::Item<double>    m_Dm_mBC_side4_low;
        NTuple::Item<double>    m_Dm_numofchildren_side4_low;
        NTuple::Item<double>    m_Dm_mass_side4_low;
        NTuple::Item<double>    m_Dm_be_side4_low;
        NTuple::Item<double>    m_Dm_deltae_side4_low;
        NTuple::Item<double>    m_chisq_4c_side4_low;
        NTuple::Array<double>   m_p4_D0_4c_side4_low;
        NTuple::Array<double>   m_p4_Dm_4c_side4_low;
        NTuple::Array<double>   m_p4_pip_4c_side4_low;
        NTuple::Array<double>   m_vtx_tagPi_side4_low;
        NTuple::Array<double>   m_rawp4_tagPi_side4_low;
        NTuple::Item<double>    m_chisq_vf_side4_low;
        NTuple::Item<int>       m_n_othershws_side4_low;
        NTuple::Matrix<double>  m_vtx_othershws_side4_low;
        NTuple::Matrix<double>  m_rawp4_othershws_side4_low;
        NTuple::Item<int>       m_n_pi0_side4_low;
        NTuple::Array<double>   m_chi2_pi0_side4_low;
        NTuple::Matrix<double>  m_p4_pi0_side4_low;
        NTuple::Item<int>       m_n_Dmtrks_side4_low;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_side4_low;
        NTuple::Matrix<double>  m_vtx_Dmtrks_side4_low;
        NTuple::Matrix<double>  m_vtx_Dmshws_side4_low;
        NTuple::Item<int>       m_n_D0trks_side4_low;
        NTuple::Matrix<double>  m_rawp4_D0trks_side4_low;
        NTuple::Matrix<double>  m_vtx_D0trks_side4_low;
        NTuple::Matrix<double>  m_vtx_D0shws_side4_low;
        NTuple::Item<int>       m_n_Dmshws_side4_low;
        NTuple::Matrix<double>  m_rawp4_Dmshws_side4_low;
        NTuple::Item<int>       m_n_D0shws_side4_low;
        NTuple::Matrix<double>  m_rawp4_D0shws_side4_low;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_side4_low;
        NTuple::Matrix<double>  m_p4_D0trks_4c_side4_low;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_side4_low;
        NTuple::Matrix<double>  m_p4_D0shws_4c_side4_low;
        NTuple::Item<double>    m_chisq_6c_side4_low;
        NTuple::Array<double>   m_p4_D0_6c_side4_low;
        NTuple::Array<double>   m_p4_Dm_6c_side4_low;
        NTuple::Array<double>   m_p4_pip_6c_side4_low;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_side4_low;
        NTuple::Matrix<double>  m_p4_D0trks_6c_side4_low;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_side4_low;
        NTuple::Matrix<double>  m_p4_D0shws_6c_side4_low;

        NTuple::Tuple*          m_tuple_side4_up;
        NTuple::Item<int>       m_runNo_side4_up;
        NTuple::Item<int>       m_evtNo_side4_up;
        NTuple::Item<int>       m_flag1_side4_up;
        NTuple::Item<int>       m_flag2_side4_up;
        NTuple::Item<int>       m_idxmc_side4_up;
        NTuple::Array<int>      m_pdgid_side4_up;
        NTuple::Array<int>      m_trkidx_side4_up;
        NTuple::Array<int>      m_motherid_side4_up;
        NTuple::Array<int>      m_motheridx_side4_up;
        NTuple::Item<double>    m_D0_mode_side4_up;
        NTuple::Item<double>    m_D0_type_side4_up;
        NTuple::Item<int>       m_D0_dcharge_side4_up;
        NTuple::Item<double>    m_D0_charm_side4_up;
        NTuple::Item<double>    m_D0_mBC_side4_up;
        NTuple::Item<double>    m_D0_numofchildren_side4_up;
        NTuple::Item<double>    m_D0_mass_side4_up;
        NTuple::Item<double>    m_D0_be_side4_up;
        NTuple::Item<double>    m_D0_deltae_side4_up;
        NTuple::Item<double>    m_Dm_mode_side4_up;
        NTuple::Item<double>    m_Dm_type_side4_up;
        NTuple::Item<int>       m_Dm_dcharge_side4_up;
        NTuple::Item<double>    m_Dm_charm_side4_up;
        NTuple::Item<double>    m_Dm_mBC_side4_up;
        NTuple::Item<double>    m_Dm_numofchildren_side4_up;
        NTuple::Item<double>    m_Dm_mass_side4_up;
        NTuple::Item<double>    m_Dm_be_side4_up;
        NTuple::Item<double>    m_Dm_deltae_side4_up;
        NTuple::Item<double>    m_chisq_4c_side4_up;
        NTuple::Array<double>   m_p4_D0_4c_side4_up;
        NTuple::Array<double>   m_p4_Dm_4c_side4_up;
        NTuple::Array<double>   m_p4_pip_4c_side4_up;
        NTuple::Array<double>   m_vtx_tagPi_side4_up;
        NTuple::Array<double>   m_rawp4_tagPi_side4_up;
        NTuple::Item<double>    m_chisq_vf_side4_up;
        NTuple::Item<int>       m_n_othershws_side4_up;
        NTuple::Matrix<double>  m_vtx_othershws_side4_up;
        NTuple::Matrix<double>  m_rawp4_othershws_side4_up;
        NTuple::Item<int>       m_n_pi0_side4_up;
        NTuple::Array<double>   m_chi2_pi0_side4_up;
        NTuple::Matrix<double>  m_p4_pi0_side4_up;
        NTuple::Item<int>       m_n_Dmtrks_side4_up;
        NTuple::Matrix<double>  m_rawp4_Dmtrks_side4_up;
        NTuple::Matrix<double>  m_vtx_Dmtrks_side4_up;
        NTuple::Matrix<double>  m_vtx_Dmshws_side4_up;
        NTuple::Item<int>       m_n_D0trks_side4_up;
        NTuple::Matrix<double>  m_rawp4_D0trks_side4_up;
        NTuple::Matrix<double>  m_vtx_D0trks_side4_up;
        NTuple::Matrix<double>  m_vtx_D0shws_side4_up;
        NTuple::Item<int>       m_n_Dmshws_side4_up;
        NTuple::Matrix<double>  m_rawp4_Dmshws_side4_up;
        NTuple::Item<int>       m_n_D0shws_side4_up;
        NTuple::Matrix<double>  m_rawp4_D0shws_side4_up;
        NTuple::Matrix<double>  m_p4_Dmtrks_4c_side4_up;
        NTuple::Matrix<double>  m_p4_D0trks_4c_side4_up;
        NTuple::Matrix<double>  m_p4_Dmshws_4c_side4_up;
        NTuple::Matrix<double>  m_p4_D0shws_4c_side4_up;
        NTuple::Item<double>    m_chisq_6c_side4_up;
        NTuple::Array<double>   m_p4_D0_6c_side4_up;
        NTuple::Array<double>   m_p4_Dm_6c_side4_up;
        NTuple::Array<double>   m_p4_pip_6c_side4_up;
        NTuple::Matrix<double>  m_p4_Dmtrks_6c_side4_up;
        NTuple::Matrix<double>  m_p4_D0trks_6c_side4_up;
        NTuple::Matrix<double>  m_p4_Dmshws_6c_side4_up;
        NTuple::Matrix<double>  m_p4_D0shws_6c_side4_up;

        void recordVariables(std::string sample_type);
        double fitVertex(VWTrkPara &vwtrkpara, VertexParameter &birth_photon);
        bool fitpi0(VWTrkPara &vwtrkpara_photons, VertexParameter &birth);
        int MatchMC(HepLorentzVector &p4, std::string MODE);
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

const double mPart_Dm[5][5] = {
	 {mka, mpi, mpi},
	 {mka, mpi, mpi},
	 {mpi, mpi, mpi},
	 {mpi, mpi, mpi},
	 {mpi, mpi, mpi, mpi, mpi}
};
#endif 
