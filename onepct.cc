#include "onepct.h"
/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
* Implementation for the 'OnePct' class.
*
* The purpose of this class is to hold all the information and run the 
* calculations of one Precinct at a time. The class uses the data from the 
* config file and creates voters, as well as find our the times a voter has
* to wait and which station a voter used. The data from this class is then
* Used to run the simulation class. 
*
* Author/copyright:  Duncan Buell. All rights reserved.
* Date: 21 May 2013
*
**/

static const string kTag = "OnePct: ";

/******************************************************************************
* Constructor.
**/
OnePct::OnePct() {
}
/******************************************************************************
* Destructor.
**/
OnePct::~OnePct() {
}
/******************************************************************************
* Accessors and mutators.
**/
/******************************************************************************
 * Accessor 'GetExpectedVoters'.
 *
 * Returns:
 *   pct_expected_voters_ - The percentage of expected voters for the precinct.
**/
int OnePct::GetExpectedVoters() const {
  // Test if function is executed. 
  // Utils::log_stream << kTag << "FUNC: " << "GetExpectedVoters() " 
  //                   << "EXECUTED." << endl;
  return pct_expected_voters_;
}
/******************************************************************************
 * Accessor 'GetPctNumber'.
 *
 * Returns:
 *   pct_number_ - The precinct number (ID) for this precinct.
**/
int OnePct::GetPctNumber() const {
  // Test if functino is executed.
  // Utils::log_stream << kTag << "FUNC: " << "GetPctNumber() const " 
  //                   << "EXECUTED." << endl;
  return pct_number_;
}
/******************************************************************************
* General functions.
**/
/******************************************************************************
 * Function 'ComputeMeanAndDev'.
 *
 * This class takes the wait time of the voters in seconds, then does a 
 * calculation to find the mean/average wait time.  It then runs through the
 * voters' wait times againj and does calculations to find the deviation of 
 * the mean
**/
void OnePct::ComputeMeanAndDev() {
  // Test if function is executed.
  // Utils::log_stream << kTag << "FUNC: " << "ComputeMeanAndDev() " 
  //                   << "EXECUTED." << endl;
  int sum_of_wait_times_seconds = 0;
  double sum_of_adjusted_times_seconds = 0.0;
  sum_of_wait_times_seconds = 0;
  multimap<int, OneVoter>::iterator iter_multimap;
  for (iter_multimap = voters_done_voting_.begin();
       iter_multimap != voters_done_voting_.end(); 
       ++iter_multimap) { //Traveling through the map of voters
    OneVoter voter = iter_multimap->second;
    sum_of_wait_times_seconds += voter.GetTimeWaiting();  //adds wait times
  } 
  // Divides sum of wait times by number of voters
  wait_mean_seconds_ = static_cast<double>(sum_of_wait_times_seconds) 
                     / static_cast<double>(pct_expected_voters_); 
  sum_of_adjusted_times_seconds = 0.0;
  for (iter_multimap = voters_done_voting_.begin();
       iter_multimap != voters_done_voting_.end(); ++iter_multimap) {
    OneVoter voter = iter_multimap->second;
    double this_addin = static_cast<double> (voter.GetTimeWaiting())
                      - wait_mean_seconds_;
    sum_of_adjusted_times_seconds += (this_addin) * (this_addin);
  } //Calculation for deviation of a mean.
  wait_dev_seconds_ = sqrt(sum_of_adjusted_times_seconds /
  static_cast<double>(pct_expected_voters_));
}
/******************************************************************************
 * Function 'CreateVoters'.
 * 
 * This method is used to create voters from a Precinct. It uses the randomizer
 * To choose a random duration from the config's actual duration, then creates
 * a voter using that duration, an arrival number, and the voter in sequence, 
 * all of which is also backed up in a vector.  The sequence is then increased
 * So that each voter has it's own sequence number. 
 *
 * Once going through all voters that arrived at time 0, a value pulled from 
 * the config, it then repeats the process using the arrivals that came 
 * throughout the day, splits them up based on the hour they came in,
 * then saves those voters into the backup vector. 
 *
 * Parameters:
 *   config - the instance of Configuration() to use
 *   random - the instance of MyRandom() to use
 *   out_stream - "the output stream to which to write" - Duncan Buell,
 *                gameplay.cc, buellduncan_hw4
**/
void OnePct::CreateVoters(const Configuration& config, MyRandom& random,
                          ofstream& out_stream) {
  // Test if function is executed. 
  // Utils::log_stream << kTag << "FUNC: " << "CreateVoters() " 
  //                   << "EXECUTED." << endl;
  int duration = 0;
  int arrival = 0;
  int sequence = 0;
  double percent = 0.0;
  string outstring = "XX";
  // If the function has been previously called, the backup is emptied.
  voters_backup_.clear();
  percent = config.arrival_zero_;
  // Number of voters at time 0.
  int voters_at_zero = round((percent / 100.0) * pct_expected_voters_);
  for (int voter = 0; voter < voters_at_zero; ++voter) {
    int durationsub = random.RandomUniformInt
      (0, config.GetMaxServiceSubscript());
    duration = config.actual_service_times_.at(durationsub);
    OneVoter one_voter(sequence, arrival, duration);
    // Saves voters into a vector.
    voters_backup_.insert(std::pair<int, OneVoter> (arrival, one_voter));                          
    ++sequence; // Incremented for each voter in sequence
  }
  for (int hour = 0; hour < config.election_day_length_hours_; ++hour) {
    // Finds number of voters in a specific hour.
    percent = config.arrival_fractions_.at(hour); 
    int voters_this_hour = round((percent / 100.0) * pct_expected_voters_); 
    if (0 == hour % 2) {
      ++voters_this_hour;
    }   
    int arrival = hour*3600; // Converts arrival time from hours to seconds.
    for(int voter = 0; voter < voters_this_hour; ++voter) {
      double lambda = static_cast<double> 
                      (voters_this_hour / 3600.0);
      // Randomizes the arrival time in seconds.
      int interarrival = random.RandomExponentialInt(lambda); 
      arrival += interarrival;
      int durationsub = random.RandomUniformInt
        (0, config.GetMaxServiceSubscript());
      duration = config.actual_service_times_.at(durationsub);
      OneVoter one_voter(sequence, arrival, duration);
      voters_backup_.insert(std::pair<int, OneVoter>(arrival, one_voter));
      ++sequence;
    }
  }
}
/*******************************************************************************
 * Function 'DoStatistics'.
 *
 * The first thing this method does it take the voters that finished voting and
 * figures out how long the voter waited in minutes and saves that data into a
 * map. It then travels the map and checks to see if the wait time was too long, 
 * which is done in terms of the wait time specified in the config file, as
 * well as that number increased by 10 and 20 minutes. Finally, it runs 
 * a method to compute the mean and deviation of the wait time, then 
 * saves all the data it found into the outstring, followed by clearing the
 * minutes waiting map. 
 *
 * Parameters: 
 *   iteration - the level of iteration
 *   config - the instance of Configuration() to use
 *   station_count - the total number of stations (pct_stations_)
 *   map_for_histo - the map container to use (<int, int>), for simulation.
 *   out_stream - "the output stream to which to write" - Duncan Buell,
 *                gameplay.cc, buellduncan_hw4
 * Returns:
 *   toolongcount - the number of people that had to wait far too long to vote
**/
int OnePct::DoStatistics(int iteration, const Configuration& config,
                         int station_count, map<int, int>& map_for_histo,
                         ofstream& out_stream) {
  // Test if function is executed.
  // Utils::log_stream << kTag << "FUNC: " << "DoStatistics() " 
  //                   << "EXECUTED." << endl;
  string outstring = "\n";
  map<int, int> wait_time_minutes_map;
  multimap<int, OneVoter>::iterator iter_multimap;
  for (iter_multimap = this->voters_done_voting_.begin();
       iter_multimap != this->voters_done_voting_.end(); ++iter_multimap) {
    OneVoter voter = iter_multimap->second;
    // Wait time converted from seconds to minutes.
    int wait_time_minutes = voter.GetTimeWaiting() / 60;
    ++(wait_time_minutes_map[wait_time_minutes]);
    ++(map_for_histo[wait_time_minutes]);
  }
  int toolongcount = 0;
  int toolongcountplus10 = 0;
  int toolongcountplus20 = 0;
  for (auto iter = wait_time_minutes_map.rbegin();
       iter != wait_time_minutes_map.rend(); ++iter) {
    int waittime = iter->first;
    int waitcount = iter->second;
    if (waittime > config.wait_time_minutes_that_is_too_long_) {
      toolongcount += waitcount;
    } //If wait time surpasses what is too long, the counter rises
    if (waittime > config.wait_time_minutes_that_is_too_long_ + 10) { 
      toolongcountplus10 += waitcount;
    } //Secondary counters for if the wait time is over 10 and/or 20 minutes.
    if (waittime > config.wait_time_minutes_that_is_too_long_ + 20) {
      toolongcountplus20 += waitcount;
    }  
  }
///////////////////////////////////////////////////////////////////////////////
  ComputeMeanAndDev();
  outstring = "";
  outstring += kTag + Utils::Format(iteration, 3) + " "
  + Utils::Format(pct_number_, 4) + " "
  + Utils::Format(pct_name_, 25, "left")
  + Utils::Format(pct_expected_voters_, 6)
  + Utils::Format(station_count, 4)
  + " stations, mean/dev wait (mins) "
  + Utils::Format(wait_mean_seconds_/60.0, 8, 2) + " "
  + Utils::Format(wait_dev_seconds_/60.0, 8, 2)
  + " toolong "
  + Utils::Format(toolongcount, 6) + " "
  + Utils::Format(100.0*toolongcount/
                  (double)pct_expected_voters_, 6, 2)
  + Utils::Format(toolongcountplus10, 6) + " "
  + Utils::Format(100.0*toolongcountplus10/
                  (double)pct_expected_voters_, 6, 2)
  + Utils::Format(toolongcountplus20, 6) + " "
  + Utils::Format(100.0*toolongcountplus20/
                  (double)pct_expected_voters_, 6, 2)
  + "\n";
  // Utils::Output(outstring, out_stream, Utils::log_stream);
  wait_time_minutes_map.clear(); //Clears the wait time map.
  return toolongcount;
}
/******************************************************************************
 * Function 'ReadData'.
 *
 * This method is called by the simulation class to read the data for a 
 * Precinct using the input file scanner.  From this file, this instance of
 * OnePct saves variables which are used in the simulation and calculations
 *
 * Parameters:
 *   infile - the input stream from which to read
**/
void OnePct::ReadData(Scanner& infile) {
  // Test if function is executed.
  // Utils::log_stream << kTag << "FUNC: " << "ReadData() " 
  //                   << "EXECUTED." << endl;
  if (infile.HasNext()) {
    pct_number_ = infile.NextInt();
    pct_name_ = infile.Next();
    pct_turnout_ = infile.NextDouble();
    pct_num_voters_ = infile.NextInt();
    pct_expected_voters_ = infile.NextInt();
    pct_expected_per_hour_ = infile.NextInt();
    pct_stations_ = infile.NextInt();
    pct_minority_ = infile.NextDouble();
    int stat1 = infile.NextInt();
    int stat2 = infile.NextInt();
    int stat3 = infile.NextInt();
    stations_to_histo_.insert(stat1);
    stations_to_histo_.insert(stat2);
    stations_to_histo_.insert(stat3);
  }
} // void OnePct::ReadData(Scanner& infile)
/******************************************************************************
 * Function 'RunSimulationPct'.
 *
 * This method is used for running the simulation of one Precinct. It first 
 * finds the min and max station counts using the expected voters and the 
 * average time it takes to vote as given by the config. It then uses these
 * numbers to go through all the stations. It creates a map for histo, then
 * it goes through the number of iterations for a station, creates voters
 * for that station, copies the backup from that method to a pending vector, 
 * clears vector of voters currently and done voting, then runs a second 
 * simulation for a specific station count. It then uses the DoStatitics
 * method to find the number of people that waited too long and, if that 
 * number is greater than 0, sets a value to continue to the next station.
 *
 * Using the Map for histo, now updated from DoStatistics, it sets the 
 * beginning and end of the map to the time lower and time upper. It 
 * then uses these values to set the voters per star. It then uses the time
 * and voters per star to create a string of stars to save to the outstring. 
 * It then, depending on the value decided by the wait time, repeats the 
 * process for the next station. 
 *
 * Parameters:
 *   config - the instance of Configuration() to use
 *   random - the instance of MyRandom() to use
 *   out_stream - "the output stream to which to write" - Duncan Buell,
 *                gameplay.cc, buellduncan_hw4
**/
void OnePct::RunSimulationPct(const Configuration& config, MyRandom& random, 
                              ofstream& out_stream) {
  // Test if function is executed.  
  // Utils::log_stream << kTag << "FUNC: " << "RunSimulationPct() " 
  //                   << "EXECUTED." << endl;
  string outstring = "XX";  
  // Setting min and max station count.
  int min_station_count = pct_expected_voters_ 
                        *  config.time_to_vote_mean_seconds_;
  int max_station_count = min_station_count  
                        + config.election_day_length_hours_;
  bool done_with_this_count = false;  
  // Reassigning min_station_count
  min_station_count = min_station_count
                    / (config.election_day_length_hours_*3600);
  if (min_station_count <= 0) { //If the station count ends up less than 0,
    min_station_count = 1;      //Sets the count to a useable 1
  }
////////////////////////////////////////////////////////////////////////////////
  for (int stations_count = min_station_count; 
       stations_count <= max_station_count; ++stations_count) {
    if (done_with_this_count) {
      break;
    }
    done_with_this_count = true;
    map<int, int> map_for_histo;
    outstring = kTag + this->ToString() + "\n";
    // Utils::Output(outstring, out_stream,  Utils::log_stream);
////////////////////////////////////////////////////////////////////////////////
    for (int iteration = 0; iteration < config.number_of_iterations_; 
         ++iteration) {     
      this->CreateVoters(config, random, out_stream);
      voters_pending_ = voters_backup_;
      voters_voting_.clear();
      voters_done_voting_.clear();
      this->RunSimulationPct2(stations_count);
      int number_too_long = DoStatistics(iteration, config, stations_count,
                                         map_for_histo, out_stream);
      if (number_too_long > 0) {
        done_with_this_count = false;
      }
    }
    voters_voting_.clear();
    voters_done_voting_.clear();
    outstring = kTag + "toolong space filler\n";
    // Utils::Output(outstring, out_stream, Utils::log_stream);
////////////////////////////////////////////////////////////////////////////////
    if (stations_to_histo_.count(stations_count) > 0) {
      outstring = "\n" + kTag + "HISTO " + 
                  this->ToString() + "\n";
      outstring += kTag + "HISTO STATIONS "
                + Utils::Format(stations_count, 4) + "\n";
     // Utils::Output(outstring, out_stream, Utils::log_stream);
      int time_lower = (map_for_histo.begin())->first;
      int time_upper = (map_for_histo.rbegin())->first;
      int voters_per_star = 1;
      if (map_for_histo[time_lower] > 50) {
        voters_per_star = map_for_histo[time_lower]
                        / (50 * config.number_of_iterations_);
        if (voters_per_star <= 0) {
          voters_per_star = 1;
        }
      }
////////////////////////////////////////////////////////////////////////////////
      for (int time = time_lower; time <= time_upper; ++time) {
        int count = map_for_histo[time];
        double count_double = static_cast<double>(count)
                            / static_cast<double>(config.number_of_iterations_);
        int count_divided_ceiling = static_cast<int>
          (ceil(count_double / voters_per_star));
        string stars = string(count_divided_ceiling, '*');
        outstring = kTag + "HISTO " + Utils::Format(time, 6) + ": "  
                  + Utils::Format(count_double, 7, 2) + ": ";
        outstring += stars + "\n";
        // Utils::Output(outstring, out_stream, Utils::log_stream);
      }
      outstring = "HISTO\n\n";
      // Utils::Output(outstring, out_stream, Utils::log_stream);
    }
  }
}
/******************************************************************************
* Function 'RunSimulationPct2'.
*
* This function uses the station count specified in the previous Run Simulation. 
* It uses an iterator to travel a vector of voters that are currently voting, 
* figures out which station a voter is at, saves that station to a free station
* vector and saves the voter to a done voter vector, then removes that voter
* from the currently voting vector. Following those voters, it then checks 
* voters that are pending, and allows them to vote in a free station and 
* assigns that station to the voter, followed by erasing that station from
* free stations. It also saves the voter's leave time. Finally, it checks if 
* the vector for voters voting or voters pending are empty. If not, it repeats
* he entire process. 
*
* Parameters:
*   stations_count - the total number of stations (pct_stations_)
**/
void OnePct::RunSimulationPct2(int stations_count) {
  // Test if function is executed.
  // Utils::log_stream << kTag << "FUNC: " << "RunSimulationPct2() " 
  //                   << "EXECUTED." << endl;
  free_stations_.clear();
  for (int i = 0; i < stations_count; ++i) {
    free_stations_.push_back(i); //Makes all stations currently free
  } 
  voters_voting_.clear(); //Makes sure both vectors are empty
  voters_done_voting_.clear();
  int second = 0;
  bool done = false;
  while (!done) {
    for (auto iter = voters_voting_.begin(); 
         iter != voters_voting_.end(); ++iter) {
      if (second == iter->first) {
        OneVoter one_voter = iter->second;
     // Creates a voter for a second and saves it under that second in the map
        int which_station = one_voter.GetStationNumber();
        free_stations_.push_back(which_station);
        voters_done_voting_.insert(std::pair<int, OneVoter>(second, one_voter));
      }
    }
    voters_voting_.erase(second); // Remove voter from currently voting. 
////////////////////////////////////////////////////////////////////////////////
    vector<map<int, OneVoter>::iterator > voters_pending_to_erase_by_iterator;
    for (auto iter = voters_pending_.begin(); 
         iter != voters_pending_.end(); ++iter) {
      if (second >= iter->first) {       // If they have already arrived
        if (free_stations_.size() > 0) { // and there are free stations
          OneVoter next_voter = iter->second;
          if (next_voter.GetTimeArrival() <= second) {
            int which_station = free_stations_.at(0);
            free_stations_.erase(free_stations_.begin());
            next_voter.AssignStation(which_station, second);
            int leave_time = next_voter.GetTimeDoneVoting();
            voters_voting_.insert(std::pair<int, OneVoter>
                                  (leave_time, next_voter));
            voters_pending_to_erase_by_iterator.push_back(iter);
             /* This was commented out 6 October 2016
              * Utils::log_stream << kTag << "ASSIGNED    "
              *                   << Utils::Format(second, 5) << ": "
              *                   << next_voter.ToString() << "\n";
              * Utils::log_stream << kTag << "PENDING, VOTING, DONE    "
              * << Utils::Format((int)voters_pending_.size(), 5) << ": "
              * << Utils::Format((int)voters_voting_.size(), 5) << ": "
              * << Utils::Format((int)voters_done_voting_.size(), 5) << endl;
             **/
          } // if (next_voter.GetTimeArrival() <= second) {
        } // if (free_stations_.size() > 0) {
      } else { // if (second == iter->first) {
        // we have walked in time past current time to arrivals in the future
        break; 
      }
    } // for (auto iter = voters_pending_.begin(); iter != voters_pending_.end(); ++iter) {
    for (auto iter = voters_pending_to_erase_by_iterator.begin();
         iter != voters_pending_to_erase_by_iterator.end(); ++iter) {
      voters_pending_.erase(*iter);
    }
    ++second;
    done = true;
    //if not empty, repeat the process
    if ((voters_pending_.size() > 0) || (voters_voting_.size() > 0)) {
      done = false;
    }
  } // while (!done) {
} // void Simulation::RunSimulationPct2()
/******************************************************************************
 * Function 'ToString'.
 *
 * Takes the number, name, turnout, number of voters, expeced voters, and 
 * expected voters per hour, as well as the station, minority and stations to
 * histo and saves it all to a string to be output to an output file. 
 *
 * Returns: string s - the string formatted for all the data. 
**/
string OnePct::ToString() {
  // Test if function is executed.
  // Utils::log_stream << kTag << "FUNC: " << "ToString() " 
  //                   << "EXECUTED." << endl;
  string s = "";
  s += Utils::Format(pct_number_, 4);
  s += " " + Utils::Format(pct_name_, 25, "left");
  s += Utils::Format(pct_turnout_, 8, 2);
  s += Utils::Format(pct_num_voters_, 8);
  s += Utils::Format(pct_expected_voters_, 8);
  s += Utils::Format(pct_expected_per_hour_, 8);
  s += Utils::Format(pct_stations_, 3);
  s += Utils::Format(pct_minority_, 8, 2);
  s += " HH ";
  for (auto iter = stations_to_histo_.begin(); 
       iter != stations_to_histo_.end(); ++iter) {
    s += Utils::Format(*iter, 4);
  }
  s += " HH";
  return s;
} // string OnePct::ToString()
// Commented out 27 November 2016, C.J. Waldron
// Description: Does not execute.
/******************************************************************************
* Parameterized Constructor.
*
OnePct::OnePct(Scanner& infile) {
  // Test if function is executed.
  // Utils::log_stream << kTag << "FUNC: " << "OnePct(infile) " 
  //                   << "EXECUTED." << endl;

  this->ReadData(infile);
}
**/
// Commented out 27 November 2016, C.J. Waldron
// Description: Does not execute.
/******************************************************************************
 * Function 'ToStringVoterMap'.
 *
 * Travels through the voter map and saves the label, followed by the size
 * of the map, then prints all the information of the each voter in the map.
 *
 * Parameters:
 *   label - the label given to a specific voter map
 *   themap - a map of voters
 *
 * Returns: 
 *    string s - formatted string of all the data
 *
string OnePct::ToStringVoterMap(string label, multimap<int, OneVoter> themap) {
  // Test if function is executed. 
  // Utils::log_stream << kTag << "FUNC: " << "ToStringVoterMap() " 
  //                   << "EXECUTED." << endl;

  string s = "";
  s += "\n" + label + " WITH " + Utils::Format((int)themap.size(), 6)
     + " ENTRIES\n";
  s += OneVoter::ToStringHeader() + "\n";
  for (auto iter = themap.begin(); iter != themap.end(); ++iter) {
    s += (iter->second).ToString() + "\n";
  }

  return s;
} // string OnePct::ToString()
**/
