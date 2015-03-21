/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2014 by IEEE.
 *
 *  This source file is an essential part of IEEE P1906.1,
 *  Recommended Practice for Nanoscale and Molecular
 *  Communication Framework.
 *  Verbatim copies of this source file may be used and
 *  distributed without restriction. Modifications to this source
 *  file as permitted in IEEE P1906.1 may also be made and
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


#ifndef P1906_MOL_MOTOR_TRANSMITTER_COMMUNICATION_INTERFACE
#define P1906_MOL_MOTOR_TRANSMITTER_COMMUNICATION_INTERFACE

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"
#include "ns3/p1906-communication-interface.h"
#include "ns3/p1906-transmitter-communication-interface.h"

namespace ns3 {

class Packet;
class P1906Perturbation;
class P1906CommunicationInterface;
class P1906Field;
class P1906Force;
class P1906Medium;
class P1906NetDevice;


/**
 * \ingroup P1906 framework
 *
 * \class P1906MOL_MOTOR_TransmitterCommunicationInterface
 *
 * \brief Base class implementing the Transmitter entity in
 * the P1906 framework for the MOLECULAR Example
 */

class P1906MOL_MOTOR_TransmitterCommunicationInterface : public P1906TransmitterCommunicationInterface
{
public:
  static TypeId GetTypeId (void);

  P1906MOL_MOTOR_TransmitterCommunicationInterface ();
  virtual ~P1906MOL_MOTOR_TransmitterCommunicationInterface();

};

}

#endif /* P1906_MOL_MOTOR_TRANSMITTER_COMMUNICATION_INTERFACE */
