#include "signal.hpp"

#include <cmath>
#include <limits>
#include <iostream>

void Signal::normalize(){

  double max = std::numeric_limits<double>::min();

  for( auto sample : data){
    double temp = fabs(sample);
    if(temp > max) max = temp;
  }

  double scale = 32768.0/max;

  if(scale < 1.0){
    std::cout << "Scaling signal by " << scale << "\n";
    for( auto& sample : data){
      sample = scale*sample;
    }
  }
}

unsigned int Signal::getSampleRate(){
  return rate;
}

void Signal::setSampleRate(unsigned int theRate){
  rate = theRate;
}

void Signal::push_back(double value){
  data.push_back(value);
}


std::size_t Signal::size(){
  return data.size();
}

double Signal::operator[](std::size_t index){
  return data[index];
}
