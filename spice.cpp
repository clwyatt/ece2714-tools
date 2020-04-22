#include "spice.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

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

  ofs << std::setprecision(10);
    
  bool ok = ofs.good();
  
  if(ok){
    for(std::size_t i = 0; i < sig.size(); ++i){
      ofs << sig[i].time << "," << sig[i].value << ",";
    }
  }
  
  return ok;
}
