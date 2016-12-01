/******************************************************************************
 * Header for the 'OneVoter' class
 *
 * Container class for a single voter. Voters are assigned a sequence,
 * arrival time, and duration of voting upon creation and can be assigned
 * to a voting station.
 * 
 * Author/copyright:  Duncan Buell
 * Used and modified with permission by:
 *   CJ Waldron
 *   Abraham Kahn
 *   Thomas O'Hara
 *   Stephen Page
 *   Oneil Clemenson
 *
 * Date: November 2016
 *
**/

#ifndef ONEVOTER_H
#define ONEVOTER_H

#include "Utilities/utils.h"

using namespace std;

static int kDummyVoterInt = -333;

class OneVoter {
public:
/******************************************************************************
 * Constructors and destructors for the class. 
**/
 OneVoter();
 OneVoter(int sequence, int arrival_seconds, int duration_seconds);
 virtual ~OneVoter();

/******************************************************************************
 * Accessors and Mutators.
**/
 int GetStationNumber() const;
 int GetTimeArrival() const;
 int GetTimeDoneVoting() const;
 int GetTimeWaiting() const;

/******************************************************************************
 * General functions.
**/
 void AssignStation(int station_number, int start_time_seconds);
 //Commented out 30 November 2016, Stephen Page
 //Description: Not Executed
 //void DoneVoting();
 
// Commented out 27 November 2016, C.J. Waldron
// Description: These are not executed.
// int GetTimeInQ() const;
// string ToString();
// static string ToStringHeader();

private:
 int sequence_ = kDummyVoterInt;
 int time_arrival_seconds_ = kDummyVoterInt;
 int time_done_voting_seconds_ = kDummyVoterInt;
 int time_start_voting_seconds_ = kDummyVoterInt;
 int time_vote_duration_seconds_ = kDummyVoterInt;
 int time_waiting_seconds_ = kDummyVoterInt;
 int which_station_ = kDummyVoterInt;

/******************************************************************************
 * General private functions.
**/
// Commented out 27 November 2016, C.J. Waldron
// Description: These are not executed.
// string ConvertTime(int time_in_seconds) const;
// string GetTOD(int time) const;
};

#endif // ONEVOTER_H
