#ifndef Physics_Analysis_PiD0Dm_H
#define Physics_Analysis_PiD0Dm_H 

#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"
#include "CLHEP/Matrix/SymMatrix.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "VertexFit/IVertexDbSvc.h"
#include "MdcRecEvent/RecMdcKalTrack.h"

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
  double ecms;  
  double setecms;
   // define Ntuples here


  NTuple::Tuple*  m_tuple1;    // charged track vertex
    NTuple::Item<double>  m_vx0;
    NTuple::Item<double>  m_vy0;
    NTuple::Item<double>  m_vz0;
    NTuple::Item<double>  m_vr0;
  
  NTuple::Tuple*  m_tuple2;
    NTuple::Item<int>       m_idxmc_mc;
    NTuple::Array<int>      m_pdgid_mc;
    NTuple::Array<int>      m_trkIdx_mc;
    NTuple::Array<int>      m_motherIdx_mc;
    NTuple::Array<int>      m_motherid_mc;
    NTuple::Array<double>   m_mc_px_mc;
    NTuple::Array<double>   m_mc_py_mc;
    NTuple::Array<double>   m_mc_pz_mc;
    NTuple::Array<double>   m_mc_pt_mc;
    NTuple::Array<double>   m_mc_p_mc;
    NTuple::Array<double>   m_mc_e_mc;
    NTuple::Array<double>   m_mc_m_mc;
    NTuple::Array<double>   m_mc_theta_mc;
    NTuple::Array<double>   m_mc_phi_mc;
    NTuple::Item<int> m_flag1_mc;
    NTuple::Item<int> m_flag2_mc;

  

  NTuple::Tuple*  m_tuple3;    // information of D Dbar pip pim and recoil
    NTuple::Item<double>  m_ecms; 
    NTuple::Item<double>  m_chisq;                     
    NTuple::Item<double>  m_D0_px;
    NTuple::Item<double>  m_D0_py;
    NTuple::Item<double>  m_D0_pz;
    NTuple::Item<double>  m_D0_p; 
    NTuple::Item<double>  m_D0_e;
    NTuple::Item<double>  m_D0_m;
    NTuple::Item<double>  m_Dm_px;                     
    NTuple::Item<double>  m_Dm_py;
    NTuple::Item<double>  m_Dm_pz;
    NTuple::Item<double>  m_Dm_p;
    NTuple::Item<double>  m_Dm_e;
    NTuple::Item<double>  m_Dm_m;
    NTuple::Item<double>  m_DDbar_px;                     
    NTuple::Item<double>  m_DDbar_py;
    NTuple::Item<double>  m_DDbar_pz;
    NTuple::Item<double>  m_DDbar_p;
    NTuple::Item<double>  m_DDbar_e;
    NTuple::Item<double>  m_DDbar_m;
    NTuple::Item<double>  m_DDbar_cosTh;
    NTuple::Item<double>  m_DDbar_bcosTh;
    NTuple::Item<double>  m_DDbarrecoil_px;                     
    NTuple::Item<double>  m_DDbarrecoil_py;
    NTuple::Item<double>  m_DDbarrecoil_pz;
    NTuple::Item<double>  m_DDbarrecoil_p;
    NTuple::Item<double>  m_DDbarrecoil_e;
    NTuple::Item<double>  m_DDbarrecoil_m;
    NTuple::Item<double>  m_D0pip_m;
    NTuple::Item<double>  m_Dmpip_m;
    NTuple::Item<double>  m_pip_px;
    NTuple::Item<double>  m_pip_py;
    NTuple::Item<double>  m_pip_pz;
    NTuple::Item<double>  m_pip_p;
    NTuple::Item<double>  m_pip_e;
    NTuple::Item<double>  m_pip_m;
    NTuple::Item<double>  m_piprecoil_px;
    NTuple::Item<double>  m_piprecoil_py;
    NTuple::Item<double>  m_piprecoil_pz;
    NTuple::Item<double>  m_piprecoil_p;
    NTuple::Item<double>  m_piprecoil_e;
    NTuple::Item<double>  m_piprecoil_m;
    NTuple::Item<double>  m_D0_mode;
    NTuple::Item<double>  m_D0_type;
    NTuple::Item<int>     m_D0_dcharge;
    NTuple::Item<double>  m_D0_charm;
    NTuple::Item<double>  m_D0_mBC;
    NTuple::Item<double>  m_D0_numofchildren;
    NTuple::Item<double>  m_D0_mass;
    NTuple::Item<double>  m_D0_be;
    NTuple::Item<double>  m_D0_deltae;
    NTuple::Item<double>  m_Dm_mode;
    NTuple::Item<double>  m_Dm_type;
    NTuple::Item<int>     m_Dm_dcharge;
    NTuple::Item<double>  m_Dm_charm;
    NTuple::Item<double>  m_Dm_mBC;
    NTuple::Item<double>  m_Dm_numofchildren;
    NTuple::Item<double>  m_Dm_mass;
    NTuple::Item<double>  m_Dm_be;
    NTuple::Item<double>  m_Dm_deltae;
    NTuple::Item<double>  m_D0_numtrk;
    NTuple::Item<double>  m_D0_numshowers;
    NTuple::Item<double>  m_D0_numothertrk;
    NTuple::Item<double>  m_D0_numothershowers;
    NTuple::Item<double>  m_Dm_numtrk;
    NTuple::Item<double>  m_Dm_numshowers;
    NTuple::Item<double>  m_Dm_numothertrk;
    NTuple::Item<double>  m_Dm_numothershowers;
    NTuple::Item<int>       m_run;
    NTuple::Item<int>       m_event;
    NTuple::Item<int>       m_idxmc;
    NTuple::Array<int>      m_pdgid;
    NTuple::Array<int>      m_trkIdx;
    NTuple::Array<int>      m_motherIdx;
    NTuple::Array<int>      m_motherid;
    NTuple::Array<double>   m_mc_px;
    NTuple::Array<double>   m_mc_py;
    NTuple::Array<double>   m_mc_pz;
    NTuple::Array<double>   m_mc_pt;
    NTuple::Array<double>   m_mc_p;
    NTuple::Array<double>   m_mc_e;
    NTuple::Array<double>   m_mc_m;
    NTuple::Array<double>   m_mc_theta;
    NTuple::Array<double>   m_mc_phi;
    NTuple::Item<int>       m_flag1;
    NTuple::Item<int>       m_flag2;


  NTuple::Tuple*  m_tuple4;    //tagD
    NTuple::Item<double>    k_ecms;
    NTuple::Item<double>    k_D0_mode;
    NTuple::Item<double>    k_D0_type;
    NTuple::Item<int>       k_D0_dcharge;
    NTuple::Item<double>    k_D0_charm;
    NTuple::Item<double>    k_D0_mBC;
    NTuple::Item<double>    k_D0_numofchildren;
    NTuple::Item<double>    k_D0_mass;
    NTuple::Item<double>    k_D0_be;
    NTuple::Item<double>    k_D0_deltae;
    NTuple::Item<double>    k_Dm_mode;
    NTuple::Item<double>    k_Dm_type;
    NTuple::Item<int>       k_Dm_dcharge;
    NTuple::Item<double>    k_Dm_charm;
    NTuple::Item<double>    k_Dm_mBC;
    NTuple::Item<double>    k_Dm_numofchildren;
    NTuple::Item<double>    k_Dm_mass;
    NTuple::Item<double>    k_Dm_be;
    NTuple::Item<double>    k_Dm_deltae;
    NTuple::Item<double>    k_chisq;                     
    NTuple::Item<double>    k_D0_px;                     
    NTuple::Item<double>    k_D0_py;
    NTuple::Item<double>    k_D0_pz;
    NTuple::Item<double>    k_D0_p;
    NTuple::Item<double>    k_D0_e;
    NTuple::Item<double>    k_D0_m;
    NTuple::Item<double>    k_Dm_px;                     
    NTuple::Item<double>    k_Dm_py;
    NTuple::Item<double>    k_Dm_pz;
    NTuple::Item<double>    k_Dm_p;
    NTuple::Item<double>    k_Dm_e;
    NTuple::Item<double>    k_Dm_m;
    NTuple::Item<double>    k_DDbar_px;                     
    NTuple::Item<double>    k_DDbar_py;
    NTuple::Item<double>    k_DDbar_pz;
    NTuple::Item<double>    k_DDbar_p;
    NTuple::Item<double>    k_DDbar_e;
    NTuple::Item<double>    k_DDbar_m;
    NTuple::Item<double>    k_DDbarrecoil_m;
    NTuple::Item<double>    k_D0pip_m;
    NTuple::Item<double>    k_Dmpip_m;
    NTuple::Item<double>    k_piprecoil_m;
    NTuple::Item<double>    k_pip_px;
    NTuple::Item<double>    k_pip_py;
    NTuple::Item<double>    k_pip_pz;
    NTuple::Item<double>    k_pip_p;
    NTuple::Item<double>    k_pip_e;
    NTuple::Item<double>    k_pip_m;
    NTuple::Item<double>    C_chisq;                     
    NTuple::Item<double>    C_D0_px;                     
    NTuple::Item<double>    C_D0_py;
    NTuple::Item<double>    C_D0_pz;
    NTuple::Item<double>    C_D0_p;
    NTuple::Item<double>    C_D0_e;
    NTuple::Item<double>    C_D0_m;
    NTuple::Item<double>    C_Dm_px;                     
    NTuple::Item<double>    C_Dm_py;
    NTuple::Item<double>    C_Dm_pz;
    NTuple::Item<double>    C_Dm_p;
    NTuple::Item<double>    C_Dm_e;
    NTuple::Item<double>    C_Dm_m;
    NTuple::Item<double>    C_DDbar_px;                     
    NTuple::Item<double>    C_DDbar_py;
    NTuple::Item<double>    C_DDbar_pz;
    NTuple::Item<double>    C_DDbar_p;
    NTuple::Item<double>    C_DDbar_e;
    NTuple::Item<double>    C_DDbar_m;
    NTuple::Item<double>    C_DDbarrecoil_m;
    NTuple::Item<double>    C_D0pip_m;
    NTuple::Item<double>    C_Dmpip_m;
    NTuple::Item<double>    C_piprecoil_m;
    NTuple::Item<double>    C_pip_px;
    NTuple::Item<double>    C_pip_py;
    NTuple::Item<double>    C_pip_pz;
    NTuple::Item<double>    C_pip_p;
    NTuple::Item<double>    C_pip_e;
    NTuple::Item<double>    C_pip_m;
    NTuple::Item<int>       k_idxmc;
    NTuple::Array<int>      k_pdgid;
    NTuple::Array<int>      k_trkIdx;
    NTuple::Array<int>      k_motherIdx;
    NTuple::Array<int>      k_motherid;
    NTuple::Array<double>   k_mc_px;
    NTuple::Array<double>   k_mc_py;
    NTuple::Array<double>   k_mc_pz;
    NTuple::Array<double>   k_mc_pt;
    NTuple::Array<double>   k_mc_p;
    NTuple::Array<double>   k_mc_e;
    NTuple::Array<double>   k_mc_m;
    NTuple::Array<double>   k_mc_theta;
    NTuple::Array<double>   k_mc_phi;

    void saveDTInfo(CLHEP::HepLorentzVector, CLHEP::HepLorentzVector, CLHEP::HepLorentzVector);
    void saveDTInfok(CLHEP::HepLorentzVector, CLHEP::HepLorentzVector, CLHEP::HepLorentzVector);
    void saveDTInfo6C(CLHEP::HepLorentzVector, CLHEP::HepLorentzVector, CLHEP::HepLorentzVector);
    void calibration(RecMdcKalTrack *, HepVector &, int);
};

const double mpi = 0.13957;
const double mka = 0.493677;
const double mPart_D0[5][4] = {
	 {mka,mpi},
	 {mka,mpi},
	 {mka,mpi},
	 {mka,mpi,mpi,mpi},
	 {mka,mpi,mpi,mpi}
};

const double mPart_Dp[5][5] = {
	 {mka,mpi,mpi},
	 {mka,mpi,mpi},
	 {mpi,mpi,mpi},
	 {mpi,mpi,mpi},
	 {mpi,mpi,mpi,mpi,mpi}
};
#endif 
