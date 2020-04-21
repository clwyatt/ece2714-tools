#include "spice.hpp"

#include <iostream>
#include <fstream>

IrregularSignal read_spice_file(const std::string &fname){

  IrregularSignal s;
  
  std::ifstream ifs(fname);

  bool ok = ifs.good();
  
  if(ok){
    std::string line;
    std::getline(ifs, line);
    
    while(!ifs.eof() && ifs.good()){
      IrregularSignal::Sample sample;
      ifs >> sample.time >> sample.value;

      if(ifs.good()){
	s.push_back(sample.time, sample.value);
      }
    }
  }

  return s;
}

bool write_spice_file(const std::string &fname, const IrregularSignal& sig){

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
