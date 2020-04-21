#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include <vector>
#include <cstdint>

/** \brief A RegularSignal holds equally spaced samples of type double. 
 */
class RegularSignal
{
public:

  /** Construct a RegularSignal given a sampling rate in Hz. 
   *  The default rate is 44.1 kHz.
   * \param sampleRate the sampling rate in Hz
   */
  RegularSignal(unsigned int sampleRate = 44100): rate(sampleRate){};

  /** return the sampling rate in Hz.*/
  unsigned int getSampleRate() const;

  /** set the sampling rate in Hz.*/
  void setSampleRate(unsigned int theRate);
  
  /** Normalize the RegularSignal to fit into a 16 bit integer 
   * when quantized.
   */
  void normalize();

  /** Add a sample to the signal. 
   * \param value the sample value to add.
   */
  void push_back(double value);
    
  /** Return the number of samples in the signal. */
  std::size_t size() const;

  /** Return the sample at given index. 
   * \param index the index of the sample to return.
   */
  double operator[](std::size_t index) const;
  
  
private:
  unsigned int rate;
  std::vector<double> data;
};

/** \brief An IrregularSignal holds irregularly spaced samples of type 
 * double. 
 */
class IrregularSignal
{
public:

  /** Each sample is a pair of a time-point and a value */
  struct Sample{
    double time;
    double value;
  };
  
  /** Construct a IrregularSignal
   */
  IrregularSignal() = default;
  
  /** Add a sample to the signal. 
   * \param value the sample time to add.
   * \param value the sample value to add.
   */
  void push_back(double time, double value);
    
  /** Return the number of samples in the signal. */
  std::size_t size() const;

  /** Return the sample at given index. 
   * \param index the index of the sample to return.
   */
  Sample operator[](std::size_t index) const;
  
private:
  
  std::vector<Sample> data;
};


#endif
