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

# This example is the first test of indepedent message carrier scheduling. 
# In this case motors operate over 10s intervals for 100s resulting in 10 samples of output information.

set -x
set -e

# Use setenv NS_LOG P1906MOL_MOTOR_Motion to display the motor motion detail and retrieve latency: "[propagation time]".

export NS_LOG='P1906MOL_MOTOR_CommunicationInterface=level_all|prefix_time'
export NS_LOG=$NS_LOG:'P1906MOL_MOTOR_ReceiverCommunicationInterface=level_all|prefix_time'
export NS_LOG=$NS_LOG:'P1906MOL_MOTORMedium=level_all|prefix_time'
export NS_LOG=$NS_LOG:'P1906MOL_MOTOR_Motion=level_all|prefix_time'

# Microtubule orientation is governed in 3D space via spherical coordinates, orientationPhi and orientationTheta. 
# Microtubule rigidity is controlled vi a persistence length argument. 
# A motor is injected at the origin and can bind and walk along microtubules to guide its motion. 
# In this example we hold persistence length constant and modify the orientation of the MTs in radians and can simulate the impact on motor propagation time.
# The results are stored in Mathematica files that can be Import[]'ed to display the microtubules and motor motion. 

for orientationPhi in 0.0 0.7854 1.5708 3.1416 4.7124
do
	../waf --run "motor-example --orientation-phi=${orientationPhi}" &> tmp_out
	mkdir -p ../MT-Orientation-${orientationPhi}
	mv tmp_out ../MT-Orientation-${orientationPhi}
	mv ../tubes-*.mma ../MT-Orientation-${orientationPhi}
	#mv ../move2Destination-*.mma ../MT-Orientation-${orientationPhi}
	mv ../volsurface_*.mma ../MT-Orientation-${orientationPhi}
	cat ../MT-Orientation-${orientationPhi}/tmp_out | awk '/propagation/ {print $4}' > ../MT-Orientation-${orientationPhi}/latencies.txt
done
