/******************************************************************************
 * Header for the 'Simulation' class.
 *
 * Runs simulations on each precinct provided
 * using OnePct::ReadData() and OnePct::RunSimulationPct()
 * 
 * Author/copyright:  Duncan Buell
 * Modified and used with permission by: Group 2, 1 December 2016
 *   Abraham Khan
 *   Thomas O'Hara
 *   Stephen Page
 *   C.J. Waldron
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
// Commented out 27 November 2016, C.J. Waldron
// Does not execute.
// string ToString();
// No such function in simulation.cc
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
