#include "spice.hpp"

#include <iostream>
#include <fstream>

SpiceSignal read_spice_file(const std::string &fname){

  SpiceSignal s;
  
  std::ifstream ifs(fname);

  bool ok = ifs.good();
  
  if(ok){
    std::string line;
    std::getline(ifs, line);
    
    while(!ifs.eof() && ifs.good()){
      Sample sample;
      ifs >> sample.time >> sample.value;

      if(ifs.good()){
	s.push_back(sample);
      }
    }
  }

  return s;
}

bool write_spice_file(const std::string &fname, const SpiceSignal& sig){

  std::ofstream ofs(fname);

  bool ok = ofs.good();
  
  if(ok){
    //ofs << "time   V\n";
    for(std::size_t i = 0; i < sig.size(); ++i){
      ofs << sig[i].time << "," << sig[i].value << ",";
    }
  }
  
  return ok;
}
