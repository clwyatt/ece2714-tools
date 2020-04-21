#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <vector>
#include <cstdint>

/** \brief A Signal holds equally spaceed samples of type double. 
 */
class Signal
{
public:

  /** Construct a Signal given a sampling rate is samples-per-second (Hz). The default rate is
   * 44.1 kHz.
   * \param sampleRate the sampling rate in Hz
   */
  Signal(unsigned int sampleRate = 44100): rate(sampleRate){};

  /** return the sampling rate in Hz.*/
  unsigned int getSampleRate();

  /** set the sampling rate in Hz.*/
  void setSampleRate(unsigned int theRate);
  
  /** Normalize the Signal to fit into a 16 bit integer 
   * when quantized.
   */
  void normalize();

  /** Add a sample to the signal. 
   * \param value the double sample value to add.
   */
  void push_back(double value);

  void pop_back(){
    data.pop_back();
  }
    

  /** Return the number of samples in the signal. */
  std::size_t size();

  /** Return the sample at given index. 
   * \param index the index of the sample to return.
   */
  double operator[](std::size_t index);
  
  
private:
  unsigned int rate;
  std::vector<double> data;
};


#endif
