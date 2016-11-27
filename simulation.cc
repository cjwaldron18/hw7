#include "simulation.h"
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Implementation for the 'Simulation' class.
 *
 * This class is what is called from main to run the simulation using the 
 * data found in the various files.  It uses the configuration file, input file
 * and randomizer to generate and read the Precincts, then it prints them when
 * the expected voters of a precinct falls into an expected range from the 
 * config
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
 * Uses the scanner to create and read Precincts, then saves
 * the precinct as whatever number precinct it is. Actual reading of the data
 * is within the OnePct class. 
 *
 * Parameters:
 *   infile - the input stream from which to read
**/
void Simulation::ReadPrecincts(Scanner& infile) {
  // Utils::log_stream << kTag << "FUNC: " << "ReadPrecincts() " 
  //                   << "EXECUTED." << endl;
  
  while (infile.HasNext()) {
    OnePct new_pct; //Generates a new precinct
    new_pct.ReadData(infile);
    pcts_[new_pct.GetPctNumber()] = new_pct; //Saves Precinct for simulation 
  } // while (infile.HasNext()) {
} // void Simulation::ReadPrecincts(Scanner& infile) {

/******************************************************************************
 * Function 'RunSimulation'.
 *
 * This method runs through the different number of precincts and checks if 
 * the expected voters of each princict is below the min or above the max
 * from the config. If it is not, it then calls prints the precinct and runs
 * a different RunSimulation method within the OnePct class. 
 *
 * Parameters:
 *   config - the instance of Configuration() to use
 *   random - the instance of MyRandom() to use
 *   out_stream - "the output stream to which to write" - Duncan Buell,
 *                gameplay.cc, buellduncan_hw4
**/
void Simulation::RunSimulation(const Configuration& config, MyRandom& random, 
                               ofstream& out_stream) {
  // Utils::log_stream << kTag << "FUNC: " << "RunSimulation() "
  //                   << "EXECUTED." << endl;

  string outstring = "XX";
  int pct_count_this_batch = 0;
  
  for(auto iterPct = pcts_.begin(); 
      iterPct != pcts_.end(); ++iterPct) {
    OnePct pct = iterPct->second; //Travels through saved Pct from Read method.
  int expected_voters = pct.GetExpectedVoters();
    if ((expected_voters <= config.min_expected_to_simulate_) || 
        (expected_voters > config.max_expected_to_simulate_)) {
      continue; //Makes sure expected voters fall into the right range
    }
  outstring = kTag + "RunSimulation for pct " + "\n";
  outstring += kTag + pct.ToString() + "\n";
  // Utils::Output(outstring, out_stream, Utils::log_stream);
  ++pct_count_this_batch; //Tracks how many Precincts there are in a batch
  pct.RunSimulationPct(config, random, out_stream);
  } // for(auto iterPct = pcts_.begin(); iterPct != pcts_.end(); ++iterPct)
  outstring = kTag + "PRECINCT COUNT THIS BATCH "
  + Utils::Format(pct_count_this_batch, 4) + "\n";
  // Utils::Output(outstring, out_stream, Utils::log_stream);
} // void Simulation::RunSimulation()

// Commented out 27 November 2016, C.J. Waldron
// Description: Does not execute.
/******************************************************************************
 * Function 'ToString'.
 *
 *
 * Runs through each precinct and prints out it's data based on the ToString
 * method within the Onepct class. 
 *
 * Returns: string s

string Simulation::ToString() {
  Utils::log_stream << kTag << "FUNC: " << "ToString() "
                    << "EXECUTED." << endl;

  string s = "";
  for(auto iterPct = pcts_.begin();  //Runs through all saved Precincts
      iterPct != pcts_.end(); ++iterPct) {
    s += kTag + (iterPct->second).ToString() + "\n";
  }
  return s;
} // string Simulation::ToString()
**/
