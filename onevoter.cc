#include "onevoter.h"
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Implementation for the 'OneVoter' class.
 *
 * This class holds all the information of one specific voter. This includes the 
 * the times surrounding a voter, such as sequence and arrival, as well as the 
 * station a voter is at.  This class also includes methods of converting time
 * from seconds to hours, minutes, and seconds. 
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Date: 6 October 2016
 *
**/

static const string kTag = "ONEVOTER: ";

/******************************************************************************
 * Constructor.
**/
OneVoter::OneVoter() {
}
/******************************************************************************
 * Parameterized constructor
 *
 * This is called from the OnePct class to create a voter, using values 
 * specified in the config class and those calculated in the OnePct class. 
 *
 * Parameters:
 *   sequence - which voter it is in sequence, specified from OnePct
 *   arrival_seconds - The time a voter arrives, dependent on the config file
 *   or calculated by the OnePct class.
 *   duration_seconds - How long a voter is there, specified by the config file.
**/
OneVoter::OneVoter(int sequence, int arrival_seconds, int duration_seconds) {
  // Utils::log_stream << kTag << "FUNC: " << "Parameterized OneVoter() " 
  //                   << "EXECUTED." << endl;
  sequence_ = sequence;
  time_arrival_seconds_ = arrival_seconds; //Sets all the variables
  time_start_voting_seconds_ = 0;
  time_vote_duration_seconds_ = duration_seconds;
  // Dummy value of -1.
  which_station_ = -1;
}

/******************************************************************************
 * Destructor
**/
OneVoter::~OneVoter() {
}

/******************************************************************************
 * Accessors and mutators.
**/
/******************************************************************************
 * Accessor 'GetTimeArrival'.
 *
 * Returns:
 *   The arrival time of a voter in seconds.
**/
int OneVoter::GetTimeArrival() const {
  // Utils::log_stream << kTag << "FUNC: " << "GetTimeArrival() const " 
  //                   << "EXECUTED." << endl;
  return time_arrival_seconds_;
}

/******************************************************************************
 * Accessor 'GetTimeWaiting'.
 *
 * Returns:
 *   The time a voter has been waiting in seconds.
**/
int OneVoter::GetTimeWaiting() const {
  // Utils::log_stream << kTag << "FUNC: " << "GetTimeWaiting() " 
  //                   << "EXECUTED." << endl;
  return time_waiting_seconds_;
}

/******************************************************************************
 * Accessor 'GetStationNumber'.
 *
 * Returns:
 *   The station used by the voter.
**/
int OneVoter::GetStationNumber() const {
  // Utils::log_stream << kTag << "FUNC: " << "GetStationNumber() " 
  //                   << "EXECUTED." << endl;
  return which_station_;
}

/******************************************************************************
 * Accessor 'GetTimeDoneVoting'.
 *
 * Returns:
 *   The sum of the time started and the duration spent voting.
**/
int OneVoter::GetTimeDoneVoting() const {
  // Utils::log_stream << kTag << "FUNC: " << "GetTimeDoneVoting() "
  //                   << "EXECUTED." << endl;
  return time_start_voting_seconds_ + 
    time_vote_duration_seconds_;
}

/******************************************************************************
 * General functions.
**/

/******************************************************************************
 * Function 'AssignStation'.
 *
 * This method is called from the RunSimulationPct2 method, in which a voter is
 * assigned a free station from th evector of free stations. 
 *
 * Parameters:
 *   station_number - the station that a voter is assigned to, out of which is
 *   specified in RunSimulationPct.
 *   start_time_seconds - At what time a voter goes into a station.
**/
void OneVoter::AssignStation(int station_number, int start_time_seconds) {
  // Utils::log_stream << kTag << "FUNC: " << "AssignStation() " 
  //                   << "EXECUTED." << endl;

  which_station_ = station_number;
  time_start_voting_seconds_ = start_time_seconds;
  time_done_voting_seconds_ = time_start_voting_seconds_  //Sets variables
    + time_vote_duration_seconds_;
  time_waiting_seconds_ = time_start_voting_seconds_
    - time_arrival_seconds_;
}

// Commented out 27 November 2016
// Description: Does not execute.
/******************************************************************************
 * Accessor 'GetTimeInQ'.
 *
 * Returns:
 *   The start time minus the arrival time.

int OneVoter::GetTimeInQ() const {
  Utils::log_stream << kTag << "FUNC: " << "GetTimeInQ() "                    
                    << "EXECUTED." << endl;
  return time_start_voting_seconds_ - time_arrival_seconds_;
}
**/

