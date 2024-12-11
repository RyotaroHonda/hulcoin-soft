#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <map>

#include "DwgFuncs.hh"
#include "UDPRBCP.hh"
#include "FPGAModule.hh"

enum argIndex{kBin, kIp, kRegFile};
using namespace HUL;
//using namespace LBUS::SLT;

namespace HulCoin{
  struct DwgReg{
    uint32_t local_address = 0;
    uint32_t delay = 0;
    uint32_t width = 0;
    uint32_t bytes = 0;
  };

  struct LocalBusReg{
    uint32_t local_address;
    uint64_t reg;
    uint32_t bytes;
  };

  using DwgMap = std::map<std::string, DwgReg>;
  DwgMap g_dwg_map;

  using RegVect = std::vector<LocalBusReg>;
  RegVect g_reg_vect;
};

int  read_register_file(std::string& filename);
void transform_dwg_reg();
void dump_lbus_reg();

int main(int argc, char* argv[])
{
  if(1 == argc){
    std::cout << "Usage\n";
    std::cout << "set_registers [IP address] [Configuration file path]"
	      << std::endl;
    return 0;
  }// usage
  
  // body ------------------------------------------------------
  std::string board_ip  = argv[kIp];
  std::string file_path = argv[kRegFile];

  read_register_file(file_path);
  transform_dwg_reg();
  dump_lbus_reg();
  
  RBCP::UDPRBCP udp_rbcp(board_ip, RBCP::gUdpPort, RBCP::DebugMode::kNoDisp);
  HUL::FPGAModule fpga_module(udp_rbcp);

  for(auto cont : HulCoin::g_reg_vect){
    fpga_module.WriteModule64(cont.local_address, cont.reg, cont.bytes);
  }

  return 0;
  
}// main

// read_register_file --------------------------------------------------

int
read_register_file(std::string& filename)
{
  std::ifstream ifs(filename.c_str());
  if(!ifs.is_open()){
    std::cout << "#E : No such the register file" << std::endl;
    return -1;
  }

  std::string line;
  while(getline(ifs, line)){
    // this is comment line
    if(line[0] == '#' || line.empty()) continue;
    
    std::istringstream LineToWord(line);
    std::string word, key;
    uint32_t    reg;
    uint32_t    address;
    uint32_t    bytes;
    LineToWord >> word;
    LineToWord >> reg;
    LineToWord >> std::hex >> address;
    LineToWord >> std::dec >> bytes;
    key = word;

    int32_t index_sep = word.find_first_of("::");
    std::string group_name = word.substr(0, index_sep);
    word = word.substr(index_sep+2, word.size() - index_sep -2);

    index_sep = word.find_first_of("::");
    std::string func_name = word.substr(0, index_sep);
    std::string reg_name  = word.substr(index_sep+2, word.size() - index_sep -2);

    
    if(func_name == "Dwg"){
      // DWGenerator setting //
      
      auto itr = HulCoin::g_dwg_map.find(group_name);
      if(itr == HulCoin::g_dwg_map.end()){
	HulCoin::DwgReg r = {address, 0, 0, bytes};
	if(reg_name == "Delay") r.delay = reg;
	if(reg_name == "Width") r.width = reg;
	HulCoin::g_dwg_map.insert(std::make_pair(group_name, r));
      }else{
	HulCoin::DwgReg& r = itr->second;
	if(reg_name == "Delay") r.delay = reg;
	if(reg_name == "Width") r.width = reg; 
      }
    }else{
      // Other local bus module settings //
      HulCoin::LocalBusReg cont = {address, reg, bytes};
      HulCoin::g_reg_vect.push_back(cont);
    }

#if defined(DEBUG)
    printf("%-25s, Reg: %4d\n", key.c_str(), reg);
#endif

  }
  return 0;
}

void
transform_dwg_reg()
{
  std::cout << "\n#D: Dump DWGenerator register" << std::endl;

  for(auto itr : HulCoin::g_dwg_map){
    std::cout << itr.first << std::endl;
    std::cout << " -address: 0x" << std::hex << std::setw(8) << std::setfill('0') << itr.second.local_address << std::endl;
    std::cout << " -delay:   " << std::dec << itr.second.delay << std::endl;
    std::cout << " -width:   " << std::dec << itr.second.width << std::endl;

    uint64_t reg64 = SetDWG(itr.second.local_address, itr.second.delay, itr.second.width);
    HulCoin::LocalBusReg cont = {itr.second.local_address, reg64, itr.second.bytes};
    HulCoin::g_reg_vect.push_back(cont);
  }
}

void
dump_lbus_reg()
{
  std::cout << "\n#D: Dump local bus register" << std::endl;
  for(auto cont : HulCoin::g_reg_vect){
    std::cout << " address(0x" << std::hex << std::setw(8) << std::setfill('0') << cont.local_address << "),"
	      << " register(0x" << std::hex << std::setw(16) << std::setfill('0') << cont.reg << "),"
	      << " bytes(" << std::dec << cont.bytes << "),"
	      << std::endl;
  }
}



//// send_register
//void
//send_register(HUL::FPGAModule& fpga_module)
//{
//  for(const auto& first_pair : HUL::MTX3D_E03E42::g_register_map){
//    for(const auto& second_pair : first_pair.second){
//      HUL::MTX3D_E03E42::Register cont = second_pair.second;
//#if DEBUG
//      printf("Address: 0x%08x, Delay: %4d, Width: %4d\n",
//	     cont.local_address, cont.delay, cont.width);
//#endif
//
//      SetDWG(fpga_module, cont.local_address, cont.delay, cont.width);
//    }
//  }
//}
