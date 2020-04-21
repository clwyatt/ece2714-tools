#include "signal.hpp"

#include <cmath>
#include <limits>
#include <iostream>

void RegularSignal::normalize(){

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

unsigned int RegularSignal::getSampleRate() const{
  return rate;
}

void RegularSignal::setSampleRate(unsigned int theRate){
  rate = theRate;
}

void RegularSignal::push_back(double value){
  data.push_back(value);
}


std::size_t RegularSignal::size() const{
  return data.size();
}

double RegularSignal::operator[](std::size_t index) const{
  return data[index];
}

void IrregularSignal::push_back(double time, double value){

  IrregularSignal::Sample s;
  s.time = time;
  s.value = value;
  
  data.push_back(s);
}
    

std::size_t IrregularSignal::size() const{
  return data.size();
}


IrregularSignal::Sample IrregularSignal::operator[](std::size_t index) const{
  return data[index];
}
