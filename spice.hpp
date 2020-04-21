#include <vector>
#include <string>

#include "signal.hpp"

// read a Signal from a text file produced by LTSpice
// Signal is zero-length on error
IrregularSignal read_spice_file(const std::string &fname);

// Write a Signal to a text file readable by LTSpice
// Return true on success, false on failure
bool write_spice_file(const std::string &fname, const IrregularSignal& sig);
