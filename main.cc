/******************************************************************************
 *3456789 123456789 123456789 123456789 123456789 123456789 123456789 123456789
 * Main program for simulation.
 *
 * The Main class sets up the input and output files for the program as well
 * as makes all major function calls. This program simulates a queue,
 * in this case an election. Thus, this program takes in a configuration
 * file, service time file, and a precinct file in order to compute 
 * information regarding the simulation of an election. The output info
 * outputs the submitted information(input) and a comparison between
 * the time taken too vote
 * with respects to what was considered " too long". 
 * 
 *
 * Author/copyright:  Duncan Buell. All rights reserved.
 * Date: 6 October 2016
 * Modified and used with permission by: Group 2, 1 December 2016
 *   Abraham Khan
 *   Thomas O'Hara
 *   Stephen Page
 *   C.J. Waldron
 *
**/
#include "main.h"

static const string kTag = "MAIN: ";

int main(int argc, char *argv[])
{
  string config_filename = "XX";
  string service_time_filename = "XX";
  string pct_filename = "XX";
  string log_filename = "XX";
  string out_filename = "XX";
  string outstring = "XX";
  // string timecall_output = "XX"; // Never used.

  ofstream out_stream;

  Scanner config_stream;
  Scanner pct_stream;
  Scanner service_stream;

  Configuration config;
  Simulation simulation;

  MyRandom random;

  cout<< kTag << "Beginning execution" << endl;

  Utils::CheckArgs(5, argc, argv,string("configfilename pctfilename") + 
		     string(" outfilename logfilename servicetimefilename"));
  
  config_filename = static_cast<string>(argv[1]);
  pct_filename = static_cast<string>(argv[2]);
  out_filename = static_cast<string>(argv[3]);
  log_filename = static_cast<string>(argv[4]);
  service_time_filename = static_cast<string>(argv[5]);
  

  Utils::FileOpen(out_stream, out_filename);
  Utils::LogFileOpen(log_filename);

  outstring = kTag + "Beginning execution\n";
  outstring += kTag + Utils::TimeCall("beginning");
  out_stream << outstring << endl;
  Utils::log_stream << outstring << endl;

  outstring = kTag + "outfile '" + out_filename + "'" + "\n";
  outstring += kTag + "logfile '" + log_filename + "'" + "\n";
  out_stream << outstring << endl;
  Utils::log_stream << outstring << endl;

  /////////////////////////////////////////////////////////////////////////////
  // config has RN seed, station count spread, election day length
  //   and mean and dev voting time
  config_stream.OpenFile(config_filename);
  config.ReadConfiguration(config_stream);
  config_stream.Close();

  service_stream.OpenFile(service_time_filename);
  config.ReadServiceTimes(service_stream);
  service_stream.Close();

  outstring = kTag + config.ToString() + "\n";
  out_stream << outstring << endl;
  Utils::log_stream << outstring << endl;

  random = MyRandom(config.seed_);

  //////////////////////////////////////////////////////////////////////////////
  // now read the precinct data
  pct_stream.OpenFile(pct_filename);
  simulation.ReadPrecincts(pct_stream);
  pct_stream.Close();

  //////////////////////////////////////////////////////////////////////////////
  // here is the real work
  simulation.RunSimulation(config, random, out_stream);

  //////////////////////////////////////////////////////////////////////////////
  // close up and go home
  outstring = kTag + "Ending execution" + "\n";
  outstring += kTag + Utils::TimeCall("ending");
  out_stream << outstring << endl;
  Utils::log_stream << outstring << endl;

  Utils::FileClose(out_stream);
  Utils::FileClose(Utils::log_stream);

  cout<< kTag << "Ending execution" << endl;

  return 0;
}
