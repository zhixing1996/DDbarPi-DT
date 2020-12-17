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
        declareProperty("AddModesDm", m_modes_Dm);
        declareProperty("Ecms", m_ecms);
        declareProperty("Debug", m_debug = false);
        declareProperty("W_D0", m_W_D0 = 0.035);
        declareProperty("W_Dm", m_W_Dm = 0.035);
        declareProperty("sample_types", m_sample_types);
        declareProperty("UsePID", m_pid = true);
}

StatusCode PiD0Dm::initialize() {
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "in initialize()" << endmsg;
    StatusCode status;

    for (int i = 0; i < m_sample_types.size(); i++) {
        if (m_sample_types[i] == "signal") {
            NTuplePtr nt_signal(ntupleSvc(), "FILE1/DTD_signal");
            if (nt_signal) m_tuple_signal = nt_signal;
            else {
                m_tuple_signal = ntupleSvc()->book ("FILE1/DTD_signal", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_signal) {
                    status = m_tuple_signal->addItem("runNo",                            m_runNo_signal);
                    status = m_tuple_signal->addItem("evtNo",                            m_evtNo_signal);
                    status = m_tuple_signal->addItem("flag1",                            m_flag1_signal);
                    status = m_tuple_signal->addItem("flag2",                            m_flag2_signal);
                    status = m_tuple_signal->addItem("indexmc",                          m_idxmc_signal, 0, 100);
                    status = m_tuple_signal->addIndexedItem("pdgid",                     m_idxmc_signal, m_pdgid_signal);
                    status = m_tuple_signal->addIndexedItem("trkidx",                    m_idxmc_signal, m_trkidx_signal);
                    status = m_tuple_signal->addIndexedItem("motherid",                  m_idxmc_signal, m_motherid_signal);
                    status = m_tuple_signal->addIndexedItem("motheridx",                 m_idxmc_signal, m_motheridx_signal);
                    status = m_tuple_signal->addItem("D0_mode",                          m_D0_mode_signal);
                    status = m_tuple_signal->addItem("D0_type",                          m_D0_type_signal);
                    status = m_tuple_signal->addItem("D0_dcharge",                       m_D0_dcharge_signal);
                    status = m_tuple_signal->addItem("D0_charm",                         m_D0_charm_signal);
                    status = m_tuple_signal->addItem("D0_numofchildren",                 m_D0_numofchildren_signal);
                    status = m_tuple_signal->addItem("D0_mass",                          m_D0_mass_signal);
                    status = m_tuple_signal->addItem("Dm_mode",                          m_Dm_mode_signal);
                    status = m_tuple_signal->addItem("Dm_type",                          m_Dm_type_signal);
                    status = m_tuple_signal->addItem("Dm_dcharge",                       m_Dm_dcharge_signal);
                    status = m_tuple_signal->addItem("Dm_charm",                         m_Dm_charm_signal);
                    status = m_tuple_signal->addItem("Dm_numofchildren",                 m_Dm_numofchildren_signal);
                    status = m_tuple_signal->addItem("Dm_mass",                          m_Dm_mass_signal);
                    status = m_tuple_signal->addItem("chisq_4c",                         m_chisq_4c_signal);
                    status = m_tuple_signal->addItem("p4_D0_4c",                         4, m_p4_D0_4c_signal);
                    status = m_tuple_signal->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_signal);
                    status = m_tuple_signal->addItem("p4_pip_4c",                        4, m_p4_pip_4c_signal);
                    status = m_tuple_signal->addItem("vtx_tagPi",                        3, m_vtx_tagPi_signal);
                    status = m_tuple_signal->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_signal);
                    status = m_tuple_signal->addItem("chisq_vf",                         m_chisq_vf_signal);
                    status = m_tuple_signal->addItem("n_othershws",                      m_n_othershws_signal, 0, 50);
                    status = m_tuple_signal->addIndexedItem("vtx_othershws",             m_n_othershws_signal, 6, m_vtx_othershws_signal);
                    status = m_tuple_signal->addIndexedItem("rawp4_othershws",           m_n_othershws_signal, 4, m_rawp4_othershws_signal);
                    status = m_tuple_signal->addItem("n_pi0",                            m_n_pi0_signal, 0, 200);
                    status = m_tuple_signal->addIndexedItem("chi2_pi0",                  m_n_pi0_signal, m_chi2_pi0_signal);
                    status = m_tuple_signal->addIndexedItem("p4_pi0",                    m_n_pi0_signal, 4, m_p4_pi0_signal);
                    status = m_tuple_signal->addItem("n_Dmtrks",                         m_n_Dmtrks_signal, 0, 10);
                    status = m_tuple_signal->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_signal, 8, m_rawp4_Dmtrks_signal);
                    status = m_tuple_signal->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_signal, 3, m_vtx_Dmtrks_signal);
                    status = m_tuple_signal->addItem("n_D0trks",                         m_n_D0trks_signal, 0, 10);
                    status = m_tuple_signal->addIndexedItem("rawp4_D0trks",              m_n_D0trks_signal, 8, m_rawp4_D0trks_signal);
                    status = m_tuple_signal->addIndexedItem("vtx_D0trks",                m_n_D0trks_signal, 3, m_vtx_D0trks_signal);
                    status = m_tuple_signal->addItem("n_Dmshws",                         m_n_Dmshws_signal, 0, 10);
                    status = m_tuple_signal->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_signal, 6, m_rawp4_Dmshws_signal);
                    status = m_tuple_signal->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_signal, 6, m_vtx_Dmshws_signal);
                    status = m_tuple_signal->addItem("n_D0shws",                         m_n_D0shws_signal, 0, 10);
                    status = m_tuple_signal->addIndexedItem("rawp4_D0shws",              m_n_D0shws_signal, 6, m_rawp4_D0shws_signal);
                    status = m_tuple_signal->addIndexedItem("vtx_D0shws",                m_n_D0shws_signal, 6, m_vtx_D0shws_signal);
                    status = m_tuple_signal->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_signal, 4, m_p4_Dmtrks_4c_signal);
                    status = m_tuple_signal->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_signal, 4, m_p4_D0trks_4c_signal);
                    status = m_tuple_signal->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_signal, 4, m_p4_Dmshws_4c_signal);
                    status = m_tuple_signal->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_signal, 4, m_p4_D0shws_4c_signal);
                    status = m_tuple_signal->addItem("chisq_6c",                         m_chisq_6c_signal);
                    status = m_tuple_signal->addItem("p4_D0_6c",                         4, m_p4_D0_6c_signal);
                    status = m_tuple_signal->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_signal);
                    status = m_tuple_signal->addItem("p4_pip_6c",                        4, m_p4_pip_6c_signal);
                    status = m_tuple_signal->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_signal, 4, m_p4_Dmtrks_6c_signal);
                    status = m_tuple_signal->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_signal, 4, m_p4_D0trks_6c_signal);
                    status = m_tuple_signal->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_signal, 4, m_p4_Dmshws_6c_signal);
                    status = m_tuple_signal->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_signal, 4, m_p4_D0shws_6c_signal);
                    status = m_tuple_signal->addItem("chisq_svf_D0",                     m_chisq_svf_D0_signal);
                    status = m_tuple_signal->addItem("ctau_svf_D0",                      m_ctau_svf_D0_signal);
                    status = m_tuple_signal->addItem("L_svf_D0",                         m_L_svf_D0_signal);
                    status = m_tuple_signal->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_signal);
                    status = m_tuple_signal->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_signal);
                    status = m_tuple_signal->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_signal);
                    status = m_tuple_signal->addItem("L_svf_Dm",                         m_L_svf_Dm_signal);
                    status = m_tuple_signal->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_signal);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_signal) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
        }
        if (m_sample_types[i] == "side1_low") {
            NTuplePtr nt_side1_low(ntupleSvc(), "FILE1/DTD_side1_low");
            if (nt_side1_low) m_tuple_side1_low = nt_side1_low;
            else {
                m_tuple_side1_low = ntupleSvc()->book ("FILE1/DTD_side1_low", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_side1_low) {
                    status = m_tuple_side1_low->addItem("runNo",                            m_runNo_side1_low);
                    status = m_tuple_side1_low->addItem("evtNo",                            m_evtNo_side1_low);
                    status = m_tuple_side1_low->addItem("flag1",                            m_flag1_side1_low);
                    status = m_tuple_side1_low->addItem("flag2",                            m_flag2_side1_low);
                    status = m_tuple_side1_low->addItem("indexmc",                          m_idxmc_side1_low, 0, 100);
                    status = m_tuple_side1_low->addIndexedItem("pdgid",                     m_idxmc_side1_low, m_pdgid_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("trkidx",                    m_idxmc_side1_low, m_trkidx_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("motherid",                  m_idxmc_side1_low, m_motherid_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("motheridx",                 m_idxmc_side1_low, m_motheridx_side1_low);
                    status = m_tuple_side1_low->addItem("D0_mode",                          m_D0_mode_side1_low);
                    status = m_tuple_side1_low->addItem("D0_type",                          m_D0_type_side1_low);
                    status = m_tuple_side1_low->addItem("D0_dcharge",                       m_D0_dcharge_side1_low);
                    status = m_tuple_side1_low->addItem("D0_charm",                         m_D0_charm_side1_low);
                    status = m_tuple_side1_low->addItem("D0_numofchildren",                 m_D0_numofchildren_side1_low);
                    status = m_tuple_side1_low->addItem("D0_mass",                          m_D0_mass_side1_low);
                    status = m_tuple_side1_low->addItem("Dm_mode",                          m_Dm_mode_side1_low);
                    status = m_tuple_side1_low->addItem("Dm_type",                          m_Dm_type_side1_low);
                    status = m_tuple_side1_low->addItem("Dm_dcharge",                       m_Dm_dcharge_side1_low);
                    status = m_tuple_side1_low->addItem("Dm_charm",                         m_Dm_charm_side1_low);
                    status = m_tuple_side1_low->addItem("Dm_numofchildren",                 m_Dm_numofchildren_side1_low);
                    status = m_tuple_side1_low->addItem("Dm_mass",                          m_Dm_mass_side1_low);
                    status = m_tuple_side1_low->addItem("chisq_4c",                         m_chisq_4c_side1_low);
                    status = m_tuple_side1_low->addItem("p4_D0_4c",                         4, m_p4_D0_4c_side1_low);
                    status = m_tuple_side1_low->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_side1_low);
                    status = m_tuple_side1_low->addItem("p4_pip_4c",                        4, m_p4_pip_4c_side1_low);
                    status = m_tuple_side1_low->addItem("vtx_tagPi",                        3, m_vtx_tagPi_side1_low);
                    status = m_tuple_side1_low->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_side1_low);
                    status = m_tuple_side1_low->addItem("chisq_vf",                         m_chisq_vf_side1_low);
                    status = m_tuple_side1_low->addItem("n_othershws",                      m_n_othershws_side1_low, 0, 50);
                    status = m_tuple_side1_low->addIndexedItem("vtx_othershws",             m_n_othershws_side1_low, 6, m_vtx_othershws_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("rawp4_othershws",           m_n_othershws_side1_low, 4, m_rawp4_othershws_side1_low);
                    status = m_tuple_side1_low->addItem("n_pi0",                            m_n_pi0_side1_low, 0, 200);
                    status = m_tuple_side1_low->addIndexedItem("chi2_pi0",                  m_n_pi0_side1_low, m_chi2_pi0_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_pi0",                    m_n_pi0_side1_low, 4, m_p4_pi0_side1_low);
                    status = m_tuple_side1_low->addItem("n_Dmtrks",                         m_n_Dmtrks_side1_low, 0, 10);
                    status = m_tuple_side1_low->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_side1_low, 8, m_rawp4_Dmtrks_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_side1_low, 3, m_vtx_Dmtrks_side1_low);
                    status = m_tuple_side1_low->addItem("n_D0trks",                         m_n_D0trks_side1_low, 0, 10);
                    status = m_tuple_side1_low->addIndexedItem("rawp4_D0trks",              m_n_D0trks_side1_low, 8, m_rawp4_D0trks_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("vtx_D0trks",                m_n_D0trks_side1_low, 3, m_vtx_D0trks_side1_low);
                    status = m_tuple_side1_low->addItem("n_Dmshws",                         m_n_Dmshws_side1_low, 0, 10);
                    status = m_tuple_side1_low->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_side1_low, 6, m_rawp4_Dmshws_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_side1_low, 6, m_vtx_Dmshws_side1_low);
                    status = m_tuple_side1_low->addItem("n_D0shws",                         m_n_D0shws_side1_low, 0, 10);
                    status = m_tuple_side1_low->addIndexedItem("rawp4_D0shws",              m_n_D0shws_side1_low, 6, m_rawp4_D0shws_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("vtx_D0shws",                m_n_D0shws_side1_low, 6, m_vtx_D0shws_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_side1_low, 4, m_p4_Dmtrks_4c_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_side1_low, 4, m_p4_D0trks_4c_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_side1_low, 4, m_p4_Dmshws_4c_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_side1_low, 4, m_p4_D0shws_4c_side1_low);
                    status = m_tuple_side1_low->addItem("chisq_6c",                         m_chisq_6c_side1_low);
                    status = m_tuple_side1_low->addItem("p4_D0_6c",                         4, m_p4_D0_6c_side1_low);
                    status = m_tuple_side1_low->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_side1_low);
                    status = m_tuple_side1_low->addItem("p4_pip_6c",                        4, m_p4_pip_6c_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_side1_low, 4, m_p4_Dmtrks_6c_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_side1_low, 4, m_p4_D0trks_6c_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_side1_low, 4, m_p4_Dmshws_6c_side1_low);
                    status = m_tuple_side1_low->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_side1_low, 4, m_p4_D0shws_6c_side1_low);
                    status = m_tuple_side1_low->addItem("chisq_svf_D0",                     m_chisq_svf_D0_side1_low);
                    status = m_tuple_side1_low->addItem("ctau_svf_D0",                      m_ctau_svf_D0_side1_low);
                    status = m_tuple_side1_low->addItem("L_svf_D0",                         m_L_svf_D0_side1_low);
                    status = m_tuple_side1_low->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_side1_low);
                    status = m_tuple_side1_low->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_side1_low);
                    status = m_tuple_side1_low->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_side1_low);
                    status = m_tuple_side1_low->addItem("L_svf_Dm",                         m_L_svf_Dm_side1_low);
                    status = m_tuple_side1_low->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_side1_low);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_side1_low) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
        }
        if (m_sample_types[i] == "side1_up") {
            NTuplePtr nt_side1_up(ntupleSvc(), "FILE1/DTD_side1_up");
            if (nt_side1_up) m_tuple_side1_up = nt_side1_up;
            else {
                m_tuple_side1_up = ntupleSvc()->book ("FILE1/DTD_side1_up", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_side1_up) {
                    status = m_tuple_side1_up->addItem("runNo",                            m_runNo_side1_up);
                    status = m_tuple_side1_up->addItem("evtNo",                            m_evtNo_side1_up);
                    status = m_tuple_side1_up->addItem("flag1",                            m_flag1_side1_up);
                    status = m_tuple_side1_up->addItem("flag2",                            m_flag2_side1_up);
                    status = m_tuple_side1_up->addItem("indexmc",                          m_idxmc_side1_up, 0, 100);
                    status = m_tuple_side1_up->addIndexedItem("pdgid",                     m_idxmc_side1_up, m_pdgid_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("trkidx",                    m_idxmc_side1_up, m_trkidx_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("motherid",                  m_idxmc_side1_up, m_motherid_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("motheridx",                 m_idxmc_side1_up, m_motheridx_side1_up);
                    status = m_tuple_side1_up->addItem("D0_mode",                          m_D0_mode_side1_up);
                    status = m_tuple_side1_up->addItem("D0_type",                          m_D0_type_side1_up);
                    status = m_tuple_side1_up->addItem("D0_dcharge",                       m_D0_dcharge_side1_up);
                    status = m_tuple_side1_up->addItem("D0_charm",                         m_D0_charm_side1_up);
                    status = m_tuple_side1_up->addItem("D0_numofchildren",                 m_D0_numofchildren_side1_up);
                    status = m_tuple_side1_up->addItem("D0_mass",                          m_D0_mass_side1_up);
                    status = m_tuple_side1_up->addItem("Dm_mode",                          m_Dm_mode_side1_up);
                    status = m_tuple_side1_up->addItem("Dm_type",                          m_Dm_type_side1_up);
                    status = m_tuple_side1_up->addItem("Dm_dcharge",                       m_Dm_dcharge_side1_up);
                    status = m_tuple_side1_up->addItem("Dm_charm",                         m_Dm_charm_side1_up);
                    status = m_tuple_side1_up->addItem("Dm_numofchildren",                 m_Dm_numofchildren_side1_up);
                    status = m_tuple_side1_up->addItem("Dm_mass",                          m_Dm_mass_side1_up);
                    status = m_tuple_side1_up->addItem("chisq_4c",                         m_chisq_4c_side1_up);
                    status = m_tuple_side1_up->addItem("p4_D0_4c",                         4, m_p4_D0_4c_side1_up);
                    status = m_tuple_side1_up->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_side1_up);
                    status = m_tuple_side1_up->addItem("p4_pip_4c",                        4, m_p4_pip_4c_side1_up);
                    status = m_tuple_side1_up->addItem("vtx_tagPi",                        3, m_vtx_tagPi_side1_up);
                    status = m_tuple_side1_up->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_side1_up);
                    status = m_tuple_side1_up->addItem("chisq_vf",                         m_chisq_vf_side1_up);
                    status = m_tuple_side1_up->addItem("n_othershws",                      m_n_othershws_side1_up, 0, 50);
                    status = m_tuple_side1_up->addIndexedItem("vtx_othershws",             m_n_othershws_side1_up, 6, m_vtx_othershws_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("rawp4_othershws",           m_n_othershws_side1_up, 4, m_rawp4_othershws_side1_up);
                    status = m_tuple_side1_up->addItem("n_pi0",                            m_n_pi0_side1_up, 0, 200);
                    status = m_tuple_side1_up->addIndexedItem("chi2_pi0",                  m_n_pi0_side1_up, m_chi2_pi0_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_pi0",                    m_n_pi0_side1_up, 4, m_p4_pi0_side1_up);
                    status = m_tuple_side1_up->addItem("n_Dmtrks",                         m_n_Dmtrks_side1_up, 0, 10);
                    status = m_tuple_side1_up->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_side1_up, 8, m_rawp4_Dmtrks_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_side1_up, 3, m_vtx_Dmtrks_side1_up);
                    status = m_tuple_side1_up->addItem("n_D0trks",                         m_n_D0trks_side1_up, 0, 10);
                    status = m_tuple_side1_up->addIndexedItem("rawp4_D0trks",              m_n_D0trks_side1_up, 8, m_rawp4_D0trks_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("vtx_D0trks",                m_n_D0trks_side1_up, 3, m_vtx_D0trks_side1_up);
                    status = m_tuple_side1_up->addItem("n_Dmshws",                         m_n_Dmshws_side1_up, 0, 10);
                    status = m_tuple_side1_up->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_side1_up, 6, m_rawp4_Dmshws_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_side1_up, 6, m_vtx_Dmshws_side1_up);
                    status = m_tuple_side1_up->addItem("n_D0shws",                         m_n_D0shws_side1_up, 0, 10);
                    status = m_tuple_side1_up->addIndexedItem("rawp4_D0shws",              m_n_D0shws_side1_up, 6, m_rawp4_D0shws_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("vtx_D0shws",                m_n_D0shws_side1_up, 6, m_vtx_D0shws_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_side1_up, 4, m_p4_Dmtrks_4c_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_side1_up, 4, m_p4_D0trks_4c_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_side1_up, 4, m_p4_Dmshws_4c_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_side1_up, 4, m_p4_D0shws_4c_side1_up);
                    status = m_tuple_side1_up->addItem("chisq_6c",                         m_chisq_6c_side1_up);
                    status = m_tuple_side1_up->addItem("p4_D0_6c",                         4, m_p4_D0_6c_side1_up);
                    status = m_tuple_side1_up->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_side1_up);
                    status = m_tuple_side1_up->addItem("p4_pip_6c",                        4, m_p4_pip_6c_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_side1_up, 4, m_p4_Dmtrks_6c_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_side1_up, 4, m_p4_D0trks_6c_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_side1_up, 4, m_p4_Dmshws_6c_side1_up);
                    status = m_tuple_side1_up->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_side1_up, 4, m_p4_D0shws_6c_side1_up);
                    status = m_tuple_side1_up->addItem("chisq_svf_D0",                     m_chisq_svf_D0_side1_up);
                    status = m_tuple_side1_up->addItem("ctau_svf_D0",                      m_ctau_svf_D0_side1_up);
                    status = m_tuple_side1_up->addItem("L_svf_D0",                         m_L_svf_D0_side1_up);
                    status = m_tuple_side1_up->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_side1_up);
                    status = m_tuple_side1_up->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_side1_up);
                    status = m_tuple_side1_up->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_side1_up);
                    status = m_tuple_side1_up->addItem("L_svf_Dm",                         m_L_svf_Dm_side1_up);
                    status = m_tuple_side1_up->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_side1_up);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_side1_up) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
        }
        if (m_sample_types[i] == "side2_low") {
            NTuplePtr nt_side2_low(ntupleSvc(), "FILE1/DTD_side2_low");
            if (nt_side2_low) m_tuple_side2_low = nt_side2_low;
            else {
                m_tuple_side2_low = ntupleSvc()->book ("FILE1/DTD_side2_low", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_side2_low) {
                    status = m_tuple_side2_low->addItem("runNo",                            m_runNo_side2_low);
                    status = m_tuple_side2_low->addItem("evtNo",                            m_evtNo_side2_low);
                    status = m_tuple_side2_low->addItem("flag1",                            m_flag1_side2_low);
                    status = m_tuple_side2_low->addItem("flag2",                            m_flag2_side2_low);
                    status = m_tuple_side2_low->addItem("indexmc",                          m_idxmc_side2_low, 0, 100);
                    status = m_tuple_side2_low->addIndexedItem("pdgid",                     m_idxmc_side2_low, m_pdgid_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("trkidx",                    m_idxmc_side2_low, m_trkidx_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("motherid",                  m_idxmc_side2_low, m_motherid_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("motheridx",                 m_idxmc_side2_low, m_motheridx_side2_low);
                    status = m_tuple_side2_low->addItem("D0_mode",                          m_D0_mode_side2_low);
                    status = m_tuple_side2_low->addItem("D0_type",                          m_D0_type_side2_low);
                    status = m_tuple_side2_low->addItem("D0_dcharge",                       m_D0_dcharge_side2_low);
                    status = m_tuple_side2_low->addItem("D0_charm",                         m_D0_charm_side2_low);
                    status = m_tuple_side2_low->addItem("D0_numofchildren",                 m_D0_numofchildren_side2_low);
                    status = m_tuple_side2_low->addItem("D0_mass",                          m_D0_mass_side2_low);
                    status = m_tuple_side2_low->addItem("Dm_mode",                          m_Dm_mode_side2_low);
                    status = m_tuple_side2_low->addItem("Dm_type",                          m_Dm_type_side2_low);
                    status = m_tuple_side2_low->addItem("Dm_dcharge",                       m_Dm_dcharge_side2_low);
                    status = m_tuple_side2_low->addItem("Dm_charm",                         m_Dm_charm_side2_low);
                    status = m_tuple_side2_low->addItem("Dm_numofchildren",                 m_Dm_numofchildren_side2_low);
                    status = m_tuple_side2_low->addItem("Dm_mass",                          m_Dm_mass_side2_low);
                    status = m_tuple_side2_low->addItem("chisq_4c",                         m_chisq_4c_side2_low);
                    status = m_tuple_side2_low->addItem("p4_D0_4c",                         4, m_p4_D0_4c_side2_low);
                    status = m_tuple_side2_low->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_side2_low);
                    status = m_tuple_side2_low->addItem("p4_pip_4c",                        4, m_p4_pip_4c_side2_low);
                    status = m_tuple_side2_low->addItem("vtx_tagPi",                        3, m_vtx_tagPi_side2_low);
                    status = m_tuple_side2_low->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_side2_low);
                    status = m_tuple_side2_low->addItem("chisq_vf",                         m_chisq_vf_side2_low);
                    status = m_tuple_side2_low->addItem("n_othershws",                      m_n_othershws_side2_low, 0, 50);
                    status = m_tuple_side2_low->addIndexedItem("vtx_othershws",             m_n_othershws_side2_low, 6, m_vtx_othershws_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("rawp4_othershws",           m_n_othershws_side2_low, 4, m_rawp4_othershws_side2_low);
                    status = m_tuple_side2_low->addItem("n_pi0",                            m_n_pi0_side2_low, 0, 200);
                    status = m_tuple_side2_low->addIndexedItem("chi2_pi0",                  m_n_pi0_side2_low, m_chi2_pi0_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_pi0",                    m_n_pi0_side2_low, 4, m_p4_pi0_side2_low);
                    status = m_tuple_side2_low->addItem("n_Dmtrks",                         m_n_Dmtrks_side2_low, 0, 10);
                    status = m_tuple_side2_low->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_side2_low, 8, m_rawp4_Dmtrks_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_side2_low, 3, m_vtx_Dmtrks_side2_low);
                    status = m_tuple_side2_low->addItem("n_D0trks",                         m_n_D0trks_side2_low, 0, 10);
                    status = m_tuple_side2_low->addIndexedItem("rawp4_D0trks",              m_n_D0trks_side2_low, 8, m_rawp4_D0trks_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("vtx_D0trks",                m_n_D0trks_side2_low, 3, m_vtx_D0trks_side2_low);
                    status = m_tuple_side2_low->addItem("n_Dmshws",                         m_n_Dmshws_side2_low, 0, 10);
                    status = m_tuple_side2_low->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_side2_low, 6, m_rawp4_Dmshws_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_side2_low, 6, m_vtx_Dmshws_side2_low);
                    status = m_tuple_side2_low->addItem("n_D0shws",                         m_n_D0shws_side2_low, 0, 10);
                    status = m_tuple_side2_low->addIndexedItem("rawp4_D0shws",              m_n_D0shws_side2_low, 6, m_rawp4_D0shws_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("vtx_D0shws",                m_n_D0shws_side2_low, 6, m_vtx_D0shws_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_side2_low, 4, m_p4_Dmtrks_4c_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_side2_low, 4, m_p4_D0trks_4c_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_side2_low, 4, m_p4_Dmshws_4c_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_side2_low, 4, m_p4_D0shws_4c_side2_low);
                    status = m_tuple_side2_low->addItem("chisq_6c",                         m_chisq_6c_side2_low);
                    status = m_tuple_side2_low->addItem("p4_D0_6c",                         4, m_p4_D0_6c_side2_low);
                    status = m_tuple_side2_low->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_side2_low);
                    status = m_tuple_side2_low->addItem("p4_pip_6c",                        4, m_p4_pip_6c_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_side2_low, 4, m_p4_Dmtrks_6c_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_side2_low, 4, m_p4_D0trks_6c_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_side2_low, 4, m_p4_Dmshws_6c_side2_low);
                    status = m_tuple_side2_low->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_side2_low, 4, m_p4_D0shws_6c_side2_low);
                    status = m_tuple_side2_low->addItem("chisq_svf_D0",                     m_chisq_svf_D0_side2_low);
                    status = m_tuple_side2_low->addItem("ctau_svf_D0",                      m_ctau_svf_D0_side2_low);
                    status = m_tuple_side2_low->addItem("L_svf_D0",                         m_L_svf_D0_side2_low);
                    status = m_tuple_side2_low->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_side2_low);
                    status = m_tuple_side2_low->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_side2_low);
                    status = m_tuple_side2_low->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_side2_low);
                    status = m_tuple_side2_low->addItem("L_svf_Dm",                         m_L_svf_Dm_side2_low);
                    status = m_tuple_side2_low->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_side2_low);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_side2_low) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
        }
        if (m_sample_types[i] == "side2_up") {
            NTuplePtr nt_side2_up(ntupleSvc(), "FILE1/DTD_side2_up");
            if (nt_side2_up) m_tuple_side2_up = nt_side2_up;
            else {
                m_tuple_side2_up = ntupleSvc()->book ("FILE1/DTD_side2_up", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_side2_up) {
                    status = m_tuple_side2_up->addItem("runNo",                            m_runNo_side2_up);
                    status = m_tuple_side2_up->addItem("evtNo",                            m_evtNo_side2_up);
                    status = m_tuple_side2_up->addItem("flag1",                            m_flag1_side2_up);
                    status = m_tuple_side2_up->addItem("flag2",                            m_flag2_side2_up);
                    status = m_tuple_side2_up->addItem("indexmc",                          m_idxmc_side2_up, 0, 100);
                    status = m_tuple_side2_up->addIndexedItem("pdgid",                     m_idxmc_side2_up, m_pdgid_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("trkidx",                    m_idxmc_side2_up, m_trkidx_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("motherid",                  m_idxmc_side2_up, m_motherid_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("motheridx",                 m_idxmc_side2_up, m_motheridx_side2_up);
                    status = m_tuple_side2_up->addItem("D0_mode",                          m_D0_mode_side2_up);
                    status = m_tuple_side2_up->addItem("D0_type",                          m_D0_type_side2_up);
                    status = m_tuple_side2_up->addItem("D0_dcharge",                       m_D0_dcharge_side2_up);
                    status = m_tuple_side2_up->addItem("D0_charm",                         m_D0_charm_side2_up);
                    status = m_tuple_side2_up->addItem("D0_numofchildren",                 m_D0_numofchildren_side2_up);
                    status = m_tuple_side2_up->addItem("D0_mass",                          m_D0_mass_side2_up);
                    status = m_tuple_side2_up->addItem("Dm_mode",                          m_Dm_mode_side2_up);
                    status = m_tuple_side2_up->addItem("Dm_type",                          m_Dm_type_side2_up);
                    status = m_tuple_side2_up->addItem("Dm_dcharge",                       m_Dm_dcharge_side2_up);
                    status = m_tuple_side2_up->addItem("Dm_charm",                         m_Dm_charm_side2_up);
                    status = m_tuple_side2_up->addItem("Dm_numofchildren",                 m_Dm_numofchildren_side2_up);
                    status = m_tuple_side2_up->addItem("Dm_mass",                          m_Dm_mass_side2_up);
                    status = m_tuple_side2_up->addItem("chisq_4c",                         m_chisq_4c_side2_up);
                    status = m_tuple_side2_up->addItem("p4_D0_4c",                         4, m_p4_D0_4c_side2_up);
                    status = m_tuple_side2_up->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_side2_up);
                    status = m_tuple_side2_up->addItem("p4_pip_4c",                        4, m_p4_pip_4c_side2_up);
                    status = m_tuple_side2_up->addItem("vtx_tagPi",                        3, m_vtx_tagPi_side2_up);
                    status = m_tuple_side2_up->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_side2_up);
                    status = m_tuple_side2_up->addItem("chisq_vf",                         m_chisq_vf_side2_up);
                    status = m_tuple_side2_up->addItem("n_othershws",                      m_n_othershws_side2_up, 0, 50);
                    status = m_tuple_side2_up->addIndexedItem("vtx_othershws",             m_n_othershws_side2_up, 6, m_vtx_othershws_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("rawp4_othershws",           m_n_othershws_side2_up, 4, m_rawp4_othershws_side2_up);
                    status = m_tuple_side2_up->addItem("n_pi0",                            m_n_pi0_side2_up, 0, 200);
                    status = m_tuple_side2_up->addIndexedItem("chi2_pi0",                  m_n_pi0_side2_up, m_chi2_pi0_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_pi0",                    m_n_pi0_side2_up, 4, m_p4_pi0_side2_up);
                    status = m_tuple_side2_up->addItem("n_Dmtrks",                         m_n_Dmtrks_side2_up, 0, 10);
                    status = m_tuple_side2_up->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_side2_up, 8, m_rawp4_Dmtrks_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_side2_up, 3, m_vtx_Dmtrks_side2_up);
                    status = m_tuple_side2_up->addItem("n_D0trks",                         m_n_D0trks_side2_up, 0, 10);
                    status = m_tuple_side2_up->addIndexedItem("rawp4_D0trks",              m_n_D0trks_side2_up, 8, m_rawp4_D0trks_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("vtx_D0trks",                m_n_D0trks_side2_up, 3, m_vtx_D0trks_side2_up);
                    status = m_tuple_side2_up->addItem("n_Dmshws",                         m_n_Dmshws_side2_up, 0, 10);
                    status = m_tuple_side2_up->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_side2_up, 6, m_rawp4_Dmshws_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_side2_up, 6, m_vtx_Dmshws_side2_up);
                    status = m_tuple_side2_up->addItem("n_D0shws",                         m_n_D0shws_side2_up, 0, 10);
                    status = m_tuple_side2_up->addIndexedItem("rawp4_D0shws",              m_n_D0shws_side2_up, 6, m_rawp4_D0shws_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("vtx_D0shws",                m_n_D0shws_side2_up, 6, m_vtx_D0shws_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_side2_up, 4, m_p4_Dmtrks_4c_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_side2_up, 4, m_p4_D0trks_4c_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_side2_up, 4, m_p4_Dmshws_4c_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_side2_up, 4, m_p4_D0shws_4c_side2_up);
                    status = m_tuple_side2_up->addItem("chisq_6c",                         m_chisq_6c_side2_up);
                    status = m_tuple_side2_up->addItem("p4_D0_6c",                         4, m_p4_D0_6c_side2_up);
                    status = m_tuple_side2_up->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_side2_up);
                    status = m_tuple_side2_up->addItem("p4_pip_6c",                        4, m_p4_pip_6c_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_side2_up, 4, m_p4_Dmtrks_6c_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_side2_up, 4, m_p4_D0trks_6c_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_side2_up, 4, m_p4_Dmshws_6c_side2_up);
                    status = m_tuple_side2_up->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_side2_up, 4, m_p4_D0shws_6c_side2_up);
                    status = m_tuple_side2_up->addItem("chisq_svf_D0",                     m_chisq_svf_D0_side2_up);
                    status = m_tuple_side2_up->addItem("ctau_svf_D0",                      m_ctau_svf_D0_side2_up);
                    status = m_tuple_side2_up->addItem("L_svf_D0",                         m_L_svf_D0_side2_up);
                    status = m_tuple_side2_up->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_side2_up);
                    status = m_tuple_side2_up->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_side2_up);
                    status = m_tuple_side2_up->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_side2_up);
                    status = m_tuple_side2_up->addItem("L_svf_Dm",                         m_L_svf_Dm_side2_up);
                    status = m_tuple_side2_up->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_side2_up);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_side2_up) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
        }
        if (m_sample_types[i] == "side3_low") {
            NTuplePtr nt_side3_low(ntupleSvc(), "FILE1/DTD_side3_low");
            if (nt_side3_low) m_tuple_side3_low = nt_side3_low;
            else {
                m_tuple_side3_low = ntupleSvc()->book ("FILE1/DTD_side3_low", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_side3_low) {
                    status = m_tuple_side3_low->addItem("runNo",                            m_runNo_side3_low);
                    status = m_tuple_side3_low->addItem("evtNo",                            m_evtNo_side3_low);
                    status = m_tuple_side3_low->addItem("flag1",                            m_flag1_side3_low);
                    status = m_tuple_side3_low->addItem("flag2",                            m_flag2_side3_low);
                    status = m_tuple_side3_low->addItem("indexmc",                          m_idxmc_side3_low, 0, 100);
                    status = m_tuple_side3_low->addIndexedItem("pdgid",                     m_idxmc_side3_low, m_pdgid_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("trkidx",                    m_idxmc_side3_low, m_trkidx_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("motherid",                  m_idxmc_side3_low, m_motherid_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("motheridx",                 m_idxmc_side3_low, m_motheridx_side3_low);
                    status = m_tuple_side3_low->addItem("D0_mode",                          m_D0_mode_side3_low);
                    status = m_tuple_side3_low->addItem("D0_type",                          m_D0_type_side3_low);
                    status = m_tuple_side3_low->addItem("D0_dcharge",                       m_D0_dcharge_side3_low);
                    status = m_tuple_side3_low->addItem("D0_charm",                         m_D0_charm_side3_low);
                    status = m_tuple_side3_low->addItem("D0_numofchildren",                 m_D0_numofchildren_side3_low);
                    status = m_tuple_side3_low->addItem("D0_mass",                          m_D0_mass_side3_low);
                    status = m_tuple_side3_low->addItem("Dm_mode",                          m_Dm_mode_side3_low);
                    status = m_tuple_side3_low->addItem("Dm_type",                          m_Dm_type_side3_low);
                    status = m_tuple_side3_low->addItem("Dm_dcharge",                       m_Dm_dcharge_side3_low);
                    status = m_tuple_side3_low->addItem("Dm_charm",                         m_Dm_charm_side3_low);
                    status = m_tuple_side3_low->addItem("Dm_numofchildren",                 m_Dm_numofchildren_side3_low);
                    status = m_tuple_side3_low->addItem("Dm_mass",                          m_Dm_mass_side3_low);
                    status = m_tuple_side3_low->addItem("chisq_4c",                         m_chisq_4c_side3_low);
                    status = m_tuple_side3_low->addItem("p4_D0_4c",                         4, m_p4_D0_4c_side3_low);
                    status = m_tuple_side3_low->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_side3_low);
                    status = m_tuple_side3_low->addItem("p4_pip_4c",                        4, m_p4_pip_4c_side3_low);
                    status = m_tuple_side3_low->addItem("vtx_tagPi",                        3, m_vtx_tagPi_side3_low);
                    status = m_tuple_side3_low->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_side3_low);
                    status = m_tuple_side3_low->addItem("chisq_vf",                         m_chisq_vf_side3_low);
                    status = m_tuple_side3_low->addItem("n_othershws",                      m_n_othershws_side3_low, 0, 50);
                    status = m_tuple_side3_low->addIndexedItem("vtx_othershws",             m_n_othershws_side3_low, 6, m_vtx_othershws_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("rawp4_othershws",           m_n_othershws_side3_low, 4, m_rawp4_othershws_side3_low);
                    status = m_tuple_side3_low->addItem("n_pi0",                            m_n_pi0_side3_low, 0, 200);
                    status = m_tuple_side3_low->addIndexedItem("chi2_pi0",                  m_n_pi0_side3_low, m_chi2_pi0_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_pi0",                    m_n_pi0_side3_low, 4, m_p4_pi0_side3_low);
                    status = m_tuple_side3_low->addItem("n_Dmtrks",                         m_n_Dmtrks_side3_low, 0, 10);
                    status = m_tuple_side3_low->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_side3_low, 8, m_rawp4_Dmtrks_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_side3_low, 3, m_vtx_Dmtrks_side3_low);
                    status = m_tuple_side3_low->addItem("n_D0trks",                         m_n_D0trks_side3_low, 0, 10);
                    status = m_tuple_side3_low->addIndexedItem("rawp4_D0trks",              m_n_D0trks_side3_low, 8, m_rawp4_D0trks_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("vtx_D0trks",                m_n_D0trks_side3_low, 3, m_vtx_D0trks_side3_low);
                    status = m_tuple_side3_low->addItem("n_Dmshws",                         m_n_Dmshws_side3_low, 0, 10);
                    status = m_tuple_side3_low->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_side3_low, 6, m_rawp4_Dmshws_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_side3_low, 6, m_vtx_Dmshws_side3_low);
                    status = m_tuple_side3_low->addItem("n_D0shws",                         m_n_D0shws_side3_low, 0, 10);
                    status = m_tuple_side3_low->addIndexedItem("rawp4_D0shws",              m_n_D0shws_side3_low, 6, m_rawp4_D0shws_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("vtx_D0shws",                m_n_D0shws_side3_low, 6, m_vtx_D0shws_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_side3_low, 4, m_p4_Dmtrks_4c_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_side3_low, 4, m_p4_D0trks_4c_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_side3_low, 4, m_p4_Dmshws_4c_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_side3_low, 4, m_p4_D0shws_4c_side3_low);
                    status = m_tuple_side3_low->addItem("chisq_6c",                         m_chisq_6c_side3_low);
                    status = m_tuple_side3_low->addItem("p4_D0_6c",                         4, m_p4_D0_6c_side3_low);
                    status = m_tuple_side3_low->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_side3_low);
                    status = m_tuple_side3_low->addItem("p4_pip_6c",                        4, m_p4_pip_6c_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_side3_low, 4, m_p4_Dmtrks_6c_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_side3_low, 4, m_p4_D0trks_6c_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_side3_low, 4, m_p4_Dmshws_6c_side3_low);
                    status = m_tuple_side3_low->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_side3_low, 4, m_p4_D0shws_6c_side3_low);
                    status = m_tuple_side3_low->addItem("chisq_svf_D0",                     m_chisq_svf_D0_side3_low);
                    status = m_tuple_side3_low->addItem("ctau_svf_D0",                      m_ctau_svf_D0_side3_low);
                    status = m_tuple_side3_low->addItem("L_svf_D0",                         m_L_svf_D0_side3_low);
                    status = m_tuple_side3_low->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_side3_low);
                    status = m_tuple_side3_low->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_side3_low);
                    status = m_tuple_side3_low->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_side3_low);
                    status = m_tuple_side3_low->addItem("L_svf_Dm",                         m_L_svf_Dm_side3_low);
                    status = m_tuple_side3_low->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_side3_low);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_side3_low) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
        }
        if (m_sample_types[i] == "side3_up") {
            NTuplePtr nt_side3_up(ntupleSvc(), "FILE1/DTD_side3_up");
            if (nt_side3_up) m_tuple_side3_up = nt_side3_up;
            else {
                m_tuple_side3_up = ntupleSvc()->book ("FILE1/DTD_side3_up", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_side3_up) {
                    status = m_tuple_side3_up->addItem("runNo",                            m_runNo_side3_up);
                    status = m_tuple_side3_up->addItem("evtNo",                            m_evtNo_side3_up);
                    status = m_tuple_side3_up->addItem("flag1",                            m_flag1_side3_up);
                    status = m_tuple_side3_up->addItem("flag2",                            m_flag2_side3_up);
                    status = m_tuple_side3_up->addItem("indexmc",                          m_idxmc_side3_up, 0, 100);
                    status = m_tuple_side3_up->addIndexedItem("pdgid",                     m_idxmc_side3_up, m_pdgid_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("trkidx",                    m_idxmc_side3_up, m_trkidx_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("motherid",                  m_idxmc_side3_up, m_motherid_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("motheridx",                 m_idxmc_side3_up, m_motheridx_side3_up);
                    status = m_tuple_side3_up->addItem("D0_mode",                          m_D0_mode_side3_up);
                    status = m_tuple_side3_up->addItem("D0_type",                          m_D0_type_side3_up);
                    status = m_tuple_side3_up->addItem("D0_dcharge",                       m_D0_dcharge_side3_up);
                    status = m_tuple_side3_up->addItem("D0_charm",                         m_D0_charm_side3_up);
                    status = m_tuple_side3_up->addItem("D0_numofchildren",                 m_D0_numofchildren_side3_up);
                    status = m_tuple_side3_up->addItem("D0_mass",                          m_D0_mass_side3_up);
                    status = m_tuple_side3_up->addItem("Dm_mode",                          m_Dm_mode_side3_up);
                    status = m_tuple_side3_up->addItem("Dm_type",                          m_Dm_type_side3_up);
                    status = m_tuple_side3_up->addItem("Dm_dcharge",                       m_Dm_dcharge_side3_up);
                    status = m_tuple_side3_up->addItem("Dm_charm",                         m_Dm_charm_side3_up);
                    status = m_tuple_side3_up->addItem("Dm_numofchildren",                 m_Dm_numofchildren_side3_up);
                    status = m_tuple_side3_up->addItem("Dm_mass",                          m_Dm_mass_side3_up);
                    status = m_tuple_side3_up->addItem("chisq_4c",                         m_chisq_4c_side3_up);
                    status = m_tuple_side3_up->addItem("p4_D0_4c",                         4, m_p4_D0_4c_side3_up);
                    status = m_tuple_side3_up->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_side3_up);
                    status = m_tuple_side3_up->addItem("p4_pip_4c",                        4, m_p4_pip_4c_side3_up);
                    status = m_tuple_side3_up->addItem("vtx_tagPi",                        3, m_vtx_tagPi_side3_up);
                    status = m_tuple_side3_up->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_side3_up);
                    status = m_tuple_side3_up->addItem("chisq_vf",                         m_chisq_vf_side3_up);
                    status = m_tuple_side3_up->addItem("n_othershws",                      m_n_othershws_side3_up, 0, 50);
                    status = m_tuple_side3_up->addIndexedItem("vtx_othershws",             m_n_othershws_side3_up, 6, m_vtx_othershws_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("rawp4_othershws",           m_n_othershws_side3_up, 4, m_rawp4_othershws_side3_up);
                    status = m_tuple_side3_up->addItem("n_pi0",                            m_n_pi0_side3_up, 0, 200);
                    status = m_tuple_side3_up->addIndexedItem("chi2_pi0",                  m_n_pi0_side3_up, m_chi2_pi0_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_pi0",                    m_n_pi0_side3_up, 4, m_p4_pi0_side3_up);
                    status = m_tuple_side3_up->addItem("n_Dmtrks",                         m_n_Dmtrks_side3_up, 0, 10);
                    status = m_tuple_side3_up->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_side3_up, 8, m_rawp4_Dmtrks_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_side3_up, 3, m_vtx_Dmtrks_side3_up);
                    status = m_tuple_side3_up->addItem("n_D0trks",                         m_n_D0trks_side3_up, 0, 10);
                    status = m_tuple_side3_up->addIndexedItem("rawp4_D0trks",              m_n_D0trks_side3_up, 8, m_rawp4_D0trks_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("vtx_D0trks",                m_n_D0trks_side3_up, 3, m_vtx_D0trks_side3_up);
                    status = m_tuple_side3_up->addItem("n_Dmshws",                         m_n_Dmshws_side3_up, 0, 10);
                    status = m_tuple_side3_up->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_side3_up, 6, m_rawp4_Dmshws_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_side3_up, 6, m_vtx_Dmshws_side3_up);
                    status = m_tuple_side3_up->addItem("n_D0shws",                         m_n_D0shws_side3_up, 0, 10);
                    status = m_tuple_side3_up->addIndexedItem("rawp4_D0shws",              m_n_D0shws_side3_up, 6, m_rawp4_D0shws_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("vtx_D0shws",                m_n_D0shws_side3_up, 6, m_vtx_D0shws_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_side3_up, 4, m_p4_Dmtrks_4c_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_side3_up, 4, m_p4_D0trks_4c_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_side3_up, 4, m_p4_Dmshws_4c_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_side3_up, 4, m_p4_D0shws_4c_side3_up);
                    status = m_tuple_side3_up->addItem("chisq_6c",                         m_chisq_6c_side3_up);
                    status = m_tuple_side3_up->addItem("p4_D0_6c",                         4, m_p4_D0_6c_side3_up);
                    status = m_tuple_side3_up->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_side3_up);
                    status = m_tuple_side3_up->addItem("p4_pip_6c",                        4, m_p4_pip_6c_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_side3_up, 4, m_p4_Dmtrks_6c_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_side3_up, 4, m_p4_D0trks_6c_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_side3_up, 4, m_p4_Dmshws_6c_side3_up);
                    status = m_tuple_side3_up->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_side3_up, 4, m_p4_D0shws_6c_side3_up);
                    status = m_tuple_side3_up->addItem("chisq_svf_D0",                     m_chisq_svf_D0_side3_up);
                    status = m_tuple_side3_up->addItem("ctau_svf_D0",                      m_ctau_svf_D0_side3_up);
                    status = m_tuple_side3_up->addItem("L_svf_D0",                         m_L_svf_D0_side3_up);
                    status = m_tuple_side3_up->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_side3_up);
                    status = m_tuple_side3_up->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_side3_up);
                    status = m_tuple_side3_up->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_side3_up);
                    status = m_tuple_side3_up->addItem("L_svf_Dm",                         m_L_svf_Dm_side3_up);
                    status = m_tuple_side3_up->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_side3_up);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_side3_up) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
        }
        if (m_sample_types[i] == "side4_low") {
            NTuplePtr nt_side4_low(ntupleSvc(), "FILE1/DTD_side4_low");
            if (nt_side4_low) m_tuple_side4_low = nt_side4_low;
            else {
                m_tuple_side4_low = ntupleSvc()->book ("FILE1/DTD_side4_low", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_side4_low) {
                    status = m_tuple_side4_low->addItem("runNo",                            m_runNo_side4_low);
                    status = m_tuple_side4_low->addItem("evtNo",                            m_evtNo_side4_low);
                    status = m_tuple_side4_low->addItem("flag1",                            m_flag1_side4_low);
                    status = m_tuple_side4_low->addItem("flag2",                            m_flag2_side4_low);
                    status = m_tuple_side4_low->addItem("indexmc",                          m_idxmc_side4_low, 0, 100);
                    status = m_tuple_side4_low->addIndexedItem("pdgid",                     m_idxmc_side4_low, m_pdgid_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("trkidx",                    m_idxmc_side4_low, m_trkidx_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("motherid",                  m_idxmc_side4_low, m_motherid_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("motheridx",                 m_idxmc_side4_low, m_motheridx_side4_low);
                    status = m_tuple_side4_low->addItem("D0_mode",                          m_D0_mode_side4_low);
                    status = m_tuple_side4_low->addItem("D0_type",                          m_D0_type_side4_low);
                    status = m_tuple_side4_low->addItem("D0_dcharge",                       m_D0_dcharge_side4_low);
                    status = m_tuple_side4_low->addItem("D0_charm",                         m_D0_charm_side4_low);
                    status = m_tuple_side4_low->addItem("D0_numofchildren",                 m_D0_numofchildren_side4_low);
                    status = m_tuple_side4_low->addItem("D0_mass",                          m_D0_mass_side4_low);
                    status = m_tuple_side4_low->addItem("Dm_mode",                          m_Dm_mode_side4_low);
                    status = m_tuple_side4_low->addItem("Dm_type",                          m_Dm_type_side4_low);
                    status = m_tuple_side4_low->addItem("Dm_dcharge",                       m_Dm_dcharge_side4_low);
                    status = m_tuple_side4_low->addItem("Dm_charm",                         m_Dm_charm_side4_low);
                    status = m_tuple_side4_low->addItem("Dm_numofchildren",                 m_Dm_numofchildren_side4_low);
                    status = m_tuple_side4_low->addItem("Dm_mass",                          m_Dm_mass_side4_low);
                    status = m_tuple_side4_low->addItem("chisq_4c",                         m_chisq_4c_side4_low);
                    status = m_tuple_side4_low->addItem("p4_D0_4c",                         4, m_p4_D0_4c_side4_low);
                    status = m_tuple_side4_low->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_side4_low);
                    status = m_tuple_side4_low->addItem("p4_pip_4c",                        4, m_p4_pip_4c_side4_low);
                    status = m_tuple_side4_low->addItem("vtx_tagPi",                        3, m_vtx_tagPi_side4_low);
                    status = m_tuple_side4_low->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_side4_low);
                    status = m_tuple_side4_low->addItem("chisq_vf",                         m_chisq_vf_side4_low);
                    status = m_tuple_side4_low->addItem("n_othershws",                      m_n_othershws_side4_low, 0, 50);
                    status = m_tuple_side4_low->addIndexedItem("vtx_othershws",             m_n_othershws_side4_low, 6, m_vtx_othershws_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("rawp4_othershws",           m_n_othershws_side4_low, 4, m_rawp4_othershws_side4_low);
                    status = m_tuple_side4_low->addItem("n_pi0",                            m_n_pi0_side4_low, 0, 200);
                    status = m_tuple_side4_low->addIndexedItem("chi2_pi0",                  m_n_pi0_side4_low, m_chi2_pi0_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_pi0",                    m_n_pi0_side4_low, 4, m_p4_pi0_side4_low);
                    status = m_tuple_side4_low->addItem("n_Dmtrks",                         m_n_Dmtrks_side4_low, 0, 10);
                    status = m_tuple_side4_low->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_side4_low, 8, m_rawp4_Dmtrks_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_side4_low, 3, m_vtx_Dmtrks_side4_low);
                    status = m_tuple_side4_low->addItem("n_D0trks",                         m_n_D0trks_side4_low, 0, 10);
                    status = m_tuple_side4_low->addIndexedItem("rawp4_D0trks",              m_n_D0trks_side4_low, 8, m_rawp4_D0trks_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("vtx_D0trks",                m_n_D0trks_side4_low, 3, m_vtx_D0trks_side4_low);
                    status = m_tuple_side4_low->addItem("n_Dmshws",                         m_n_Dmshws_side4_low, 0, 10);
                    status = m_tuple_side4_low->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_side4_low, 6, m_rawp4_Dmshws_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_side4_low, 6, m_vtx_Dmshws_side4_low);
                    status = m_tuple_side4_low->addItem("n_D0shws",                         m_n_D0shws_side4_low, 0, 10);
                    status = m_tuple_side4_low->addIndexedItem("rawp4_D0shws",              m_n_D0shws_side4_low, 6, m_rawp4_D0shws_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("vtx_D0shws",                m_n_D0shws_side4_low, 6, m_vtx_D0shws_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_side4_low, 4, m_p4_Dmtrks_4c_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_side4_low, 4, m_p4_D0trks_4c_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_side4_low, 4, m_p4_Dmshws_4c_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_side4_low, 4, m_p4_D0shws_4c_side4_low);
                    status = m_tuple_side4_low->addItem("chisq_6c",                         m_chisq_6c_side4_low);
                    status = m_tuple_side4_low->addItem("p4_D0_6c",                         4, m_p4_D0_6c_side4_low);
                    status = m_tuple_side4_low->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_side4_low);
                    status = m_tuple_side4_low->addItem("p4_pip_6c",                        4, m_p4_pip_6c_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_side4_low, 4, m_p4_Dmtrks_6c_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_side4_low, 4, m_p4_D0trks_6c_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_side4_low, 4, m_p4_Dmshws_6c_side4_low);
                    status = m_tuple_side4_low->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_side4_low, 4, m_p4_D0shws_6c_side4_low);
                    status = m_tuple_side4_low->addItem("chisq_svf_D0",                     m_chisq_svf_D0_side4_low);
                    status = m_tuple_side4_low->addItem("ctau_svf_D0",                      m_ctau_svf_D0_side4_low);
                    status = m_tuple_side4_low->addItem("L_svf_D0",                         m_L_svf_D0_side4_low);
                    status = m_tuple_side4_low->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_side4_low);
                    status = m_tuple_side4_low->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_side4_low);
                    status = m_tuple_side4_low->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_side4_low);
                    status = m_tuple_side4_low->addItem("L_svf_Dm",                         m_L_svf_Dm_side4_low);
                    status = m_tuple_side4_low->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_side4_low);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_side4_low) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
        }
        if (m_sample_types[i] == "side4_up") {
            NTuplePtr nt_side4_up(ntupleSvc(), "FILE1/DTD_side4_up");
            if (nt_side4_up) m_tuple_side4_up = nt_side4_up;
            else {
                m_tuple_side4_up = ntupleSvc()->book ("FILE1/DTD_side4_up", CLID_ColumnWiseTuple, "double D tag method");
                if (m_tuple_side4_up) {
                    status = m_tuple_side4_up->addItem("runNo",                            m_runNo_side4_up);
                    status = m_tuple_side4_up->addItem("evtNo",                            m_evtNo_side4_up);
                    status = m_tuple_side4_up->addItem("flag1",                            m_flag1_side4_up);
                    status = m_tuple_side4_up->addItem("flag2",                            m_flag2_side4_up);
                    status = m_tuple_side4_up->addItem("indexmc",                          m_idxmc_side4_up, 0, 100);
                    status = m_tuple_side4_up->addIndexedItem("pdgid",                     m_idxmc_side4_up, m_pdgid_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("trkidx",                    m_idxmc_side4_up, m_trkidx_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("motherid",                  m_idxmc_side4_up, m_motherid_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("motheridx",                 m_idxmc_side4_up, m_motheridx_side4_up);
                    status = m_tuple_side4_up->addItem("D0_mode",                          m_D0_mode_side4_up);
                    status = m_tuple_side4_up->addItem("D0_type",                          m_D0_type_side4_up);
                    status = m_tuple_side4_up->addItem("D0_dcharge",                       m_D0_dcharge_side4_up);
                    status = m_tuple_side4_up->addItem("D0_charm",                         m_D0_charm_side4_up);
                    status = m_tuple_side4_up->addItem("D0_numofchildren",                 m_D0_numofchildren_side4_up);
                    status = m_tuple_side4_up->addItem("D0_mass",                          m_D0_mass_side4_up);
                    status = m_tuple_side4_up->addItem("Dm_mode",                          m_Dm_mode_side4_up);
                    status = m_tuple_side4_up->addItem("Dm_type",                          m_Dm_type_side4_up);
                    status = m_tuple_side4_up->addItem("Dm_dcharge",                       m_Dm_dcharge_side4_up);
                    status = m_tuple_side4_up->addItem("Dm_charm",                         m_Dm_charm_side4_up);
                    status = m_tuple_side4_up->addItem("Dm_numofchildren",                 m_Dm_numofchildren_side4_up);
                    status = m_tuple_side4_up->addItem("Dm_mass",                          m_Dm_mass_side4_up);
                    status = m_tuple_side4_up->addItem("chisq_4c",                         m_chisq_4c_side4_up);
                    status = m_tuple_side4_up->addItem("p4_D0_4c",                         4, m_p4_D0_4c_side4_up);
                    status = m_tuple_side4_up->addItem("p4_Dm_4c",                         4, m_p4_Dm_4c_side4_up);
                    status = m_tuple_side4_up->addItem("p4_pip_4c",                        4, m_p4_pip_4c_side4_up);
                    status = m_tuple_side4_up->addItem("vtx_tagPi",                        3, m_vtx_tagPi_side4_up);
                    status = m_tuple_side4_up->addItem("rawp4_tagPi",                      7, m_rawp4_tagPi_side4_up);
                    status = m_tuple_side4_up->addItem("chisq_vf",                         m_chisq_vf_side4_up);
                    status = m_tuple_side4_up->addItem("n_othershws",                      m_n_othershws_side4_up, 0, 50);
                    status = m_tuple_side4_up->addIndexedItem("vtx_othershws",             m_n_othershws_side4_up, 6, m_vtx_othershws_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("rawp4_othershws",           m_n_othershws_side4_up, 4, m_rawp4_othershws_side4_up);
                    status = m_tuple_side4_up->addItem("n_pi0",                            m_n_pi0_side4_up, 0, 200);
                    status = m_tuple_side4_up->addIndexedItem("chi2_pi0",                  m_n_pi0_side4_up, m_chi2_pi0_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_pi0",                    m_n_pi0_side4_up, 4, m_p4_pi0_side4_up);
                    status = m_tuple_side4_up->addItem("n_Dmtrks",                         m_n_Dmtrks_side4_up, 0, 10);
                    status = m_tuple_side4_up->addIndexedItem("rawp4_Dmtrks",              m_n_Dmtrks_side4_up, 8, m_rawp4_Dmtrks_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("vtx_Dmtrks",                m_n_Dmtrks_side4_up, 3, m_vtx_Dmtrks_side4_up);
                    status = m_tuple_side4_up->addItem("n_D0trks",                         m_n_D0trks_side4_up, 0, 10);
                    status = m_tuple_side4_up->addIndexedItem("rawp4_D0trks",              m_n_D0trks_side4_up, 8, m_rawp4_D0trks_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("vtx_D0trks",                m_n_D0trks_side4_up, 3, m_vtx_D0trks_side4_up);
                    status = m_tuple_side4_up->addItem("n_Dmshws",                         m_n_Dmshws_side4_up, 0, 10);
                    status = m_tuple_side4_up->addIndexedItem("rawp4_Dmshws",              m_n_Dmshws_side4_up, 6, m_rawp4_Dmshws_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("vtx_Dmshws",                m_n_Dmshws_side4_up, 6, m_vtx_Dmshws_side4_up);
                    status = m_tuple_side4_up->addItem("n_D0shws",                         m_n_D0shws_side4_up, 0, 10);
                    status = m_tuple_side4_up->addIndexedItem("rawp4_D0shws",              m_n_D0shws_side4_up, 6, m_rawp4_D0shws_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("vtx_D0shws",                m_n_D0shws_side4_up, 6, m_vtx_D0shws_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_Dmtrks_4c",              m_n_Dmtrks_side4_up, 4, m_p4_Dmtrks_4c_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_D0trks_4c",              m_n_D0trks_side4_up, 4, m_p4_D0trks_4c_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_Dmshws_4c",              m_n_Dmshws_side4_up, 4, m_p4_Dmshws_4c_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_D0shws_4c",              m_n_D0shws_side4_up, 4, m_p4_D0shws_4c_side4_up);
                    status = m_tuple_side4_up->addItem("chisq_6c",                         m_chisq_6c_side4_up);
                    status = m_tuple_side4_up->addItem("p4_D0_6c",                         4, m_p4_D0_6c_side4_up);
                    status = m_tuple_side4_up->addItem("p4_Dm_6c",                         4, m_p4_Dm_6c_side4_up);
                    status = m_tuple_side4_up->addItem("p4_pip_6c",                        4, m_p4_pip_6c_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_Dmtrks_6c",              m_n_Dmtrks_side4_up, 4, m_p4_Dmtrks_6c_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_D0trks_6c",              m_n_D0trks_side4_up, 4, m_p4_D0trks_6c_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_Dmshws_6c",              m_n_Dmshws_side4_up, 4, m_p4_Dmshws_6c_side4_up);
                    status = m_tuple_side4_up->addIndexedItem("p4_D0shws_6c",              m_n_D0shws_side4_up, 4, m_p4_D0shws_6c_side4_up);
                    status = m_tuple_side4_up->addItem("chisq_svf_D0",                     m_chisq_svf_D0_side4_up);
                    status = m_tuple_side4_up->addItem("ctau_svf_D0",                      m_ctau_svf_D0_side4_up);
                    status = m_tuple_side4_up->addItem("L_svf_D0",                         m_L_svf_D0_side4_up);
                    status = m_tuple_side4_up->addItem("Lerr_svf_D0",                      m_Lerr_svf_D0_side4_up);
                    status = m_tuple_side4_up->addItem("chisq_svf_Dm",                     m_chisq_svf_Dm_side4_up);
                    status = m_tuple_side4_up->addItem("ctau_svf_Dm",                      m_ctau_svf_Dm_side4_up);
                    status = m_tuple_side4_up->addItem("L_svf_Dm",                         m_L_svf_Dm_side4_up);
                    status = m_tuple_side4_up->addItem("Lerr_svf_Dm",                      m_Lerr_svf_Dm_side4_up);
                }
                else {
                    log << MSG::ERROR << "Cannot book N-tuple:" << long(m_tuple_side4_up) << endmsg;
                    return StatusCode::FAILURE;
                }
            }
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

    HepLorentzVector ecms_lab = HepLorentzVector(sin(0.011)*m_ecms, 0. ,0., m_ecms);
    int ncharge = 0;
    double MassD0 = 1.86486;
    double MassDm = 1.86962;
    double MassD0Cand;
    double MassDmCand;
    double MassPi0 = 0.13498;
    double CHISQ = 999.;
    HepLorentzVector besKpipp4 = 0;
    HepLorentzVector besKD0p4 = 0;
    HepLorentzVector besKDmp4 = 0;
    HepLorentzVector bestpip4 = 0;
    RecMdcKalTrack* bestpiTrkex = NULL;
    EvtRecDTag * besttag1; 
    EvtRecDTag * besttag2;
    
    double D0_signal_low = 1.86483 - m_W_D0/2.;
    double D0_signal_up = 1.86483 + m_W_D0/2.;
    double D0_sidebandlow_up = D0_signal_low - m_W_D0;
    double D0_sidebandlow_low = D0_signal_low - 2. * m_W_D0;
    double D0_sidebandup_low = D0_signal_up + m_W_D0;
    double D0_sidebandup_up = D0_signal_up + 2. * m_W_D0;
    double Dm_signal_low = 1.86965 - m_W_Dm/2.;
    double Dm_signal_up = 1.86965 + m_W_Dm/2.;
    double Dm_sidebandlow_up = Dm_signal_low - m_W_Dm;
    double Dm_sidebandlow_low = Dm_signal_low - 2. * m_W_Dm;
    double Dm_sidebandup_low = Dm_signal_up + m_W_Dm;
    double Dm_sidebandup_up = Dm_signal_up + 2. * m_W_Dm;

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
    for (int mm = 0; mm < m_sample_types.size(); mm++) {
        if (m_sample_types[mm] == "signal") {MassD0Cand = MassD0; MassDmCand = MassDm;}
        else if (m_sample_types[mm] == "side1_low") {MassD0Cand = MassD0; MassDmCand = (Dm_sidebandlow_low + Dm_sidebandlow_up)/2.;}
        else if (m_sample_types[mm] == "side1_up") {MassD0Cand = MassD0; MassDmCand = (Dm_sidebandup_low + Dm_sidebandup_up)/2.;}
        else if (m_sample_types[mm] == "side2_low") {MassD0Cand = (D0_sidebandlow_low + D0_sidebandlow_up)/2.; MassDmCand = MassDm;}
        else if (m_sample_types[mm] == "side2_up") {MassD0Cand = (D0_sidebandup_low + D0_sidebandup_up)/2.; MassDmCand = MassDm;}
        else if (m_sample_types[mm] == "side3_low") {MassD0Cand = (D0_sidebandlow_low + D0_sidebandlow_up)/2.; MassDmCand = (Dm_sidebandlow_low + Dm_sidebandlow_up)/2.;}
        else if (m_sample_types[mm] == "side3_up") {MassD0Cand = (D0_sidebandlow_low + D0_sidebandlow_up)/2.; MassDmCand = (Dm_sidebandup_low + Dm_sidebandup_up)/2.;}
        else if (m_sample_types[mm] == "side4_low") {MassD0Cand = (D0_sidebandup_low + D0_sidebandup_up)/2.; MassDmCand = (Dm_sidebandlow_low + Dm_sidebandlow_up)/2.;}
        else if (m_sample_types[mm] == "side4_up") {MassD0Cand = (D0_sidebandup_low + D0_sidebandup_up)/2.; MassDmCand = (Dm_sidebandup_low + Dm_sidebandup_up)/2.;}
        else {std::cout << "You have press an unexpected data sample, please check!"<< std::endl; continue;}
        for (int modeA = 0; modeA != m_modes_D0.size(); modeA++ ) {
            for (int modeB = 0; modeB != m_modes_Dm.size(); modeB++) {
                Vint ipip, ipim;
                ipip.clear();
                ipim.clear();
                Vp4 ppip, ppim;
                ppip.clear();
                ppim.clear();
                // use util tool to get dtaglist
                DTagTool dtagTool;
                if (m_pid) dtagTool.setPID(true);
                if (dtagTool.isDTagListEmpty()) {
                    return StatusCode::SUCCESS;
                }
                Ncut1++;
                bool dtagflag = false;
                dtagflag = dtagTool.findADTag(m_modes_D0[modeA], m_modes_Dm[modeB], double(runNo), double(evtNo));
                if (dtagflag == false) continue;
                Ncut2++;
                vector<DTagToolIterator> vtag1 = dtagTool.vdtag1();
                vector<DTagToolIterator> vtag2 = dtagTool.vdtag2();
                EvtRecDTag * tag1;
                EvtRecDTag * tag2;
                if (m_debug) {
                    std::cout << "runNo: " << runNo << ", evtNo: " << evtNo << ", modeA = " << m_modes_D0[modeA] << ", modeB= " << m_modes_Dm[modeB] << std::endl;
                    std::cout << "size of vtag1: " << vtag1.size() << std::endl;
                }
                for (int itag = 0; itag < vtag1.size(); itag++) {
                    if (m_debug) std::cout << "vtag1 size: " << vtag1.size() << ", itag: " << itag << std::endl;
                    tag1 = *(vtag1[itag]);
                    tag2 = *(vtag2[itag]);
                    if (m_debug) {
                        std::cout << "modeA = " << m_modes_D0[modeA] << " modeB= " << m_modes_Dm[modeB] << std::endl;
                        std::cout << "Tag mode: " << tag1->charm() << " " << tag1->decayMode() << " " << tag2->charm() << " " << tag2->decayMode() << std::endl;
                    }
                    SmartRefVector<EvtRecTrack> Tracks1 = tag1->tracks();
                    SmartRefVector<EvtRecTrack> Tracks2 = tag2->tracks();
                    SmartRefVector<EvtRecTrack> Showers1 = tag1->showers();
                    SmartRefVector<EvtRecTrack> Showers2 = tag2->showers();
                    SmartRefVector<EvtRecTrack> OtherTracks1 = tag1->otherTracks();
                    SmartRefVector<EvtRecTrack> OtherTracks2 = tag2->otherTracks();
                    int pi_id = 0;
                    ppip.clear();
                    ipip.clear();
                    ppim.clear();
                    ipim.clear();
                    for (int tk = 0; tk < OtherTracks1.size(); tk++) {
                        int t = 0;
                        for (int i = 0; i < Tracks2.size(); i++) {
                            if (OtherTracks1[tk]->trackId() == Tracks2[i]->trackId()) t = 1;
                        }
                        if (t == 1) continue;
                        if (m_debug) std::cout << itag << ": after checking pi from the other D" << std::endl;
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
                    if (m_debug) std::cout << "Npip: " << ppip.size() << ", Npim, " << ppim.size() << ", charm D0: " << tag1->charm() << ", charm Dm: " << tag2->charm() <<std::endl;
                    bool isPipD0Dm = ppip.size() == 1 && ppim.size() == 0 && tag1->charm() == 1 && tag2->charm() == -1;
                    bool isPimD0barDm = ppip.size() == 0 && ppim.size() == 1 && tag1->charm() == -1 && tag2->charm() == 1;
                    if (m_debug) {
                        std::cout << isPipD0Dm << " " << isPimD0barDm << std::endl;
                    }
                    if (!(isPipD0Dm || isPimD0barDm)) continue;
                    if (m_debug) std::cout << itag << ": after finding combination" << std::endl;
                    Ncut3++;
                    VWTrkPara vwtrkpara_charge;
                    vwtrkpara_charge.clear();
                    VWTrkPara vwtrkpara_D0;
                    vwtrkpara_D0.clear();
                    n_D0trks = 0;
                    HepLorentzVector pK;
                    HepLorentzVector ppi;
                    for (int trk1 = 0; trk1 < Tracks1.size(); trk1++) {
                        RecMdcKalTrack* KalTrk = Tracks1[trk1]->mdcKalTrack();
                        if (dtagTool.isKaon(Tracks1[trk1])) {
                            KalTrk->setPidType(RecMdcKalTrack::kaon);
                            vwtrkpara_charge.push_back(WTrackParameter(mka, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                            vwtrkpara_D0.push_back(WTrackParameter(mka, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                            for (int k = 0; k < 4; k++) rawp4_D0trks[n_D0trks][k] = KalTrk->p4(mka)[k];
                            rawp4_D0trks[n_D0trks][4] = KalTrk->charge();
                            rawp4_D0trks[n_D0trks][5] = 3;
                            pK.setPx(KalTrk->p4(mka)[0]);
                            pK.setPy(KalTrk->p4(mka)[1]);
                            pK.setPz(KalTrk->p4(mka)[2]);
                            pK.setE(KalTrk->p4(mka)[3]);
                            if (runNo < 0) rawp4_D0trks[n_D0trks][6] = MatchMC(pK, "D0_tag");
                            else rawp4_D0trks[n_D0trks][6] = -999;
                            if (runNo < 0) rawp4_D0trks[n_D0trks][7] = MatchMC(pK, "Dm_tag");
                            else rawp4_D0trks[n_D0trks][7] = -999;
                            HepVector a = KalTrk->getZHelixK();
                            HepSymMatrix Ea = KalTrk->getZErrorK();
                            HepPoint3D point0(0., 0., 0.);
                            HepPoint3D IP(xorigin[0], xorigin[1], xorigin[2]);
                            VFHelix helixip(point0, a, Ea);
                            helixip.pivot(IP);
                            HepVector vecipa = helixip.a();
                            vtx_D0trks[n_D0trks][0] = fabs(vecipa[0]);
                            vtx_D0trks[n_D0trks][1] = fabs(vecipa[3]);
                            vtx_D0trks[n_D0trks][2] = cos(KalTrk->theta());
                        }
                        else {
                            KalTrk->setPidType(RecMdcKalTrack::pion);
                            vwtrkpara_charge.push_back(WTrackParameter(mpi, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                            vwtrkpara_D0.push_back(WTrackParameter(mpi, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                            for (int k = 0; k < 4; k++) rawp4_D0trks[n_D0trks][k] = KalTrk->p4(mpi)[k];
                            rawp4_D0trks[n_D0trks][4] = KalTrk->charge();
                            rawp4_D0trks[n_D0trks][5] = 2;
                            ppi.setPx(KalTrk->p4(mpi)[0]);
                            ppi.setPy(KalTrk->p4(mpi)[1]);
                            ppi.setPz(KalTrk->p4(mpi)[2]);
                            ppi.setE(KalTrk->p4(mpi)[3]);
                            if (runNo < 0) rawp4_D0trks[n_D0trks][6] = MatchMC(ppi, "D0_tag");
                            else rawp4_D0trks[n_D0trks][6] = -999;
                            if (runNo < 0) rawp4_D0trks[n_D0trks][7] = MatchMC(ppi, "Dm_tag");
                            else rawp4_D0trks[n_D0trks][7] = -999;
                            HepVector a = KalTrk->getZHelixK();
                            HepSymMatrix Ea = KalTrk->getZErrorK();
                            HepPoint3D point0(0., 0., 0.);
                            HepPoint3D IP(xorigin[0], xorigin[1], xorigin[2]);
                            VFHelix helixip(point0, a, Ea);
                            helixip.pivot(IP);
                            HepVector vecipa = helixip.a();
                            vtx_D0trks[n_D0trks][0] = fabs(vecipa[0]);
                            vtx_D0trks[n_D0trks][1] = fabs(vecipa[3]);
                            vtx_D0trks[n_D0trks][2] = cos(KalTrk->theta());
                        }
                        n_D0trks++;
                    }
                    bool is_OK_D0 = fitSecondVertex(vwtrkpara_D0);
                    chisq_svf_D0 = matrix_svf[0];
                    ctau_svf_D0 = matrix_svf[1];
                    L_svf_D0 = matrix_svf[2];
                    Lerr_svf_D0 = matrix_svf[3];
                    VWTrkPara vwtrkpara_Dm;
                    vwtrkpara_Dm.clear();
                    n_Dmtrks = 0;
                    for (int trk2 = 0; trk2 < Tracks2.size(); trk2++) {
                        RecMdcKalTrack* KalTrk = Tracks2[trk2]->mdcKalTrack();
                        if (dtagTool.isKaon(Tracks2[trk2])) {
                            KalTrk->setPidType(RecMdcKalTrack::kaon);
                            vwtrkpara_charge.push_back(WTrackParameter(mka, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                            vwtrkpara_Dm.push_back(WTrackParameter(mka, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                            for (int k = 0; k < 4; k++) rawp4_Dmtrks[n_Dmtrks][k] = KalTrk->p4(mka)[k];
                            rawp4_Dmtrks[n_Dmtrks][4] = KalTrk->charge();
                            rawp4_Dmtrks[n_Dmtrks][5] = 3;
                            pK.setPx(KalTrk->p4(mka)[0]);
                            pK.setPy(KalTrk->p4(mka)[1]);
                            pK.setPz(KalTrk->p4(mka)[2]);
                            pK.setE(KalTrk->p4(mka)[3]);
                            if (runNo < 0) rawp4_Dmtrks[n_Dmtrks][6] = MatchMC(pK, "Dm_tag");
                            else rawp4_Dmtrks[n_Dmtrks][6] = -999;
                            if (runNo < 0) rawp4_Dmtrks[n_Dmtrks][7] = MatchMC(pK, "D0_tag");
                            else rawp4_Dmtrks[n_Dmtrks][7] = -999;
                            HepVector a = KalTrk->getZHelixK();
                            HepSymMatrix Ea = KalTrk->getZErrorK();
                            HepPoint3D point0(0., 0., 0.);
                            HepPoint3D IP(xorigin[0], xorigin[1], xorigin[2]);
                            VFHelix helixip(point0, a, Ea);
                            helixip.pivot(IP);
                            HepVector vecipa = helixip.a();
                            vtx_Dmtrks[n_Dmtrks][0] = fabs(vecipa[0]);
                            vtx_Dmtrks[n_Dmtrks][1] = fabs(vecipa[3]);
                            vtx_Dmtrks[n_Dmtrks][2] = cos(KalTrk->theta());
                        }
                        else {
                            KalTrk->setPidType(RecMdcKalTrack::pion);
                            vwtrkpara_charge.push_back(WTrackParameter(mpi, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                            vwtrkpara_Dm.push_back(WTrackParameter(mpi, KalTrk->getZHelixK(), KalTrk->getZErrorK()));
                            for (int k = 0; k < 4; k++) rawp4_Dmtrks[n_Dmtrks][k] = KalTrk->p4(mpi)[k];
                            rawp4_Dmtrks[n_Dmtrks][4] = KalTrk->charge();
                            rawp4_Dmtrks[n_Dmtrks][5] = 2;
                            ppi.setPx(KalTrk->p4(mpi)[0]);
                            ppi.setPy(KalTrk->p4(mpi)[1]);
                            ppi.setPz(KalTrk->p4(mpi)[2]);
                            ppi.setE(KalTrk->p4(mpi)[3]);
                            if (runNo < 0) rawp4_Dmtrks[n_Dmtrks][6] = MatchMC(ppi, "Dm_tag");
                            else rawp4_Dmtrks[n_Dmtrks][6] = -999;
                            if (runNo < 0) rawp4_Dmtrks[n_Dmtrks][7] = MatchMC(ppi, "D0_tag");
                            else rawp4_Dmtrks[n_Dmtrks][7] = -999;
                            HepVector a = KalTrk->getZHelixK();
                            HepSymMatrix Ea = KalTrk->getZErrorK();
                            HepPoint3D point0(0., 0., 0.);
                            HepPoint3D IP(xorigin[0], xorigin[1], xorigin[2]);
                            VFHelix helixip(point0, a, Ea);
                            helixip.pivot(IP);
                            HepVector vecipa = helixip.a();
                            vtx_Dmtrks[n_Dmtrks][0] = fabs(vecipa[0]);
                            vtx_Dmtrks[n_Dmtrks][1] = fabs(vecipa[3]);
                            vtx_Dmtrks[n_Dmtrks][2] = cos(KalTrk->theta());
                        }
                        n_Dmtrks++;
                    }
                    bool is_OK_Dm = fitSecondVertex(vwtrkpara_Dm);
                    chisq_svf_Dm = matrix_svf[0];
                    ctau_svf_Dm = matrix_svf[1];
                    L_svf_Dm = matrix_svf[2];
                    Lerr_svf_Dm = matrix_svf[3];
                    RecMdcKalTrack *mdcPip = OtherTracks1[pi_id]->mdcKalTrack();
                    mdcPip->setPidType(RecMdcKalTrack::pion);
                    vwtrkpara_charge.push_back(WTrackParameter(mpi, mdcPip->getZHelixK(), mdcPip->getZErrorK()));
                    chisq_vf = fitVertex(vwtrkpara_charge, birth);
                    if (chisq_vf > 999999) continue;
                    if (m_debug) std::cout << itag << ": after Vtx fit" << std::endl;
                    Ncut4++;
                    // Begain to fill the Kinematic Fitter
                    kmfit->init();
                    HepLorentzVector pgam;
                    // Tracks of first D
                    for (int i = 0; i < Tracks1.size(); i++) {
                        RecMdcKalTrack *kapiTrk = Tracks1[i]->mdcKalTrack();
                        WTrackParameter wvkapiTrk = WTrackParameter(mPart_D0[tag1->decayMode()][i], kapiTrk->getZHelix(), kapiTrk->getZError());
                        kmfit->AddTrack(i, wvkapiTrk);
                    }
                    // Showers of first D
                    n_D0shws = 0;
                    for (int i = 0; i < Showers1.size(); i++) {
                        RecEmcShower* gamShower = Showers1[i]->emcShower();
                        kmfit->AddTrack(i + Tracks1.size(), 0.0, gamShower);
                        Hep3Vector Gm_Vec(gamShower->x(), gamShower->y(), gamShower->z());
                        Hep3Vector Gm_Mom = Gm_Vec - birth.vx();
                        Gm_Mom.setMag(gamShower->energy());
                        HepLorentzVector Gm_p4(Gm_Mom, gamShower->energy());
                        pgam.setPx(Gm_p4[0]);
                        pgam.setPy(Gm_p4[1]);
                        pgam.setPz(Gm_p4[2]);
                        pgam.setE(Gm_p4[3]);
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
                        double eraw = gamShower->energy();
                        double phi = gamShower->phi();
                        double the = gamShower->theta();
                        dang = dang * 180 / (CLHEP::pi);
                        int module = gamShower->module();
                        double Tdc = gamShower->time();
                        for (int j = 0; j < 4; j++) rawp4_D0shws[n_D0shws][j] = Gm_p4[j];
                        if (runNo < 0) rawp4_D0shws[n_D0shws][4] = MatchMC(pgam, "D0_tag");
                        else rawp4_D0shws[n_D0shws][4] = -999;
                        if (runNo < 0) rawp4_D0shws[n_D0shws][5] = MatchMC(pgam, "Dm_tag");
                        else rawp4_D0shws[n_D0shws][5] = -999;
                        vtx_D0shws[n_D0shws][0] = eraw;
                        vtx_D0shws[n_D0shws][1] = phi;
                        vtx_D0shws[n_D0shws][2] = the;
                        vtx_D0shws[n_D0shws][3] = dang;
                        vtx_D0shws[n_D0shws][4] = module;
                        vtx_D0shws[n_D0shws][5] = Tdc;
                        n_D0shws++;
                    }
                    // Tracks of secound D
                    for (int i = 0; i < Tracks2.size(); i++) {
                        RecMdcKalTrack *kapiTrk = Tracks2[i]->mdcKalTrack();
                        WTrackParameter wvkapiTrk = WTrackParameter(mPart_Dm[tag2->decayMode()-200][i], kapiTrk->getZHelix(), kapiTrk->getZError());
                        kmfit->AddTrack(i + Tracks1.size() + Showers1.size(), wvkapiTrk);
                    }
                    // Showers of secound D
                    n_Dmshws = 0;
                    for (int i = 0; i < Showers2.size(); i++) {
                        RecEmcShower* gamShower = Showers2[i]->emcShower();
                        kmfit->AddTrack(i + Tracks1.size() + Showers1.size() + Tracks2.size(), 0.0, gamShower);
                        Hep3Vector Gm_Vec(gamShower->x(), gamShower->y(), gamShower->z());
                        Hep3Vector Gm_Mom = Gm_Vec - birth.vx();
                        Gm_Mom.setMag(gamShower->energy());
                        HepLorentzVector Gm_p4(Gm_Mom, gamShower->energy());
                        pgam.setPx(Gm_p4[0]);
                        pgam.setPy(Gm_p4[1]);
                        pgam.setPz(Gm_p4[2]);
                        pgam.setE(Gm_p4[3]);
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
                        double eraw = gamShower->energy();
                        double phi = gamShower->phi();
                        double the = gamShower->theta();
                        dang = dang * 180 / (CLHEP::pi);
                        int module = gamShower->module();
                        double Tdc = gamShower->time();
                        for (int j = 0; j < 4; j++) rawp4_Dmshws[n_Dmshws][j] = Gm_p4[j];
                        if (runNo < 0) rawp4_Dmshws[n_Dmshws][4] = MatchMC(pgam, "Dm_tag");
                        else rawp4_Dmshws[n_Dmshws][4] = -999;
                        if (runNo < 0) rawp4_Dmshws[n_Dmshws][5] = MatchMC(pgam, "D0_tag");
                        else rawp4_Dmshws[n_Dmshws][5] = -999;
                        vtx_Dmshws[n_Dmshws][0] = eraw;
                        vtx_Dmshws[n_Dmshws][1] = phi;
                        vtx_Dmshws[n_Dmshws][2] = the;
                        vtx_Dmshws[n_Dmshws][3] = dang;
                        vtx_Dmshws[n_Dmshws][4] = module;
                        vtx_Dmshws[n_Dmshws][5] = Tdc;
                        n_Dmshws++;
                    }
                    // The pion out of D 
                    RecMdcKalTrack* piex; 
                    WTrackParameter wvpiexTrk_4c;
                    HepLorentzVector pipp4; 
                    if (tag1->charm() == 1) {
                        piex = OtherTracks1[ipip[0]]->mdcKalTrack();
                        pipp4 = ppip[0]; 
                        wvpiexTrk_4c = WTrackParameter(mpi, piex->getZHelix(), piex->getZError());
                    }
                    else if (tag1->charm() == -1) {
                        piex = OtherTracks1[ipim[0]]->mdcKalTrack();
                        pipp4 = ppim[0]; 
                        wvpiexTrk_4c = WTrackParameter(mpi, piex->getZHelix(), piex->getZError());
                    }
                    kmfit->AddTrack(Tracks1.size() + Showers1.size() + Tracks2.size() + Showers2.size(), wvpiexTrk_4c);
                    kmfit->AddFourMomentum(0, ecms_lab);
                    bool oksq_4c = kmfit->Fit();
                    if (!oksq_4c) continue;
                    Ncut5++;
                    chisq_4c = kmfit->chisq();
                    if (m_debug) {
                        std::cout << itag << ": OK 4c" << std::endl;
                        std::cout << "4C fit result: " << oksq_4c << " " << kmfit->chisq() << std::endl;
                    }
                    //Get the P4 of two D and pion from Fitter
                    HepLorentzVector kD0p4_4c = 0;
                    for (int i = 0; i < Tracks1.size(); i++) {
                        kD0p4_4c += kmfit->pfit(i);
                        for (int j = 0; j < 4; j++) p4_D0trks_4c[i][j] = kmfit->pfit(i)[j];
                    }
                    for (int i = 0; i < Showers1.size(); i++) {
                        kD0p4_4c += kmfit->pfit(i + Tracks1.size());
                        for (int j = 0; j < 4; j++) p4_D0shws_4c[i][j] = kmfit->pfit(i + Tracks1.size())[j];
                    }
                    HepLorentzVector kDmp4_4c = 0;
                    for (int i = 0; i < Tracks2.size(); i++) {
                        kDmp4_4c += kmfit->pfit(i + Tracks1.size() + Showers1.size());
                        for (int j = 0; j < 4; j++) p4_Dmtrks_4c[i][j] = kmfit->pfit(i + Tracks1.size() + Showers1.size())[j];
                    }
                    for (int i = 0; i < Showers2.size(); i++) {
                        kDmp4_4c += kmfit->pfit(i + Tracks1.size() + Showers1.size() + Tracks2.size());
                        for (int j = 0; j < 4; j++) p4_Dmshws_4c[i][j] = kmfit->pfit(i + Tracks1.size() + Showers1.size() + Tracks2.size())[j];
                    }
                    HepLorentzVector kpipp4_4c = kmfit->pfit(Tracks1.size() + Showers1.size() + Tracks2.size() + Showers2.size());
                    if (chisq_4c > 200) continue;
                    if (m_debug) std::cout << itag << ": chi2 4c" << std::endl;
                    besKD0p4 = kD0p4_4c;
                    besKDmp4 = kDmp4_4c;
                    besKpipp4 = kpipp4_4c;
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
                    vtx_tagPi[0] = fabs(vecipa[0]);
                    vtx_tagPi[1] = fabs(vecipa[3]);
                    vtx_tagPi[2] = cos(mdcTrk->theta());
                    HepLorentzVector ppi_tag;
                    ppi_tag.setPx(mdcTrk->p4(mpi)[0]);
                    ppi_tag.setPy(mdcTrk->p4(mpi)[1]);
                    ppi_tag.setPz(mdcTrk->p4(mpi)[2]);
                    ppi_tag.setE(mdcTrk->p4(mpi)[3]);
                    for (int k = 0; k < 4; k++) rawp4_tagPi[k] = mdcTrk->p4(mpi)[k];
                    if (runNo < 0) rawp4_tagPi[4] = MatchMC(ppi_tag, "pi_tag");
                    else rawp4_tagPi[4] = -999;
                    if (runNo < 0) rawp4_tagPi[5] = MatchMC(ppi_tag, "Dm_tag");
                    else rawp4_tagPi[5] = -999;
                    if (runNo < 0) rawp4_tagPi[6] = MatchMC(ppi_tag, "D0_tag");
                    else rawp4_tagPi[6] = -999;
                    SmartRefVector<EvtRecTrack> OtherShowers1 = tag1->otherShowers();
                    SmartRefVector<EvtRecTrack> OtherShowers2 = tag2->otherShowers();
                    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), "/Event/EvtRec/EvtRecEvent");
                    SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(),  EventModel::EvtRec::EvtRecTrackCol);
                    if (!evtRecTrkCol) {
                        std::cout << MSG::FATAL << "Could not find EvtRecTrackCol" << std::endl;
                        continue;
                    }
                    n_othershws = 0;
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
                        vtx_othershws[n_othershws][0] = eraw;
                        vtx_othershws[n_othershws][1] = phi;
                        vtx_othershws[n_othershws][2] = the;
                        vtx_othershws[n_othershws][3] = dang;
                        vtx_othershws[n_othershws][4] = module;
                        vtx_othershws[n_othershws][5] = Tdc;
                        Hep3Vector Gm_Mom = Gm_Vec - birth.vx();
                        Gm_Mom.setMag(gTrk->energy());
                        HepLorentzVector Gm_p4(Gm_Mom, gTrk->energy());
                        for (int m = 0; m < 4; m++) rawp4_othershws[n_othershws][m] = Gm_p4[m];
                        n_othershws++;
                        if (n_othershws >= 50) continue;
                        vwtrkpara_othershws.push_back(WTrackParameter(gTrk->position(), Gm_p4, gTrk->dphi(), gTrk->dtheta(), gTrk->dE()));
                    }
                    stat_fitpi0 = false;
                    stat_fitpi0 = fitpi0(vwtrkpara_othershws, birth);
                    if (!stat_fitpi0 && m_debug) std::cout << "Cannot find enough gamma to reconstruct pi0!" << std::endl;
                    D0_mode          = besttag1->decayMode();
                    D0_type          = besttag1->type();
                    D0_dcharge       = besttag1->charge();
                    D0_charm         = besttag1->charm();
                    D0_numofchildren = besttag1->numOfChildren();
                    D0_mass          = besttag1->mass();
                    Dm_mode          = besttag2->decayMode();
                    Dm_type          = besttag2->type();
                    Dm_dcharge       = besttag2->charge();
                    Dm_charm         = besttag2->charm();
                    Dm_numofchildren = besttag2->numOfChildren();
                    Dm_mass          = besttag2->mass();
	                p4_D0_4c[0] = besKD0p4.px();
                    p4_D0_4c[1] = besKD0p4.py();
                    p4_D0_4c[2] = besKD0p4.pz();
                    p4_D0_4c[3] = besKD0p4.e();
                    p4_Dm_4c[0] = besKDmp4.px();
                    p4_Dm_4c[1] = besKDmp4.py();
                    p4_Dm_4c[2] = besKDmp4.pz();
                    p4_Dm_4c[3] = besKDmp4.e();
                    p4_pip_4c[0] = besKpipp4.px();
                    p4_pip_4c[1] = besKpipp4.py();
                    p4_pip_4c[2] = besKpipp4.pz();
                    p4_pip_4c[3] = besKpipp4.e();

                    SmartRefVector<EvtRecTrack> tracks1 = besttag1->tracks();
                    SmartRefVector<EvtRecTrack> tracks2 = besttag2->tracks();
                    SmartRefVector<EvtRecTrack> showers1 = besttag1->showers();
                    SmartRefVector<EvtRecTrack> showers2 = besttag2->showers();

                    // Begain to fill the Kinematic Fitter
                    kmfit->init();
                    // Tracks of first D
                    for (int i = 0; i < tracks1.size(); i++) {
                        RecMdcKalTrack *kapiTrk = tracks1[i]->mdcKalTrack();
                        WTrackParameter wvkapiTrk = WTrackParameter(mPart_D0[besttag1->decayMode()][i], kapiTrk->getZHelix(), kapiTrk->getZError());
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
                        WTrackParameter wvkapiTrk = WTrackParameter(mPart_Dm[besttag2->decayMode() - 200][i], kapiTrk->getZHelix(), kapiTrk->getZError());
                        kmfit->AddTrack(i + tracks1.size() + showers1.size(), wvkapiTrk);
                    }
                    // Showers of secound D
                    for (int i = 0; i < showers2.size(); i++) {
                        RecEmcShower* gamShower = showers2[i]->emcShower();
                        kmfit->AddTrack(i + tracks1.size() + showers1.size() + tracks2.size(), 0.0, gamShower);
                    }
                    // The pion out of D 
                    WTrackParameter wvpiexTrk_6c = WTrackParameter(mpi, bestpiTrkex->getZHelix(), bestpiTrkex->getZError());
                    kmfit->AddTrack(tracks1.size() + showers1.size() + tracks2.size() + showers2.size(), wvpiexTrk_6c);
                    if (m_debug) {
                        std::cout << "Tag1: " << besttag1->decayMode() << " Track size: " << tracks1.size() << " shower size: " << showers1.size() << std::endl;
                        std::cout << "Tag2: " << besttag2->decayMode() << " Track size: " << tracks2.size() << " shower size: " << showers2.size() << std::endl;
                    }
                    // Add the mass constrain of D0 and D-
                    if (m_debug) std::cout << "Sample type: " << m_sample_types[mm] << ", Mean D0: " << MassD0Cand << ", Mean D-: " << MassDmCand << std::endl;
                    if (besttag1->decayMode() == 0)      kmfit->AddResonance(0, MassD0Cand, 0, 1);
                    else if (besttag1->decayMode() == 1) kmfit->AddResonance(0, MassD0Cand, 0, 1, 2, 3);
                    else if (besttag1->decayMode() == 3) kmfit->AddResonance(0, MassD0Cand, 0, 1, 2, 3);
                    else if (besttag1->decayMode() == 4) kmfit->AddResonance(0, MassD0Cand, 0, 1, 2, 3, 4, 5);
                    else std::cout << "Attention! You need add more mode!" << std::endl;
                    int nTr = tracks1.size() + showers1.size();
                    if (besttag2->decayMode() == 200)      kmfit->AddResonance(1, MassDmCand, nTr + 0, nTr + 1, nTr + 2);
                    else if (besttag2->decayMode() == 201) kmfit->AddResonance(1, MassDmCand, nTr + 0, nTr + 1, nTr + 2, nTr + 3, nTr + 4);
                    else if (besttag2->decayMode() == 202) kmfit->AddResonance(1, MassDmCand, nTr + 0, nTr + 1, nTr + 2);
                    else if (besttag2->decayMode() == 203) kmfit->AddResonance(1, MassDmCand, nTr + 0, nTr + 1, nTr + 2, nTr + 3, nTr + 4);
                    else if (besttag2->decayMode() == 204) kmfit->AddResonance(1, MassDmCand, nTr + 0, nTr + 1, nTr + 2, nTr + 3, nTr + 4);
                    else std::cout << "Attention! You need add more mode!" << std::endl;
                    kmfit->AddFourMomentum(2, ecms_lab);
                    bool oksq_6c = kmfit->Fit();
                    chisq_6c = kmfit->chisq();
                    if (m_debug) {
                        std::cout << "6C fit result: " << oksq_6c << " " << kmfit->chisq() << std::endl;
                    }
                    //Get the P4 of two D and pion from Fitter
                    HepLorentzVector kD0p4_6c = 0;
                    for (int i = 0; i < tracks1.size(); i++) {
                        kD0p4_6c += kmfit->pfit(i);
                        for (int j = 0; j < 4; j++) p4_D0trks_6c[i][j] = kmfit->pfit(i)[j];
                    }
                    for (int i = 0; i < showers1.size(); i++) {
                        kD0p4_6c += kmfit->pfit(i + tracks1.size());
                        for (int j = 0; j < 4; j++) p4_D0shws_6c[i][j] = kmfit->pfit(i + tracks1.size())[j];
                    }
                    HepLorentzVector kDmp4_6c = 0;
                    for (int i = 0; i < tracks2.size(); i++) {
                        kDmp4_6c += kmfit->pfit(i + tracks1.size() + showers1.size());
                        for (int j = 0; j < 4; j++) p4_Dmtrks_6c[i][j] = kmfit->pfit(i + tracks1.size() + showers1.size())[j];
                    }
                    for (int i = 0; i < showers2.size(); i++) {
                        kDmp4_6c += kmfit->pfit(i + tracks1.size() + showers1.size() + tracks2.size());
                        for (int j = 0; j < 4; j++) p4_Dmshws_6c[i][j] = kmfit->pfit(i + tracks1.size() + showers1.size() + tracks2.size())[j];
                    }
                    HepLorentzVector kpipp4_6c = kmfit->pfit(tracks1.size() + showers1.size() + tracks2.size() + showers2.size());
	                p4_D0_6c[0] = kD0p4_6c.px();
                    p4_D0_6c[1] = kD0p4_6c.py();
                    p4_D0_6c[2] = kD0p4_6c.pz();
                    p4_D0_6c[3] = kD0p4_6c.e();
                    p4_Dm_6c[0] = kDmp4_6c.px();
                    p4_Dm_6c[1] = kDmp4_6c.py();
                    p4_Dm_6c[2] = kDmp4_6c.pz();
                    p4_Dm_6c[3] = kDmp4_6c.e();
                    p4_pip_6c[0] = kpipp4_6c.px();
                    p4_pip_6c[1] = kpipp4_6c.py();
                    p4_pip_6c[2] = kpipp4_6c.pz();
                    p4_pip_6c[3] = kpipp4_6c.e();
                    recordVariables(m_sample_types[mm]);
                    dtagTool.clear();
                } // ***** tag1
            } //***** mode B
        } //***** mode A
    } //***** end of multi-samples
} //end of execute()

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
StatusCode PiD0Dm::finalize() {
    std::cout << "Total number:                " << Ncut0  << std::endl;
    std::cout << "DTagList is not Empty        " << Ncut1  << std::endl;
    std::cout << "Double tagD combinations     " << Ncut2  << std::endl;
    std::cout << "D0D-pi+ combinations         " << Ncut3  << std::endl;
    std::cout << "Pass Vertex fit              " << Ncut4  << std::endl;
    std::cout << "Pass 4c fit                  " << Ncut5  << std::endl;
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "in finalize()" << endmsg;
    return StatusCode::SUCCESS;
}

void PiD0Dm::recordVariables(std::string sample_type) {
    if (sample_type == "signal") {
        m_runNo_signal = runNo;
        m_evtNo_signal = evtNo;
        m_flag1_signal = flag1;
        m_flag2_signal = flag2;
        // save all McTruth info
        if (m_runNo_signal < 0) {
            m_idxmc_signal = idxmc;
            for (int i = 0; i< m_idxmc_signal; i++) {
                m_motherid_signal[i] = motherid[i];
                m_motheridx_signal[i] = motheridx[i];
                m_pdgid_signal[i] = pdgid[i];
                m_trkidx_signal[i] = trkidx[i];
            }
        }

        m_n_D0trks_signal = n_D0trks;
        for (int i = 0; i< m_n_D0trks_signal; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_signal[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_signal[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_signal[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_signal[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_signal = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_signal; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_signal[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_signal[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_signal[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_signal[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_signal = n_D0shws;
        for (int i = 0; i< m_n_D0shws_signal; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_signal[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_signal[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_signal[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_signal[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_signal = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_signal; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_signal[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_signal[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_signal[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_signal[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_signal          =  D0_mode         ;
        m_D0_type_signal          =  D0_type         ;
        m_D0_dcharge_signal       =  D0_dcharge      ;
        m_D0_charm_signal         =  D0_charm        ;
        m_D0_numofchildren_signal =  D0_numofchildren;
        m_D0_mass_signal          =  D0_mass         ;
        m_Dm_mode_signal          =  Dm_mode         ;
        m_Dm_type_signal          =  Dm_type         ;
        m_Dm_dcharge_signal       =  Dm_dcharge      ;
        m_Dm_charm_signal         =  Dm_charm        ;
        m_Dm_numofchildren_signal =  Dm_numofchildren;
        m_Dm_mass_signal          =  Dm_mass         ;
        m_chisq_4c_signal         =  chisq_4c        ;
        m_chisq_6c_signal         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_signal[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_signal[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_signal[i] = p4_pip_4c[i];
	        m_p4_D0_6c_signal[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_signal[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_signal[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_signal[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_signal[i] = rawp4_tagPi[i];
        m_n_othershws_signal = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_signal[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_signal[i][j] = rawp4_othershws[i][j];
        m_n_pi0_signal = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_signal[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_signal[i] = chi2_pi0[i];
        m_chisq_vf_signal = chisq_vf;

        m_chisq_svf_D0_signal = chisq_svf_D0;
        m_ctau_svf_D0_signal = ctau_svf_D0;
        m_L_svf_D0_signal = L_svf_D0;
        m_Lerr_svf_D0_signal = Lerr_svf_D0;
        m_chisq_svf_Dm_signal = chisq_svf_Dm;
        m_ctau_svf_Dm_signal = ctau_svf_Dm;
        m_L_svf_Dm_signal = L_svf_Dm;
        m_Lerr_svf_Dm_signal = Lerr_svf_Dm;

        if (sample_type == "signal") m_tuple_signal->write();
    }
    if (sample_type == "side1_low") {
        m_runNo_side1_low = runNo;
        m_evtNo_side1_low = evtNo;
        m_flag1_side1_low = flag1;
        m_flag2_side1_low = flag2;
        // save all McTruth info
        if (m_runNo_side1_low < 0) {
            m_idxmc_side1_low = idxmc;
            for (int i = 0; i< m_idxmc_side1_low; i++) {
                m_motherid_side1_low[i] = motherid[i];
                m_motheridx_side1_low[i] = motheridx[i];
                m_pdgid_side1_low[i] = pdgid[i];
                m_trkidx_side1_low[i] = trkidx[i];
            }
        }

        m_n_D0trks_side1_low = n_D0trks;
        for (int i = 0; i< m_n_D0trks_side1_low; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_side1_low[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_side1_low[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_side1_low[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_side1_low[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_side1_low = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_side1_low; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_side1_low[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_side1_low[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_side1_low[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_side1_low[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_side1_low = n_D0shws;
        for (int i = 0; i< m_n_D0shws_side1_low; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_side1_low[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_side1_low[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_side1_low[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_side1_low[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_side1_low = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_side1_low; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_side1_low[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_side1_low[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_side1_low[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_side1_low[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_side1_low          =  D0_mode         ;
        m_D0_type_side1_low          =  D0_type         ;
        m_D0_dcharge_side1_low       =  D0_dcharge      ;
        m_D0_charm_side1_low         =  D0_charm        ;
        m_D0_numofchildren_side1_low =  D0_numofchildren;
        m_D0_mass_side1_low          =  D0_mass         ;
        m_Dm_mode_side1_low          =  Dm_mode         ;
        m_Dm_type_side1_low          =  Dm_type         ;
        m_Dm_dcharge_side1_low       =  Dm_dcharge      ;
        m_Dm_charm_side1_low         =  Dm_charm        ;
        m_Dm_numofchildren_side1_low =  Dm_numofchildren;
        m_Dm_mass_side1_low          =  Dm_mass         ;
        m_chisq_4c_side1_low         =  chisq_4c        ;
        m_chisq_6c_side1_low         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_side1_low[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_side1_low[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_side1_low[i] = p4_pip_4c[i];
	        m_p4_D0_6c_side1_low[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_side1_low[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_side1_low[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_side1_low[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_side1_low[i] = rawp4_tagPi[i];
        m_n_othershws_side1_low = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_side1_low[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_side1_low[i][j] = rawp4_othershws[i][j];
        m_n_pi0_side1_low = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_side1_low[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_side1_low[i] = chi2_pi0[i];
        m_chisq_vf_side1_low = chisq_vf;

        m_chisq_svf_D0_side1_low = chisq_svf_D0;
        m_ctau_svf_D0_side1_low = ctau_svf_D0;
        m_L_svf_D0_side1_low = L_svf_D0;
        m_Lerr_svf_D0_side1_low = Lerr_svf_D0;
        m_chisq_svf_Dm_side1_low = chisq_svf_Dm;
        m_ctau_svf_Dm_side1_low = ctau_svf_Dm;
        m_L_svf_Dm_side1_low = L_svf_Dm;
        m_Lerr_svf_Dm_side1_low = Lerr_svf_Dm;

        if (sample_type == "side1_low") m_tuple_side1_low->write();
    }
    if (sample_type == "side1_up") {
        m_runNo_side1_up = runNo;
        m_evtNo_side1_up = evtNo;
        m_flag1_side1_up = flag1;
        m_flag2_side1_up = flag2; 
        // save all McTruth info
        if (m_runNo_side1_up < 0) {
            m_idxmc_side1_up = idxmc;
            for (int i = 0; i< m_idxmc_side1_up; i++) {
                m_motherid_side1_up[i] = motherid[i];
                m_motheridx_side1_up[i] = motheridx[i];
                m_pdgid_side1_up[i] = pdgid[i];
                m_trkidx_side1_up[i] = trkidx[i];
            }
        }

        m_n_D0trks_side1_up = n_D0trks;
        for (int i = 0; i< m_n_D0trks_side1_up; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_side1_up[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_side1_up[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_side1_up[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_side1_up[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_side1_up = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_side1_up; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_side1_up[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_side1_up[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_side1_up[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_side1_up[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_side1_up = n_D0shws;
        for (int i = 0; i< m_n_D0shws_side1_up; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_side1_up[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_side1_up[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_side1_up[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_side1_up[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_side1_up = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_side1_up; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_side1_up[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_side1_up[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_side1_up[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_side1_up[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_side1_up          =  D0_mode         ;
        m_D0_type_side1_up          =  D0_type         ;
        m_D0_dcharge_side1_up       =  D0_dcharge      ;
        m_D0_charm_side1_up         =  D0_charm        ;
        m_D0_numofchildren_side1_up =  D0_numofchildren;
        m_D0_mass_side1_up          =  D0_mass         ;
        m_Dm_mode_side1_up          =  Dm_mode         ;
        m_Dm_type_side1_up          =  Dm_type         ;
        m_Dm_dcharge_side1_up       =  Dm_dcharge      ;
        m_Dm_charm_side1_up         =  Dm_charm        ;
        m_Dm_numofchildren_side1_up =  Dm_numofchildren;
        m_Dm_mass_side1_up          =  Dm_mass         ;
        m_chisq_4c_side1_up         =  chisq_4c        ;
        m_chisq_6c_side1_up         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_side1_up[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_side1_up[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_side1_up[i] = p4_pip_4c[i];
	        m_p4_D0_6c_side1_up[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_side1_up[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_side1_up[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_side1_up[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_side1_up[i] = rawp4_tagPi[i];
        m_n_othershws_side1_up = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_side1_up[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_side1_up[i][j] = rawp4_othershws[i][j];
        m_n_pi0_side1_up = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_side1_up[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_side1_up[i] = chi2_pi0[i];
        m_chisq_vf_side1_up = chisq_vf;

        m_chisq_svf_D0_side1_up = chisq_svf_D0;
        m_ctau_svf_D0_side1_up = ctau_svf_D0;
        m_L_svf_D0_side1_up = L_svf_D0;
        m_Lerr_svf_D0_side1_up = Lerr_svf_D0;
        m_chisq_svf_Dm_side1_up = chisq_svf_Dm;
        m_ctau_svf_Dm_side1_up = ctau_svf_Dm;
        m_L_svf_Dm_side1_up = L_svf_Dm;
        m_Lerr_svf_Dm_side1_up = Lerr_svf_Dm;

        if (sample_type == "side1_up") m_tuple_side1_up->write();
    }
    if (sample_type == "side2_low") {
        m_runNo_side2_low = runNo;
        m_evtNo_side2_low = evtNo;
        m_flag1_side2_low = flag1;
        m_flag2_side2_low = flag2;
        // save all McTruth info
        if (m_runNo_side2_low < 0) {
            m_idxmc_side2_low = idxmc;
            for (int i = 0; i< m_idxmc_side2_low; i++) {
                m_motherid_side2_low[i] = motherid[i];
                m_motheridx_side2_low[i] = motheridx[i];
                m_pdgid_side2_low[i] = pdgid[i];
                m_trkidx_side2_low[i] = trkidx[i];
            }
        }

        m_n_D0trks_side2_low = n_D0trks;
        for (int i = 0; i< m_n_D0trks_side2_low; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_side2_low[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_side2_low[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_side2_low[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_side2_low[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_side2_low = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_side2_low; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_side2_low[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_side2_low[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_side2_low[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_side2_low[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_side2_low = n_D0shws;
        for (int i = 0; i< m_n_D0shws_side2_low; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_side2_low[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_side2_low[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_side2_low[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_side2_low[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_side2_low = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_side2_low; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_side2_low[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_side2_low[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_side2_low[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_side2_low[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_side2_low          =  D0_mode         ;
        m_D0_type_side2_low          =  D0_type         ;
        m_D0_dcharge_side2_low       =  D0_dcharge      ;
        m_D0_charm_side2_low         =  D0_charm        ;
        m_D0_numofchildren_side2_low =  D0_numofchildren;
        m_D0_mass_side2_low          =  D0_mass         ;
        m_Dm_mode_side2_low          =  Dm_mode         ;
        m_Dm_type_side2_low          =  Dm_type         ;
        m_Dm_dcharge_side2_low       =  Dm_dcharge      ;
        m_Dm_charm_side2_low         =  Dm_charm        ;
        m_Dm_numofchildren_side2_low =  Dm_numofchildren;
        m_Dm_mass_side2_low          =  Dm_mass         ;
        m_chisq_4c_side2_low         =  chisq_4c        ;
        m_chisq_6c_side2_low         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_side2_low[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_side2_low[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_side2_low[i] = p4_pip_4c[i];
	        m_p4_D0_6c_side2_low[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_side2_low[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_side2_low[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_side2_low[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_side2_low[i] = rawp4_tagPi[i];
        m_n_othershws_side2_low = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_side2_low[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_side2_low[i][j] = rawp4_othershws[i][j];
        m_n_pi0_side2_low = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_side2_low[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_side2_low[i] = chi2_pi0[i];
        m_chisq_vf_side2_low = chisq_vf;

        m_chisq_svf_D0_side2_low = chisq_svf_D0;
        m_ctau_svf_D0_side2_low = ctau_svf_D0;
        m_L_svf_D0_side2_low = L_svf_D0;
        m_Lerr_svf_D0_side2_low = Lerr_svf_D0;
        m_chisq_svf_Dm_side2_low = chisq_svf_Dm;
        m_ctau_svf_Dm_side2_low = ctau_svf_Dm;
        m_L_svf_Dm_side2_low = L_svf_Dm;
        m_Lerr_svf_Dm_side2_low = Lerr_svf_Dm;

        if (sample_type == "side2_low") m_tuple_side2_low->write();
    }
    if (sample_type == "side2_up") {
        m_runNo_side2_up = runNo;
        m_evtNo_side2_up = evtNo;
        m_flag1_side2_up = flag1;
        m_flag2_side2_up = flag2;
        // save all McTruth info
        if (m_runNo_side2_up < 0) {
            m_idxmc_side2_up = idxmc;
            for (int i = 0; i< m_idxmc_side2_up; i++) {
                m_motherid_side2_up[i] = motherid[i];
                m_motheridx_side2_up[i] = motheridx[i];
                m_pdgid_side2_up[i] = pdgid[i];
                m_trkidx_side2_up[i] = trkidx[i];
            }
        }

        m_n_D0trks_side2_up = n_D0trks;
        for (int i = 0; i< m_n_D0trks_side2_up; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_side2_up[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_side2_up[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_side2_up[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_side2_up[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_side2_up = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_side2_up; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_side2_up[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_side2_up[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_side2_up[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_side2_up[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_side2_up = n_D0shws;
        for (int i = 0; i< m_n_D0shws_side2_up; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_side2_up[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_side2_up[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_side2_up[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_side2_up[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_side2_up = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_side2_up; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_side2_up[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_side2_up[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_side2_up[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_side2_up[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_side2_up          =  D0_mode         ;
        m_D0_type_side2_up          =  D0_type         ;
        m_D0_dcharge_side2_up       =  D0_dcharge      ;
        m_D0_charm_side2_up         =  D0_charm        ;
        m_D0_numofchildren_side2_up =  D0_numofchildren;
        m_D0_mass_side2_up          =  D0_mass         ;
        m_Dm_mode_side2_up          =  Dm_mode         ;
        m_Dm_type_side2_up          =  Dm_type         ;
        m_Dm_dcharge_side2_up       =  Dm_dcharge      ;
        m_Dm_charm_side2_up         =  Dm_charm        ;
        m_Dm_numofchildren_side2_up =  Dm_numofchildren;
        m_Dm_mass_side2_up          =  Dm_mass         ;
        m_chisq_4c_side2_up         =  chisq_4c        ;
        m_chisq_6c_side2_up         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_side2_up[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_side2_up[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_side2_up[i] = p4_pip_4c[i];
	        m_p4_D0_6c_side2_up[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_side2_up[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_side2_up[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_side2_up[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_side2_up[i] = rawp4_tagPi[i];
        m_n_othershws_side2_up = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_side2_up[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_side2_up[i][j] = rawp4_othershws[i][j];
        m_n_pi0_side2_up = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_side2_up[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_side2_up[i] = chi2_pi0[i];
        m_chisq_vf_side2_up = chisq_vf;

        m_chisq_svf_D0_side2_up = chisq_svf_D0;
        m_ctau_svf_D0_side2_up = ctau_svf_D0;
        m_L_svf_D0_side2_up = L_svf_D0;
        m_Lerr_svf_D0_side2_up = Lerr_svf_D0;
        m_chisq_svf_Dm_side2_up = chisq_svf_Dm;
        m_ctau_svf_Dm_side2_up = ctau_svf_Dm;
        m_L_svf_Dm_side2_up = L_svf_Dm;
        m_Lerr_svf_Dm_side2_up = Lerr_svf_Dm;

        if (sample_type == "side2_up") m_tuple_side2_up->write();
    }
    if (sample_type == "side3_low") {
        m_runNo_side3_low = runNo;
        m_evtNo_side3_low = evtNo;
        m_flag1_side3_low = flag1;
        m_flag2_side3_low = flag2;
        // save all McTruth info
        if (m_runNo_side3_low < 0) {
            m_idxmc_side3_low = idxmc;
            for (int i = 0; i< m_idxmc_side3_low; i++) {
                m_motherid_side3_low[i] = motherid[i];
                m_motheridx_side3_low[i] = motheridx[i];
                m_pdgid_side3_low[i] = pdgid[i];
                m_trkidx_side3_low[i] = trkidx[i];
            }
        }

        m_n_D0trks_side3_low = n_D0trks;
        for (int i = 0; i< m_n_D0trks_side3_low; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_side3_low[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_side3_low[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_side3_low[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_side3_low[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_side3_low = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_side3_low; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_side3_low[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_side3_low[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_side3_low[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_side3_low[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_side3_low = n_D0shws;
        for (int i = 0; i< m_n_D0shws_side3_low; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_side3_low[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_side3_low[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_side3_low[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_side3_low[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_side3_low = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_side3_low; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_side3_low[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_side3_low[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_side3_low[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_side3_low[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_side3_low          =  D0_mode         ;
        m_D0_type_side3_low          =  D0_type         ;
        m_D0_dcharge_side3_low       =  D0_dcharge      ;
        m_D0_charm_side3_low         =  D0_charm        ;
        m_D0_numofchildren_side3_low =  D0_numofchildren;
        m_D0_mass_side3_low          =  D0_mass         ;
        m_Dm_mode_side3_low          =  Dm_mode         ;
        m_Dm_type_side3_low          =  Dm_type         ;
        m_Dm_dcharge_side3_low       =  Dm_dcharge      ;
        m_Dm_charm_side3_low         =  Dm_charm        ;
        m_Dm_numofchildren_side3_low =  Dm_numofchildren;
        m_Dm_mass_side3_low          =  Dm_mass         ;
        m_chisq_4c_side3_low         =  chisq_4c        ;
        m_chisq_6c_side3_low         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_side3_low[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_side3_low[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_side3_low[i] = p4_pip_4c[i];
	        m_p4_D0_6c_side3_low[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_side3_low[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_side3_low[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_side3_low[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_side3_low[i] = rawp4_tagPi[i];
        m_n_othershws_side3_low = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_side3_low[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_side3_low[i][j] = rawp4_othershws[i][j];
        m_n_pi0_side3_low = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_side3_low[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_side3_low[i] = chi2_pi0[i];
        m_chisq_vf_side3_low = chisq_vf;

        m_chisq_svf_D0_side3_low = chisq_svf_D0;
        m_ctau_svf_D0_side3_low = ctau_svf_D0;
        m_L_svf_D0_side3_low = L_svf_D0;
        m_Lerr_svf_D0_side3_low = Lerr_svf_D0;
        m_chisq_svf_Dm_side3_low = chisq_svf_Dm;
        m_ctau_svf_Dm_side3_low = ctau_svf_Dm;
        m_L_svf_Dm_side3_low = L_svf_Dm;
        m_Lerr_svf_Dm_side3_low = Lerr_svf_Dm;

        if (sample_type == "side3_low") m_tuple_side3_low->write();
    }
    if (sample_type == "side3_up") {
        m_runNo_side3_up = runNo;
        m_evtNo_side3_up = evtNo;
        m_flag1_side3_up = flag1;
        m_flag2_side3_up = flag2;
        // save all McTruth info
        if (m_runNo_side3_up < 0) {
            m_idxmc_side3_up = idxmc;
            for (int i = 0; i< m_idxmc_side3_up; i++) {
                m_motherid_side3_up[i] = motherid[i];
                m_motheridx_side3_up[i] = motheridx[i];
                m_pdgid_side3_up[i] = pdgid[i];
                m_trkidx_side3_up[i] = trkidx[i];
            }
        }

        m_n_D0trks_side3_up = n_D0trks;
        for (int i = 0; i< m_n_D0trks_side3_up; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_side3_up[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_side3_up[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_side3_up[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_side3_up[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_side3_up = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_side3_up; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_side3_up[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_side3_up[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_side3_up[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_side3_up[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_side3_up = n_D0shws;
        for (int i = 0; i< m_n_D0shws_side3_up; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_side3_up[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_side3_up[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_side3_up[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_side3_up[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_side3_up = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_side3_up; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_side3_up[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_side3_up[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_side3_up[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_side3_up[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_side3_up          =  D0_mode         ;
        m_D0_type_side3_up          =  D0_type         ;
        m_D0_dcharge_side3_up       =  D0_dcharge      ;
        m_D0_charm_side3_up         =  D0_charm        ;
        m_D0_numofchildren_side3_up =  D0_numofchildren;
        m_D0_mass_side3_up          =  D0_mass         ;
        m_Dm_mode_side3_up          =  Dm_mode         ;
        m_Dm_type_side3_up          =  Dm_type         ;
        m_Dm_dcharge_side3_up       =  Dm_dcharge      ;
        m_Dm_charm_side3_up         =  Dm_charm        ;
        m_Dm_numofchildren_side3_up =  Dm_numofchildren;
        m_Dm_mass_side3_up          =  Dm_mass         ;
        m_chisq_4c_side3_up         =  chisq_4c        ;
        m_chisq_6c_side3_up         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_side3_up[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_side3_up[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_side3_up[i] = p4_pip_4c[i];
	        m_p4_D0_6c_side3_up[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_side3_up[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_side3_up[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_side3_up[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_side3_up[i] = rawp4_tagPi[i];
        m_n_othershws_side3_up = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_side3_up[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_side3_up[i][j] = rawp4_othershws[i][j];
        m_n_pi0_side3_up = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_side3_up[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_side3_up[i] = chi2_pi0[i];
        m_chisq_vf_side3_up = chisq_vf;

        m_chisq_svf_D0_side3_up = chisq_svf_D0;
        m_ctau_svf_D0_side3_up = ctau_svf_D0;
        m_L_svf_D0_side3_up = L_svf_D0;
        m_Lerr_svf_D0_side3_up = Lerr_svf_D0;
        m_chisq_svf_Dm_side3_up = chisq_svf_Dm;
        m_ctau_svf_Dm_side3_up = ctau_svf_Dm;
        m_L_svf_Dm_side3_up = L_svf_Dm;
        m_Lerr_svf_Dm_side3_up = Lerr_svf_Dm;

        if (sample_type == "side3_up") m_tuple_side3_up->write();
    }
    if (sample_type == "side4_low") {
        m_runNo_side4_low = runNo;
        m_evtNo_side4_low = evtNo;
        m_flag1_side4_low = flag1;
        m_flag2_side4_low = flag2;
        // save all McTruth info
        if (m_runNo_side4_low < 0) {
            m_idxmc_side4_low = idxmc;
            for (int i = 0; i< m_idxmc_side4_low; i++) {
                m_motherid_side4_low[i] = motherid[i];
                m_motheridx_side4_low[i] = motheridx[i];
                m_pdgid_side4_low[i] = pdgid[i];
                m_trkidx_side4_low[i] = trkidx[i];
            }
        }

        m_n_D0trks_side4_low = n_D0trks;
        for (int i = 0; i< m_n_D0trks_side4_low; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_side4_low[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_side4_low[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_side4_low[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_side4_low[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_side4_low = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_side4_low; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_side4_low[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_side4_low[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_side4_low[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_side4_low[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_side4_low = n_D0shws;
        for (int i = 0; i< m_n_D0shws_side4_low; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_side4_low[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_side4_low[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_side4_low[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_side4_low[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_side4_low = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_side4_low; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_side4_low[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_side4_low[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_side4_low[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_side4_low[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_side4_low          =  D0_mode         ;
        m_D0_type_side4_low          =  D0_type         ;
        m_D0_dcharge_side4_low       =  D0_dcharge      ;
        m_D0_charm_side4_low         =  D0_charm        ;
        m_D0_numofchildren_side4_low =  D0_numofchildren;
        m_D0_mass_side4_low          =  D0_mass         ;
        m_Dm_mode_side4_low          =  Dm_mode         ;
        m_Dm_type_side4_low          =  Dm_type         ;
        m_Dm_dcharge_side4_low       =  Dm_dcharge      ;
        m_Dm_charm_side4_low         =  Dm_charm        ;
        m_Dm_numofchildren_side4_low =  Dm_numofchildren;
        m_Dm_mass_side4_low          =  Dm_mass         ;
        m_chisq_4c_side4_low         =  chisq_4c        ;
        m_chisq_6c_side4_low         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_side4_low[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_side4_low[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_side4_low[i] = p4_pip_4c[i];
	        m_p4_D0_6c_side4_low[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_side4_low[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_side4_low[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_side4_low[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_side4_low[i] = rawp4_tagPi[i];
        m_n_othershws_side4_low = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_side4_low[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_side4_low[i][j] = rawp4_othershws[i][j];
        m_n_pi0_side4_low = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_side4_low[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_side4_low[i] = chi2_pi0[i];
        m_chisq_vf_side4_low = chisq_vf;

        m_chisq_svf_D0_side4_low = chisq_svf_D0;
        m_ctau_svf_D0_side4_low = ctau_svf_D0;
        m_L_svf_D0_side4_low = L_svf_D0;
        m_Lerr_svf_D0_side4_low = Lerr_svf_D0;
        m_chisq_svf_Dm_side4_low = chisq_svf_Dm;
        m_ctau_svf_Dm_side4_low = ctau_svf_Dm;
        m_L_svf_Dm_side4_low = L_svf_Dm;
        m_Lerr_svf_Dm_side4_low = Lerr_svf_Dm;

        if (sample_type == "side4_low") m_tuple_side4_low->write();
    }
    if (sample_type == "side4_up") {
        m_runNo_side4_up = runNo;
        m_evtNo_side4_up = evtNo;
        m_flag1_side4_up = flag1;
        m_flag2_side4_up = flag2;
        // save all McTruth info
        if (m_runNo_side4_up < 0) {
            m_idxmc_side4_up = idxmc;
            for (int i = 0; i< m_idxmc_side4_up; i++) {
                m_motherid_side4_up[i] = motherid[i];
                m_motheridx_side4_up[i] = motheridx[i];
                m_pdgid_side4_up[i] = pdgid[i];
                m_trkidx_side4_up[i] = trkidx[i];
            }
        }

        m_n_D0trks_side4_up = n_D0trks;
        for (int i = 0; i< m_n_D0trks_side4_up; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_D0trks_side4_up[i][j] = rawp4_D0trks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_D0trks_side4_up[i][j] = vtx_D0trks[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_4c_side4_up[i][j] = p4_D0trks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0trks_6c_side4_up[i][j] = p4_D0trks_6c[i][j];
        }

        m_n_Dmtrks_side4_up = n_Dmtrks;
        for (int i = 0; i< m_n_Dmtrks_side4_up; i++) {
            for (int j = 0; j < 8; j++) m_rawp4_Dmtrks_side4_up[i][j] = rawp4_Dmtrks[i][j];
            for (int j = 0; j < 3; j++) m_vtx_Dmtrks_side4_up[i][j] = vtx_Dmtrks[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_4c_side4_up[i][j] = p4_Dmtrks_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmtrks_6c_side4_up[i][j] = p4_Dmtrks_6c[i][j];
        }

        m_n_D0shws_side4_up = n_D0shws;
        for (int i = 0; i< m_n_D0shws_side4_up; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_D0shws_side4_up[i][j] = rawp4_D0shws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_D0shws_side4_up[i][j] = vtx_D0shws[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_4c_side4_up[i][j] = p4_D0shws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_D0shws_6c_side4_up[i][j] = p4_D0shws_6c[i][j];
        }

        m_n_Dmshws_side4_up = n_Dmshws;
        for (int i = 0; i< m_n_Dmshws_side4_up; i++) {
            for (int j = 0; j < 6; j++) m_rawp4_Dmshws_side4_up[i][j] = rawp4_Dmshws[i][j];
            for (int j = 0; j < 6; j++) m_vtx_Dmshws_side4_up[i][j] = vtx_Dmshws[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_4c_side4_up[i][j] = p4_Dmshws_4c[i][j];
            for (int j = 0; j < 4; j++) m_p4_Dmshws_6c_side4_up[i][j] = p4_Dmshws_6c[i][j];
        }

        m_D0_mode_side4_up          =  D0_mode         ;
        m_D0_type_side4_up          =  D0_type         ;
        m_D0_dcharge_side4_up       =  D0_dcharge      ;
        m_D0_charm_side4_up         =  D0_charm        ;
        m_D0_numofchildren_side4_up =  D0_numofchildren;
        m_D0_mass_side4_up          =  D0_mass         ;
        m_Dm_mode_side4_up          =  Dm_mode         ;
        m_Dm_type_side4_up          =  Dm_type         ;
        m_Dm_dcharge_side4_up       =  Dm_dcharge      ;
        m_Dm_charm_side4_up         =  Dm_charm        ;
        m_Dm_numofchildren_side4_up =  Dm_numofchildren;
        m_Dm_mass_side4_up          =  Dm_mass         ;
        m_chisq_4c_side4_up         =  chisq_4c        ;
        m_chisq_6c_side4_up         =  chisq_6c        ;
        for (int i = 0; i < 4; i++) {
	        m_p4_D0_4c_side4_up[i] = p4_D0_4c[i];
	        m_p4_Dm_4c_side4_up[i] = p4_Dm_4c[i];
	        m_p4_pip_4c_side4_up[i] = p4_pip_4c[i];
	        m_p4_D0_6c_side4_up[i] = p4_D0_6c[i];
	        m_p4_Dm_6c_side4_up[i] = p4_Dm_6c[i];
	        m_p4_pip_6c_side4_up[i] = p4_pip_6c[i];
        }

        for (int i = 0; i < 3; i++) m_vtx_tagPi_side4_up[i] = vtx_tagPi[i];
        for (int i = 0; i < 7; i++) m_rawp4_tagPi_side4_up[i] = rawp4_tagPi[i];
        m_n_othershws_side4_up = n_othershws;
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 6; j++) m_vtx_othershws_side4_up[i][j] = vtx_othershws[i][j];
        for (int i = 0; i < n_othershws; i++) for (int j = 0; j < 4; j++) m_rawp4_othershws_side4_up[i][j] = rawp4_othershws[i][j];
        m_n_pi0_side4_up = n_pi0;
        for (int i = 0; i < n_pi0; i++) for (int j = 0; j < 4; j++) m_p4_pi0_side4_up[i][j] = p4_pi0[i][j];
        for (int i = 0; i < n_pi0; i++) m_chi2_pi0_side4_up[i] = chi2_pi0[i];
        m_chisq_vf_side4_up = chisq_vf;

        m_chisq_svf_D0_side4_up = chisq_svf_D0;
        m_ctau_svf_D0_side4_up = ctau_svf_D0;
        m_L_svf_D0_side4_up = L_svf_D0;
        m_Lerr_svf_D0_side4_up = Lerr_svf_D0;
        m_chisq_svf_Dm_side4_up = chisq_svf_Dm;
        m_ctau_svf_Dm_side4_up = ctau_svf_Dm;
        m_L_svf_Dm_side4_up = L_svf_Dm;
        m_Lerr_svf_Dm_side4_up = Lerr_svf_Dm;

        if (sample_type == "side4_up") m_tuple_side4_up->write();
    }
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
    n_pi0 = 0;
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
                HepLorentzVector ppi0 = kmfit->pfit(0) + kmfit->pfit(1);
                chi2_pi0[n_pi0] = chi2;
                for (int k = 0; k < 4; k++) p4_pi0[n_pi0][k] = ppi0[k];
                n_pi0++;
            }
            else {
                continue;
            }
        }
    }
    return true;
}

int PiD0Dm::MatchMC(HepLorentzVector &p4, std::string MODE) {
    SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");
    if (!mcParticleCol) {
        return -999;
    } else {
        Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
        double clst_ang = 999.;
        Event::McParticle* clst_particle;
        for (; iter_mc != mcParticleCol->end(); iter_mc++) {
            if (!(*iter_mc)->decayFromGenerator())  continue;
            double pid_cand = (*iter_mc)->particleProperty();
            if (!(pid_cand == 211 || pid_cand == -211 || pid_cand == 321 || pid_cand == -321 || pid_cand == 22)) continue;
            double ang = p4.angle((*iter_mc)->initialFourMomentum());
            if (clst_ang > ang) {
                clst_ang = ang;
                clst_particle = (*iter_mc);
            }
        }
        if (clst_ang < 999) {
            Event::McParticle mom = clst_particle->mother();
            int pid_mom = mom.particleProperty();
            Event::McParticle grandmom = mom.mother();
            int pid_grandmom = grandmom.particleProperty();
            if (MODE == "Dm_tag" && (fabs(pid_mom) == 411 || (pid_mom == 310 && fabs(pid_grandmom) == 411) || (pid_mom == 111 && fabs(pid_grandmom) == 411))) {
                return 1;
            } 
            if (MODE == "D0_tag" && (fabs(pid_mom) == 421 || (pid_mom == 111 && fabs(pid_grandmom) == 421))) {
                return 1;
            } 
            if (MODE == "Dm_tag" && !(fabs(pid_mom) == 411 || (pid_mom == 310 && fabs(pid_grandmom) == 411) || (pid_mom == 111 && fabs(pid_grandmom) == 411))) {
                return 0;
            } 
            if (MODE == "D0_tag" && !(fabs(pid_mom) == 421 || (pid_mom == 111 && fabs(pid_grandmom) == 421))) {
                return 0;
            } 
            if (MODE == "pi_tag" && (pid_mom == 9020443 || pid_mom == 9030443 || pid_mom == 90022 || pid_mom == 80022 || fabs(pid_mom) == 10413 || (fabs(pid_mom) == 211 && fabs(pid_grandmom) == 211))) {
                return 1;
            } 
            if (MODE == "pi_tag" && !(pid_mom == 9020443 || pid_mom == 9030443 || pid_mom == 90022 || pid_mom == 80022 || fabs(pid_mom) == 10413 || (fabs(pid_mom) == 211 && fabs(pid_grandmom) == 211))) {
                return 0;
            }
        }
    }
    return 0;
}

bool PiD0Dm::fitSecondVertex(VWTrkPara &vwtrkpara) {
    matrix_svf[0] = 999.;
    matrix_svf[1] = 999.;
    matrix_svf[2] = 999.;
    matrix_svf[3] = 999.;
    Hep3Vector ip(0, 0, 0);
    HepSymMatrix ipEx(3, 0);
    IVertexDbSvc* vtxsvc;
    Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
    if (vtxsvc->isVertexValid()) {
        double* dbv = vtxsvc->PrimaryVertex();
        double* vv = vtxsvc->SigmaPrimaryVertex();
        ip.setX(dbv[0]);
        ip.setY(dbv[1]);
        ip.setZ(dbv[2]);
        ipEx[0][0] = vv[0] * vv[0];
        ipEx[1][1] = vv[1] * vv[1];
        ipEx[2][2] = vv[2] * vv[2];
    }
    else return false;
    VertexParameter bs;
    bs.setVx(ip);
    bs.setEvx(ipEx);
    HepPoint3D vx(0., 0., 0.);
    HepSymMatrix Evx(3, 0);
    double bx = 1E+6;
    double by = 1E+6;
    double bz = 1E+6;
    Evx[0][0] = bx * bx;
    Evx[1][1] = by * by;
    Evx[2][2] = bz * bz;
    // vertex fit
    VertexParameter vxpar;
    vxpar.setVx(vx);
    vxpar.setEvx(Evx);
    VertexFit *vtxfit = VertexFit::instance();
    vtxfit->init();
    for (int i = 0; i < vwtrkpara.size(); i++) vtxfit->AddTrack(i, vwtrkpara[i]);
    Vint trkId(vwtrkpara.size(), 0);
    for (int i = 0; i < vwtrkpara.size(); i++) trkId[i] = i;
    vtxfit->AddVertex(0, vxpar, trkId);
    if (!(vtxfit->Fit(0))) return false;
    vtxfit->Swim(0);
    vtxfit->BuildVirtualParticle(0);
    // second vertex fit
    SecondVertexFit *svtxfit = SecondVertexFit::instance();
    svtxfit->init();
    svtxfit->setPrimaryVertex(bs);
    svtxfit->AddTrack(0, vtxfit->wVirtualTrack(0));
    svtxfit->setVpar(vtxfit->vpar(0));
    if (!svtxfit->Fit()) return false;
    matrix_svf[0] = svtxfit->chisq();
    matrix_svf[1] = svtxfit->ctau();
    matrix_svf[2] = svtxfit->decayLength();
    matrix_svf[3] = svtxfit->decayLengthError();
    return true;
}
