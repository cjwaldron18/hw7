#include "onevoter.h"
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Implementation for the 'OneVoter' class.
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
 * (Description)
 *
 * Parameters:
 *   sequence - 
 *   arrival_seconds - 
 *   duration_seconds -
**/
OneVoter::OneVoter(int sequence, int arrival_seconds,
int duration_seconds) {
  sequence_ = sequence;
  time_arrival_seconds_ = arrival_seconds;
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
  return time_arrival_seconds_;
}

/******************************************************************************
 * Accessor 'GetTimeWaiting'.
 *
 * Returns:
 *   The time a voter has been waiting in seconds.
**/
int OneVoter::GetTimeWaiting() const {
  return time_waiting_seconds_;
}

/******************************************************************************
 * Accessor 'GetStationNumber'.
 *
 * Returns:
 *   The station used by the voter.
**/
int OneVoter::GetStationNumber() const {
  return which_station_;
}

/******************************************************************************
 * General functions.
**/

/******************************************************************************
 * Function 'AssignStation'.
 *
 * (Description)
 *
 * Parameters:
 *   station_number - 
 *   start_time_seconds -
**/
void OneVoter::AssignStation(int station_number,
int start_time_seconds) {
  which_station_ = station_number;
  time_start_voting_seconds_ = start_time_seconds;
  time_done_voting_seconds_ = time_start_voting_seconds_
    + time_vote_duration_seconds_;
  time_waiting_seconds_ = time_start_voting_seconds_
    - time_arrival_seconds_;
}

/******************************************************************************
 * Accessor 'GetTimeDoneVoting'.
 *
 * Returns:
 *   The sum of the time started and the duration spent voting.
**/
int OneVoter::GetTimeDoneVoting() const {
  return time_start_voting_seconds_ + 
    time_vote_duration_seconds_;
}

/******************************************************************************
 * Accessor 'GetTimeInQ'.
 *
 * Returns:
 *   The start time minus the arrival time.
**/
int OneVoter::GetTimeInQ() const {
  return time_start_voting_seconds_ - time_arrival_seconds_;
}

/******************************************************************************
 * Function 'GetTOD'.
 *
 * (Description)
 *
 * Parameters:
 *   time_in_seconds - 
 * Returns:
 *   The time converted from seconds.
**/
string OneVoter::GetTOD(int time_in_seconds) const {
  // Commented out when received.
  //  int offset_hours = 6;
  int offset_hours = 0;
  string s = "";
  return this->ConvertTime(time_in_seconds + offset_hours*3600);
}

/******************************************************************************
 * Function 'ConvertTime'.
 *
 * (Description)
 *
 * Parameters:
 *   time_in_seconds - 
 * Returns:
 *   A formatted conversion of time from seconds in the format
 *   hours:minutes:seconds.
**/
string OneVoter::ConvertTime(int time_in_seconds) const {
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
  } else if (hours < 10) {
    s += " 0" + Utils::Format(hours, 1);
  } else {
    s += " " + Utils::Format(hours, 2);
  }
  if(minutes < 0) {
    s += ":00";
  } else if(minutes < 10) {
    s += ":0" + Utils::Format(minutes, 1);
  } else {
    s += ":" + Utils::Format(minutes, 2);
  }

  if(seconds < 0) {
    s += ":00";
  } else if (seconds < 10) {
    s += ":0" + Utils::Format(seconds, 1);
  } else {
    s += ":" + Utils::Format(seconds, 2);
  }

  return s;
} // string OneVoter::ConvertTime(int time_in_seconds) const

/******************************************************************************
 * Function 'ToString'.
 *
 * (Description)
 *
 * Returns:
**/
string OneVoter::ToString() {
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

/******************************************************************************
 * Function 'ToStringHeader'.
 *
 * (Description)
 *
 * Returns:
**/
string OneVoter::ToStringHeader() {
  string s = kTag;
  s += "    Seq        Arr           Start             Dur             " + 
    "End            Wait         Stn";
  return s;
}

