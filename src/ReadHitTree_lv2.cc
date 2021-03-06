#include "ReadHitTree_lv2.hh"

using namespace anl;

namespace hittree_gen 
{

ReadHitTree_lv2::ReadHitTree_lv2()
  : m_EventID(0),
    eventDataBuffer_(nullptr)
{
  add_alias("ReadHitTree_lv2");
}

ANLStatus ReadHitTree_lv2::mod_startup()
{
  register_parameter(&m_FileList, "file_list");
  return AS_OK;
}

ANLStatus ReadHitTree_lv2::mod_init()
{
  m_FileIterator = m_FileList.begin();
  GetANLModule("EventDataBuffer", &eventDataBuffer_);
  eventData = eventDataBuffer_->getEventData_lv2();

  m_Tree = new TChain("hittree_lv2");
  while (!wasLastFile()) {
    m_Tree->Add(nextFile().c_str());
  }

  m_Tree->SetBranchAddress("nsignal_lv2", &nsignal);
  m_Tree->SetBranchAddress("detid_lv2", detid);
  m_Tree->SetBranchAddress("detector_material_lv2", detector_material);
  m_Tree->SetBranchAddress("detector_HV_lv2", detector_HV);
  m_Tree->SetBranchAddress("n_merged_signal_lv2", n_merged_signal);
  m_Tree->SetBranchAddress("pos_x_lv2", pos_x);
  m_Tree->SetBranchAddress("pos_y_lv2", pos_y);
  m_Tree->SetBranchAddress("pos_z_lv2", pos_z);
  m_Tree->SetBranchAddress("delta_x_lv2", delta_x);
  m_Tree->SetBranchAddress("delta_y_lv2", delta_y);
  m_Tree->SetBranchAddress("delta_z_lv2", delta_z);
  m_Tree->SetBranchAddress("epi_lv2", epi);
  
  m_NEvents = m_Tree->GetEntries();
  std::cout << "Total Event : " << m_NEvents << std::endl;

  return AS_OK;
}

ANLStatus ReadHitTree_lv2::mod_ana()
{
  ++m_EventID;
  static unsigned int iEvent = 0;
  if(iEvent == m_NEvents) {
    return AS_QUIT;
  }

  eventData->ResetData();

  m_Tree->GetEntry(iEvent);

  eventData->set_nsignal(nsignal);
  for(int isignal = 0; isignal < nsignal; ++isignal){
    eventData->set_detid(detid[isignal]);
    eventData->set_detector_material(detector_material[isignal]);
    eventData->set_detector_HV(detector_HV[isignal]);
    eventData->set_n_merged_signal(n_merged_signal[isignal]);
    eventData->set_pos_x(pos_x[isignal]);
    eventData->set_pos_y(pos_y[isignal]);
    eventData->set_pos_z(pos_z[isignal]);
    eventData->set_delta_x(delta_x[isignal]);
    eventData->set_delta_y(delta_y[isignal]);
    eventData->set_delta_z(delta_z[isignal]);
    eventData->set_epi(epi[isignal]);
  }
  
  iEvent++;
  return AS_OK;
}

bool ReadHitTree_lv2::checkFiles()
{
  for (auto& filename: m_FileList) {
    std::ifstream fin;
    fin.open( filename.c_str() );
    if (!fin) {
      std::cout << "ReadHitTree_lv2: cannot open " << filename << std::endl;
      return false;
    }
    fin.close();
  }
  return true;
}

} /* namespace hittree_gen*/
