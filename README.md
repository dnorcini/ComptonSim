# Compton spectrometer GEANT4 simulation

Danielle Norcini (danielle.norcini@yale.edu), September 2016

## Overview
This program models the gamma-ray energy deposition physics within the context of a Compton spectrometer set-up used to measure the energy non-linearity of scintillator material. Using GEANT-4, a particle physics simulation based on C++ and the ROOT framework, the geometry represented includes:

- gamma source within a lead housing
- lead system for gamma-ray collimation
- scintillator target material within an acrylic container painted with reflective TiO2
- lead system for deflected gamma-ray shielding
- HPGe crystal
- ...

The goal of the program is to optimize the geometry of the set-up (rate vs. small area interaction). This involves understanding the rate and energy of the source, distance between components, beam colliation, etc. It is also tasked with determining the rate of multiple-scatter events and how these can be reduced in the Compton spectra. Systematics are also studied.

This package was created based from Example B4 in the GEANT-4 examples guide (http://geant4.web.cern.ch/geant4/UserDocumentation/Doxygen/examples_doc/html/ExampleB4.html). Currently operating in single-threaded mode, the program makes use of G4GeomtryMessengers class to change the values of parameters upon runtime within looped macros. It is important to note that currently the simulation only exports information about the energy deposition in the so called 'sensitive detectors', e.g. the scintillator material and HPGe crystal. Individual photons are not tracked. Furthermore, PMTs and the scintillator properities itself are not modeled, as this adds unncessary complication for our purposes. Only gamma-ray physics within the FTFP_BERT physics list is utilized.   

## Build 
The program is tested and supported on wright.physics.yale.edu. `Geant4.10`, configured with Qt viewer, is used and the source code can be found on wright via `/cmn/prospect/geant4.10.02.p01`.
 
To install on wright, complete the following steps (this assumes a working GitHub account):
```
$ mkdir ~/ComptonSim
$ cd ~/ComptonSim
$ git clone https://github.com/dnorcini/ComptonSim 
$ cd ~
$ mkdir ComptonSim-build
$ cd ComptonSim-build
$ cmake -DGeant4_DIR=/cmn/prospect/geant4.10.02.p01-build/lib64/Geant4-10.2.1 ~/ComptonSim
$ make 
```
## Run
The Qt GUI can be entered with the command `./ComptonSim` in the build directory. Individual user commands can be entered interactively on the GUI command line. To run in batch mode for long-statistics runs, the basic command `./ComptonSim -m batch.mac` can be used. 

.... add info about loops and geometry messengers

## Improvements
- model aluminum struction and concrete to understand gamma-ray reflection rate
- add real gamma source (geometry, spectrum, rate)
- ...