/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Header for the 'OnePct' class
 *
 * Stores precinct data. Functions are related to running a simulation on
 * one precinct, including creating the voters needed to run a simulation.
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

#ifndef ONEPCT_H
#define ONEPCT_H

#include <cmath>
#include <map>
#include <set>
#include <vector>

#include "Utilities/utils.h"
#include "Utilities/scanner.h"
#include "Utilities/scanline.h"

using namespace std;

#include "configuration.h"
#include "myrandom.h"
#include "onevoter.h"

static const double kDummyDouble = -88.88;
static const int kDummyInt = -999;
static const string kDummyString = "dummystring";

class OnePct {
public:
/******************************************************************************
 * Constructors and destructors for the class. 
**/
 OnePct();
 virtual ~OnePct();
// Commented out 27 November 2016, C.J. Waldron
// Description: The parameterized constructor is not used.
// OnePct(Scanner& infile);

/******************************************************************************
 * Accessors and Mutators.
**/
 int GetExpectedVoters() const;
 int GetPctNumber() const;
 
/******************************************************************************
 * General functions.
**/
 void ReadData(Scanner& infile);
 void RunSimulationPct(const Configuration& config, MyRandom& random, 
                        ofstream& out_stream);
 string ToString();
// Commented out 27 November 2016, C.J. Waldron
// Description: This function is not executed.
// string ToStringVoterMap(string label, multimap<int, OneVoter> themap);

private:
 int    pct_expected_voters_ = kDummyInt;
 int    pct_expected_per_hour_ = kDummyInt;
 double pct_minority_ = kDummyDouble;
 string pct_name_ = kDummyString;
 int    pct_number_ = kDummyInt;
 double pct_turnout_ = kDummyDouble;
 int    pct_stations_ = kDummyInt;
 int    pct_num_voters_ = kDummyInt;
 double wait_dev_seconds_;
 double wait_mean_seconds_;
 set<int> stations_to_histo_;
 vector<int> free_stations_;
 multimap<int, OneVoter> voters_backup_;
 multimap<int, OneVoter> voters_done_voting_;
 multimap<int, OneVoter> voters_pending_;
 multimap<int, OneVoter> voters_voting_;

/******************************************************************************
 * General private functions.
**/
 void CreateVoters(const Configuration& config, MyRandom& random,
                   ofstream& out_stream);
 int DoStatistics(int iteration, const Configuration& config, 
                  int station_count,
                  map<int, int>& map_for_histo, ofstream& out_stream);                  
 void ComputeMeanAndDev();
 void RunSimulationPct2(int stations);
};

#endif // ONEPCT_H
