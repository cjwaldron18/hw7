#include "simulation.h"
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Implementation for the 'Simulation' class.
 *
 * Author/copyright:  Duncan Buell
 * Date: 19 July 2016
 *
**/

static const string kTag = "SIM: ";

/******************************************************************************
 * Constructor.
**/
Simulation::Simulation() {
}

/******************************************************************************
 * Destructor.
**/
Simulation::~Simulation() {
}

/******************************************************************************
 * General functions.
**/
/******************************************************************************
 * Function 'ReadPrecincts'.
 *
 * (Description)
 *
 * Parameters:
 *   infile - the input stream from which to read
**/
void Simulation::ReadPrecincts(Scanner& infile) {
  
  while (infile.HasNext()) {
    OnePct new_pct;
    new_pct.ReadData(infile);
    pcts_[new_pct.GetPctNumber()] = new_pct;
  } // while (infile.HasNext()) {
} // void Simulation::ReadPrecincts(Scanner& infile) {

/******************************************************************************
 * Function 'RunSimulation'.
 *
 * (Description)
 *
 * Parameters:
 *   config - the instance of Configuration() to use
 *   random - the instance of MyRandom() to use
 *   out_stream - "the output stream to which to write" - Duncan Buell,
 *                gameplay.cc, buellduncan_hw4
**/
void Simulation::RunSimulation(const Configuration& config,
MyRandom& random, ofstream& out_stream) {
  
  string outstring = "XX";
  int pct_count_this_batch = 0;
  
  for(auto iterPct = pcts_.begin(); 
      iterPct != pcts_.end(); ++iterPct) {
    OnePct pct = iterPct->second;
  int expected_voters = pct.GetExpectedVoters();
    if ((expected_voters <= config.min_expected_to_simulate_) || 
        (expected_voters > config.max_expected_to_simulate_)) {
      continue;
    }
  outstring = kTag + "RunSimulation for pct " + "\n";
  outstring += kTag + pct.ToString() + "\n";
  // Utils::Output(outstring, out_stream, Utils::log_stream);
  ++pct_count_this_batch;
  pct.RunSimulationPct(config, random, out_stream);
  } // for(auto iterPct = pcts_.begin(); iterPct != pcts_.end(); ++iterPct)
  outstring = kTag + "PRECINCT COUNT THIS BATCH "
  + Utils::Format(pct_count_this_batch, 4) + "\n";
  // Utils::Output(outstring, out_stream, Utils::log_stream);
} // void Simulation::RunSimulation()

/******************************************************************************
 * Function 'ToString'.
 *
 * Returns:
**/
string Simulation::ToString() {
  
  string s = "";
  for(auto iterPct = pcts_.begin(); 
      iterPct != pcts_.end(); ++iterPct) {
    s += kTag + (iterPct->second).ToString() + "\n";
  }
  return s;
} // string Simulation::ToString()

