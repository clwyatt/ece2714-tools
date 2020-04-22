#include <iostream>
#include <cstddef>
#include <cmath>

#include "wav.hpp"
#include "signal.hpp"
#include "spice.hpp"

void usage(){
  std::cout << "spice2wav [-rate R] input.wav output.pwl\n";
}

int main(int argc, char *argv[])
{
  std::string inputfile;
  std::string outputfile;
  unsigned int rate = 1000;
  
  // parse arguments
  if((argc != 3) && (argc != 5)){
    std::cerr << "Error: incorrect number of arguments\n";
    usage();
    return EXIT_FAILURE;
  }

  int argn = 1;
  if(argc == 5){
    if(std::string(argv[argn]) != "-rate"){
      std::cerr << "Error: invalid option " << argv[argn] << "\n";
      usage();
      return EXIT_FAILURE;
    }
    argn++;
    char * end; // TODO: replace with std::stoi
    rate = strtol(argv[argn], &end, 10);
    if(rate <= 0){
      std::cerr << "Error: invalid rate " << rate << "\n";
      return EXIT_FAILURE;
    }
    argn++;
  }

  inputfile = argv[argn];
  outputfile = argv[argn+1];

  // convert the file
  IrregularSignal input = read_spice_file(inputfile);
    
  if( input.size() == 0 ){
    std::cerr << "Error: Bad input file.\n";
    return EXIT_FAILURE;
  }

  double absmax = 0;
  for(std::size_t i = 0; i < input.size(); ++i){
    double value = fabs(input[i].value);
    if(value > absmax) absmax = value;
  }

  double scale = 4096/absmax;
  double dt = 1.0/rate;
  
  // get time point of last sample
  double endTime = input[input.size() -1].time;

  // generate regularly sampled signal
  RegularSignal sampled(rate);
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
      
      sampled.push_back(scale*value);
    }
    else{
      sampled.push_back(input[nearidx].value);
    }
    
    t += dt;
  }
  
  // save output wav file
  write_wav(sampled, outputfile);
  
  return EXIT_SUCCESS;
}
