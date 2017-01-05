// ComptonRunAction.hh

#ifndef ComptonRunAction_h
#define ComptonRunAction_h 1
#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

// Run action class
// It accumulates statistic and computes dispersion of the energy deposit 
// and track lengths of charged particles with use of analysis tools:
// H1D histograms are created in BeginOfRunAction() for the following 
// physics quantities:
// - Energy deposition in HPGe
// - Energy deposition in scintillator
// - Track length in HPGe
// - Track length in scintillator
// The same values are also saved in the ntuple.
// The histograms and ntuple are saved in the output file in a format
// accoring to a selected technology in ComptonAnalysis.hh.
//
// In EndOfRunAction(), the accumulated statistic and computed 
// dispersion is printed.

class ComptonRunAction : public G4UserRunAction
{
  public:
    ComptonRunAction();
    virtual ~ComptonRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};

#endif

