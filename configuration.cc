#include "configuration.h"
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Implementation for the 'Configuration' class. This class is responsible
 * for assigning independent variables needed in order to simulate an election,
 * as specified by Queueing theory. Ultimately, what this class does is read an
 * input stream that points to a configuration file while also using another
 * input stream which points to a service times file. Both of these files are 
 * needed in order to populate the vectors for service times and arrival
 * percentages per hour. 
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
 * Since the service times are in ascending order from least to greatest, the 
 * subscript of the greatest element will be the last element in the
 * actucal_service_times_ vector. 
 *
 * Returns:
 *   the maximum service time - actual_service_times_.size() - 1. 
 * 
**/
int Configuration::GetMaxServiceSubscript() const {
  // Test if function is executed. 
  // Utils::log_stream << kTag << "FUNC: " << "GetMaxServiceSubscript() " 
  //                   << "EXECUTED." << endl 
  int value = (actual_service_times_.size()) - 1;
  return value;
}
/******************************************************************************
 * General functions.
**/
/******************************************************************************
 * Function 'ReadConfiguration'.
 * 
 * This member function reads a configuration file via an input stream. This file
 * is used to define the contours of a statistical computation relating
 * to the simulation of election polling times. Therefore, the confinguration
 * file from which it reads should have the following: an RN seed,
 * number of hours in an election day, the mean time taken to vote,
 * minimum and maximum number of voters per precinct, 
 * the wait time maximum (wait_time_minutes_that_is_too_long), and the number  
 * of iterations to perform. 
 *
 * Parameters:
 *   instream - the input stream from which to read.
**/
void Configuration::ReadConfiguration(Scanner& instream) {
  // Test if function is executed. 
  // Utils::log_stream << kTag << "FUNC: " << "ReadConfiguration() "
  //                   << "EXECUTED." << endl; 
  string line;
  ScanLine scanline;
  // Read the first line.
  line = instream.NextLine();
  scanline.OpenString(line);  
  /* Fill in the variables with the tokens delimited by spaces
     as specified in the documentation of the function for the
     first line.
   */
  seed_ = scanline.NextInt(); 
  election_day_length_hours_ = scanline.NextInt();
  election_day_length_seconds_ = election_day_length_hours_ * 3600; 
  time_to_vote_mean_seconds_ = scanline.NextInt();
  min_expected_to_simulate_ = scanline.NextInt();
  max_expected_to_simulate_ = scanline.NextInt();
  wait_time_minutes_that_is_too_long_ = scanline.NextInt();
  number_of_iterations_ = scanline.NextInt();
  //Read the second line
  line = instream.NextLine();
  scanline.OpenString(line);
  /* Fill in the variables with the tokens delimited by spaces
     as specified in the documentation of the function for
     the second line.
  */
  arrival_zero_ = scanline.NextDouble();  
  for (int sub = 0; sub < election_day_length_hours_; ++sub) {    
    double input = scanline.NextDouble();
    arrival_fractions_.push_back(input);    
  }
}

/******************************************************************************
 * Function 'ReadServiceTimes'.
 *
 * This function is meant to read
 * service times, sorted in ascending order, via an input stream. 
 * these service time values must be integers.
 * Now, when we mean service time we usually mean it in the context of
 * Queueing theory.
 *
 * Parameters:
 *   instream - the input stream from which to read.
**/
void Configuration::ReadServiceTimes(Scanner& instream) {
  while (instream.HasNext()) {    
    int thetime = instream.NextInt();
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
  // Test if function is executed. 
  // Utils::log_stream << kTag << "FUNC: " << "ToString() "
  //                   << "EXECUTED." << endl;
  string s = "\n";
  int offset = 6;
  s += kTag;  
  // s += "RN seed:              "; 
  s += Utils::Format("RN seed:", 22);
  s += Utils::Format(seed_, 8) + "\n";
  s += kTag + "Election Day length:  ";  
  s += Utils::Format(election_day_length_seconds_, 8) + " =";
  // s += Utils::Format(election_day_length_seconds_/3600.0, 8, 2) + " (";
  s += Utils::Format(election_day_length_hours_, 6, 2) + " (";
  s += Utils::Format(election_day_length_hours_, 8, 2) + ") hours\n";
  s += kTag + "Time to vote mean:  "; 
  s += Utils::Format(time_to_vote_mean_seconds_, 8) + " =";
  s += Utils::Format(time_to_vote_mean_seconds_/60.0, 5, 2) + " minutes\n";
  s += kTag + "Min and max expected voters for this simulation:     ";  
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
