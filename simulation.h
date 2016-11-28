/******************************************************************************
 * Header for the 'Simulation' class.
 *
 * Author/copyright:  Duncan Buell
 * Date: 19 July 2016
 *
**/

#ifndef SIMULATION_H
#define SIMULATION_H

#include <map>

#include "Utilities/scanline.h"
#include "Utilities/scanner.h"
#include "Utilities/utils.h"

#include "configuration.h"
#include "onepct.h"

using namespace std;

class Simulation
{
public:
/******************************************************************************
 * Constructors and destructors for the class. 
**/
  Simulation();
  virtual ~Simulation();

/******************************************************************************
 * General functions.
**/
  void ReadPrecincts(Scanner& infile);
  void RunSimulation(const Configuration& config, MyRandom& random, 
                     ofstream& out_stream);
  string ToString();
  // Commented out 27 November 2016, C.J. Waldron
  // No such function in Simulation.cc
  // string ToStringPcts();

private:
/******************************************************************************
 * Variables.
**/
  map<int, OnePct> pcts_;

/******************************************************************************
 * Private functions.
**/
};

#endif // SIMULATION_H
