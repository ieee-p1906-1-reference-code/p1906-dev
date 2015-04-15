#/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#/*
# *  Copyright © 2014 by IEEE.
# *
# *  This source file is an essential part of IEEE P1906.1,
# *  Recommended Practice for Nanoscale and Molecular
# *  Communication Framework.
# *  Verbatim copies of this source file may be used and
# *  distributed without restriction. Modifications to this source
# *  file as permitted in IEEE P1906.1 may also be made and
# *  distributed. All other uses require permission from the IEEE
# *  Standards Department (stds-ipr@ieee.org). All other rights
# *  reserved.
# *
# *  This source file is provided on an AS IS basis.
# *  The IEEE disclaims ANY WARRANTY EXPRESS OR IMPLIED INCLUDING
# *  ANY WARRANTY OF MERCHANTABILITY AND FITNESS FOR USE FOR A
# *  PARTICULAR PURPOSE.
# *  The user of the source file shall indemnify and hold
# *  IEEE harmless from any damages or liability arising out of
# *  the use thereof.
# *
#  * Author: Stephen F Bush - GE Global Research
# *                      bushsf@research.ge.com
# *                      http://www.amazon.com/author/stephenbush
# */

set -x
set -e

# General microtubule orientation is governed in 3D space via spherical coordinates, orientationPhi and orientationTheta. A motor is transmitted and can bind to the microtubules to guide its motion. The results are stored in Mathematica files that can be Import[]'ed to display the microtubules and motor motion. Use setenv NS_LOG P1906MOL_MOTOR_Motion to display the motion details.

for orientationPhi in {0..3..1}
do
	../waf --run "motor-example --orientation-phi=${orientationPhi}" &> tmp_out
	mv ../tubes.mma ../tubes${orientationPhi}.mma
	mv ../float2destination_*.mma ../float2destination${orientationPhi}.mma
done
