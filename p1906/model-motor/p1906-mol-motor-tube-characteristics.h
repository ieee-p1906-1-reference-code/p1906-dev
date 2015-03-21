/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2015 by IEEE.
 *
 *  This source file is an essential part of IEEE Std 1906.1,
 *  Recommended Practice for Nanoscale and Molecular
 *  Communication Framework.
 *  Verbatim copies of this source file may be used and
 *  distributed without restriction. Modifications to this source
 *  file as permitted in IEEE Std 1906.1 may also be made and
 *  distributed. All other uses require permission from the IEEE
 *  Standards Department (stds-ipr@ieee.org). All other rights
 *  reserved.
 *
 *  This source file is provided on an AS IS basis.
 *  The IEEE disclaims ANY WARRANTY EXPRESS OR IMPLIED INCLUDING
 *  ANY WARRANTY OF MERCHANTABILITY AND FITNESS FOR USE FOR A
 *  PARTICULAR PURPOSE.
 *  The user of the source file shall indemnify and hold
 *  IEEE harmless from any damages or liability arising out of
 *  the use thereof.
 *
 * Author: Stephen F Bush - GE Global Research
 *                      bushsf@research.ge.com
 *                      http://www.amazon.com/author/stephenbush
 */

#ifndef MOL_MOTOR_TUBE_CHARACTERISTICS
#define MOL_MOTOR_TUBE_CHARACTERISTICS

  //! systemic properties of the microtubule network
  struct tubeCharacteristcs_t
  {
    //! the volume of space in which tubes originate (nm^3)
    double volume;
	//! the mean tube length in nm
    double mean_tube_length;
	//! the mean angle between segments within a tube (deg)
    double mean_intra_tube_angle;
	//! the mean angle between tubes (deg)
    double mean_inter_tube_angle;
	//! the density of tube segments within the volume (segments/nm^3)
    double mean_tube_density;
	//! segment length (nm)
    double segLength;
	//! the total number of segments for all tubes
    size_t numSegments;
	//! the persistence length (nm)
    double persistenceLength;
	//! the number of segments per tube
    size_t segPerTube;
	//! the total number of tubes
	size_t numTubes;
	//! the structural entropy of all tubes in bits
    double se;
  };
  
  #endif /* MOL_MOTOR_TUBE_CHARACTERISTICS */