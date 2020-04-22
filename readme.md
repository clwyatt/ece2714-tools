Tools for ECE 2714 Project
---------------------------

Two command-line programs for the software-only version of the ECE 2714 project, Spring 2020.

* wav2spice - convert single channel wav file to continuous-time approximation as a PFL file, suitable for use as a source signal in LTspice.

Usage:

```
wav2spice -linear input.wav output.pwl
```
uses a linear apprximation, or
```
wav2spice -step input.wav output.pwl
```
uses a step approximation (like the output of an R-2R ladder).

* spice2wav - convert an ASCII file like that exported by LTspice to a wav file, sampling at regularly spaced intervals at specified rate.

Usage:

```
spice2wav -rate 44100 input.txt output.wav 
```


