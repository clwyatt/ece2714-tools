#include <vector>
#include <string>

struct Sample{
  double time;
  double value;
};

typedef std::vector<Sample> SpiceSignal;

// read a Signal from a text file produced by LTSpice
// Signal is zero-length on error
SpiceSignal read_spice_file(const std::string &fname);

// Write a Signal to a text file readable by LTSpice
// Return true on success, false on failure
bool write_spice_file(const std::string &fname, const SpiceSignal& sig);