// Commented out 27 November 2016, C.J. Waldron
// Description: Does not execute.
/******************************************************************************
 * Function 'GetTOD'.
 *
 * Converts the time of a voter by using the time in seconds and the offset
 * hours converted into seconds. 
 *
 * Parameters:
 *   time_in_seconds - 
 * Returns:
 *   The time converted from seconds.

string OneVoter::GetTOD(int time_in_seconds) const {
  Utils::log_stream << kTag << "FUNC: " << "GetTOD() " 
                    << "EXECUTED." << endl;

  // Commented out when received.
  //  int offset_hours = 6;
  int offset_hours = 0;
  string s = "";
  return this->ConvertTime(time_in_seconds + offset_hours*3600);
}
**/

// Commented out 27 November 2016, C.J. Waldron
// Description: Does not execute.
/******************************************************************************
 * Function 'ConvertTime'.
 *
 * Converts time that is listed in number of seconds to an hour:minute:seconds
 * format. 
 *
 * Parameters:
 *   time_in_seconds - The time a voter takes in terms of secons
 * Returns:
 *   A formatted conversion of time from seconds in the format
 *   hours:minutes:seconds.

string OneVoter::ConvertTime(int time_in_seconds) const { 
  Utils::log_stream << kTag << "FUNC: " << "ConvertTime() " 
                    << "EXECUTED." << endl;

  int hours = 0;
  int minutes = 0;
  int seconds = 0;
  string s = "";

  hours = time_in_seconds / 3600;
  minutes = (time_in_seconds - 3600*hours) / 60;
  seconds = (time_in_seconds - 3600*hours - 60*minutes);

  s += Utils::Format(time_in_seconds, 6);

  if (hours < 0) {
    s += " 00";
  } else if (hours < 10) { //Formats Hours 
    s += " 0" + Utils::Format(hours, 1);
  } else {
    s += " " + Utils::Format(hours, 2);
  }
  if(minutes < 0) {
    s += ":00";  
  } else if(minutes < 10) {  //Formats Minutes
    s += ":0" + Utils::Format(minutes, 1);
  } else {
    s += ":" + Utils::Format(minutes, 2);
  }

  if(seconds < 0) {
    s += ":00";
  } else if (seconds < 10) {  //Formats seconds
    s += ":0" + Utils::Format(seconds, 1);
  } else {
    s += ":" + Utils::Format(seconds, 2);
  }

  return s;
} // string OneVoter::ConvertTime(int time_in_seconds) const
**/

// Commented out 27 November 2016, C.J. Waldron
// Description: Does not execute.
/******************************************************************************
 * Function 'ToString'.
 *
 * Takes the data from a voter and converts it into a string to be printed to
 * an output file. This data includes a voter's sequence, arrival time, voting 
 * time, the duration of voting, the time a voter took in total, time in Queue,
 * and the station a voter is at.
 *
 * Returns:
 *      string s - string formatted to hold all the data

string OneVoter::ToString() {
  Utils::log_stream << kTag << "FUNC: " << "ToString() " 
                    << "EXECUTED." << endl;

  string s = kTag;

  s += Utils::Format(sequence_, 7);
  s += ": ";
  s += Utils::Format(this->GetTOD(time_arrival_seconds_));
  s += " ";
  s += Utils::Format(this->GetTOD(time_start_voting_seconds_));
  s += " ";
  s += Utils::Format(this->ConvertTime
                     (time_vote_duration_seconds_));
  s += " ";
  s += Utils::Format(this->GetTOD(
    time_start_voting_seconds_ + time_vote_duration_seconds_));
  s += " ";
  s += Utils::Format(this->ConvertTime(GetTimeInQ()));
  s += ": ";
  s += Utils::Format(which_station_, 4);

  return s;
} // string OneVoter::toString()
**/

// Commented out 27 November 2016, C.J. Waldron
// Description: Does not execute.
/******************************************************************************
 * Function 'ToStringHeader'.
 *
 * Creates a header to show all the data created in the ToString() method for
 * easier reading.
 *
 * Returns:
 *      string s - string holding the header to be printed to the output

string OneVoter::ToStringHeader() {
  Utils::log_stream << kTag << "FUNC: " << "ToStringHeader() " 
                    << "EXECUTED." << endl;

  string s = kTag;
  s += string("    Seq        Arr           Start             Dur             " )
       + string("End            Wait         Stn");
  return s;
}
**/
