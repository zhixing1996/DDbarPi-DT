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
        double m_ecms;
        bool m_debug;
        int runNo;
        int evtNo;
        int flag1;
        int flag2;
        
        // All McTruth info
        int pdgid[100];
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
        NTuple::Item<double>    m_chisq_6c;
        NTuple::Array<double>   m_p4_D0_6c;
        NTuple::Array<double>   m_p4_Dm_6c;
        NTuple::Array<double>   m_p4_pip_6c;
        
        void recordVariables();
};

const double mpi = 0.13957;
const double mka = 0.493677;
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
