#include <iostream>
#include <cstddef>
#include <cmath>

#include "wav.hpp"
#include "signal.hpp"
#include "spice.hpp"

int main(int argc, char *argv[])
{
  SpiceSignal input = read_spice_file(std::string(argv[1]));
    
  if( input.size() == 0 ){
    std::cerr << "Error: Bad input.\n";
    return EXIT_FAILURE;
  }

  double absmax = 0;
  for(std::size_t i = 0; i < input.size(); ++i){
    double value = fabs(input[i].value);
    if(value > absmax) absmax = value;
  }

  std::cout << "Abs Max = " << absmax << "\n";

  double scale = 4096/absmax;
    
  char * end;
  unsigned int rate = strtol(argv[2], &end, 10);
  double dt = 1.0/rate;

  std::cout << "Using Time Step: " << dt << "\n";
  
  // get time point of last sample
  double endTime = input.back().time;

  // generate regularly sampled signal
  Signal sampled(rate);
  double t = 0;
  std::size_t nearidx = 0;
  while(t < endTime){

    while(input[nearidx].time < t){
      nearidx += 1;
    }

    if(nearidx > 0){
      auto i = nearidx;
      auto j = i-1;

      double ti = input[i].time;
      double xi = input[i].value;
      double tj = input[j].time;
      double xj = input[j].value;

      double value = xj + (xi-xj)*(t-tj)/(ti-tj);

      // std::cout << "Time " << t << " ";
      // std::cout << "ti[" << i << "] = " << ti << " ";
      // std::cout << "xi[" << i << "] = " << xi << " ";
      // std::cout << "tj[" << j << "] = " << tj << " ";
      // std::cout << "xj[" << j << "] = " << xj << "\n";
      
      sampled.push_back(scale*value);
    }
    else{
      sampled.push_back(input[nearidx].value);
    }
    
    t += dt;
  }
  
  // save output wav file
  write_wav(sampled, std::string(argv[3]));
  
  return EXIT_SUCCESS;
}
