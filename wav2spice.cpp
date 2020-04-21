#include <iostream>
#include <cstddef>
#include <cmath>

#include "wav.hpp"
#include "signal.hpp"
#include "spice.hpp"

int main(int argc, char *argv[])
{
  RegularSignal input;
  
  if( read_wav(input, std::string(argv[1])) ){

    double max = 0;
    for(int i = 0; i < input.size(); ++i){
      if(fabs(input[i]) > max) max = fabs(input[i]); 
    }
    std::cout << "Max value = " << max << "\n";
    
    IrregularSignal output;
    
    double dt = 1./input.getSampleRate();

    std::cout << "Time step = " << dt << "\n";
    
    double t = 0.;
    for(int i = 0; i < input.size(); ++i){
      output.push_back(t, input[i]/max);
      t += dt;
    }
    write_spice_file(std::string(argv[2]), output);
  } else {
    std::cerr << "Error: Bad input.\n";
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
}
