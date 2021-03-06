/** \file WAV Module
 */

#ifndef WAV_HPP
#define WAV_HPP

#include <string>
#include "signal.hpp"

/** \fn Write a signal to a WAV file. Returns true on success, false on failure.
 * The signal should be quantized into a 16 bit integer using rounding to nearest.
 * \param sound the signal to write.
 * \param filename the filename to write to.
 */
bool write_wav(RegularSignal & sound, const std::string &filename);

bool read_wav(RegularSignal & sound, const std::string &filename);

#endif
