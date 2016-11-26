#include "configuration.h"
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Implementation for the 'Configuration' class.
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Date: 6 October 2016
**/

  static const string kTag = "CONFIG: ";

/******************************************************************************
 * Constructor.
**/
Configuration::Configuration() {
}

/******************************************************************************
 * Destructor.
**/
Configuration::~Configuration() {
}

/******************************************************************************
 * Accessors and mutators.
**/
/******************************************************************************
 * Accessor 'GetMaxServiceSubscript'.
 * 
 * Returns:
 *   The Max Service Subscript: actual_service_times_.size() - 1. 
 * (Needs less generalized return definition.)
**/
int Configuration::GetMaxServiceSubscript() const {
  static_cast<int> value = (actual_service_times.size()) - 1;
  return value;
}

/******************************************************************************
 * General functions.
**/
/******************************************************************************
 * Function 'ReadConfiguration'.
 * 
 * This method takes in the configuration file specified in the main class, 
 * reads it, and saves the data in the file to various variables to be sed in
 * the calculations. 
 *
 * Parameters:
 *   instream - the input stream from which to read.
**/
void Configuration::ReadConfiguration(Scanner& instream) {
  string line;
  ScanLine scanline;

  line = instream.NextLine();
  scanline.OpenString(line);  //Takes one line, seperates it by delim, saves 
  seed_ = scanline.NextInt(); //variables from that one line
  election_day_length_hours_ = scanline.NextInt();
  election_day_length_seconds_ = election_day_length_hours_ * 3600;
  time_to_vote_mean_seconds_ = scanline.NextInt();
  min_expected_to_simulate_ = scanline.NextInt();
  max_expected_to_simulate_ = scanline.NextInt();
  wait_time_minutes_that_is_too_long_ = scanline.NextInt();
  number_of_iterations_ = scanline.NextInt();

  line = instream.NextLine();
  scanline.OpenString(line); //Similar to above, split and save data. 
  arrival_zero_ = scanline.NextDouble();
  for (int sub = 0; sub < election_day_length_hours_; ++sub) {
    double input = scanline.NextDouble();
    arrival_fractions_.push_back(input);
  }

  Scanner service_times_file; //Uses actual data for comparision 
  service_times_file.OpenFile("dataallsorted.txt");
  while (service_times_file.HasNext()) {
    int thetime = service_times_file.NextInt();
    actual_service_times_.push_back(thetime);
  }
}

/******************************************************************************
 * Function 'ToString'.
 *
 * This returns a formatted output of seed_, election_day_length_hours_,
 * time_to_vote_mean_seconds_, min_expected_to_simulate_,
 * max_expected_to_simulate_, wait_time_minutes_that_is_too_long_,
 * number_of_iterations_, GetMaxServiceSubscript().
 *  
**/
string Configuration::ToString() {
  
  string s = "\n";
  int offset = 6;
  s += kTag;
  s += "RN seed:              ";
  s += Utils::Format(seed_, 8) + "\n";
  s += kTag;
  s += "Election Day length:  ";
  s += Utils::Format(election_day_length_seconds_, 8) + " =";
  s += Utils::Format(election_day_length_seconds_/3600.0, 8, 2) + " (";
  s += Utils::Format(election_day_length_hours_, 8, 2) + ") hours\n";
  s += kTag;
  s += "Time to vote mean:    ";
  s += Utils::Format(time_to_vote_mean_seconds_, 8) + " =";
  s += Utils::Format(time_to_vote_mean_seconds_/60.0, 8, 2) + " minutes\n";
  s += kTag;
  s += "Min and max expected voters for this simulation:     ";
  s += Utils::Format(min_expected_to_simulate_, 8);
  s += Utils::Format(max_expected_to_simulate_, 8) + "\n";
  s += "Wait time (minutes) that is 'too long': ";
  s += Utils::Format(wait_time_minutes_that_is_too_long_, 8) + "\n";
  s += "Number of iterations to perform: ";
  s += Utils::Format(number_of_iterations_, 4) + "\n";
  s += "Max service time subscript: ";
  s += Utils::Format(GetMaxServiceSubscript(), 6) + "\n";
  // int offset = 6; Duplicate Declaration
  s += kTag;
  s += Utils::Format(0, 2) + "-" + Utils::Format(0, 2);
  s += " : " + Utils::Format(arrival_zero_, 7, 2) + "\n";
  for (UINT sub = 0; sub < arrival_fractions_.size(); ++sub) {
    s += kTag;
    s += Utils::Format(offset+sub, 2) + "-" + Utils::Format(offset+sub+1, 2);
    s += " : " + Utils::Format(arrival_fractions_.at(sub), 7, 2) + "\n";
  }  
  s += "\n";
  return s;
}

