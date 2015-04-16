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


#ifndef P1906_MOL_MOTOR_MEDIUM_H
#define P1906_MOL_MOTOR_MEDIUM_H

#include "ns3/net-device.h"
#include "ns3/channel.h"
#include "ns3/packet.h"

#include "ns3/p1906-medium.h"

namespace ns3 {

class P1906CommunicationInterface;
class P1906MessageCarrier;
class P1906Field;
class P1906Motion;


/**
 * \ingroup P1906 framework
 *
 * \class P1906MOL_MOTORMedium
 *
 * \brief This class implements the medium of the P1906 framework.
 */
class P1906MOL_MOTORMedium : public P1906Medium
{
public:

  P1906MOL_MOTORMedium ();
  virtual ~P1906MOL_MOTORMedium ();

  static TypeId GetTypeId ();

  /**
   * \param xxx add parameters
   * The metod is is charge of delivering the message to the destination node
   */
  void HandleTransmission  (Ptr<P1906CommunicationInterface> src,
		                    Ptr<P1906MessageCarrier> message,
		                    Ptr<P1906Field> field);

};

}

#endif /* P1906_MOL_MOTOR_MEDIUM_H */
