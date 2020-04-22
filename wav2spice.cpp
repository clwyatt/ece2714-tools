#include <cmath>
#include <cstddef>
#include <iostream>

#include "signal.hpp"
#include "spice.hpp"
#include "wav.hpp"

enum ApproxType { LINEAR, STEP };

void usage() {
  std::cout << "wav2spice [-linear | -step] input.wav output.pwl\n";
}

int main(int argc, char *argv[]) {

  std::string inputfile;
  std::string outputfile;
  ApproxType approximation = LINEAR;

  // parse arguments
  if ((argc < 3) || (argc > 4)) {
    std::cerr << "Error: incorrect number of arguments\n";
    usage();
    return EXIT_FAILURE;
  }

  int startargs = 1;
  if (argc == 4) {
    startargs++;
    if (std::string(argv[1]) == "-linear") {
      approximation = LINEAR;
    } else if (std::string(argv[1]) == "-step") {
      approximation = STEP;
    } else {
      std::cerr << "Error: invalid option " << argv[1] << "\n";
      usage();
      return EXIT_FAILURE;
    }
  }

  inputfile = argv[startargs];
  outputfile = argv[startargs + 1];

  // convert the file
  RegularSignal input;

  if (read_wav(input, inputfile)) {

    double max = 0;
    for (int i = 0; i < input.size(); ++i) {
      if (fabs(input[i]) > max)
        max = fabs(input[i]);
    }

    IrregularSignal output;

    const double dt = 1. / input.getSampleRate();

    double t = 0.;
    for (int i = 0; i < input.size(); ++i) {
      const double eps = dt / 100;
      if (approximation == LINEAR) {
        output.push_back(t, input[i] / max);
      } else { // assume STEP
        if (i == 0) {
          output.push_back(t, input[i] / max);
          output.push_back(t + dt / 2, input[i] / max);
        } else {
          output.push_back(t - eps, input[i - 1] / max);
          output.push_back(t + eps, input[i] / max);
          output.push_back(t + dt / 2, input[i] / max);
        }
      }

      t += dt;
    }

    write_spice_file(outputfile, output);
  } else {
    std::cerr << "Error: Bad input file.\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
