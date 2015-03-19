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
 * Author: Giuseppe Piro - Telematics Lab Research Group
 *                         Politecnico di Bari
 *                         giuseppe.piro@poliba.it
 *                         telematics.poliba.it/piro
 */


#ifndef P1906_HELPER_H
#define P1906_HELPER_H

#include <string>
#include "ns3/object-factory.h"
#include "ns3/node-container.h"
#include <ns3/mobility-model.h>
#include "ns3/net-device-container.h"


namespace ns3 {

class P1906NetDevice;
class P1906Field;
class P1906Perturbation;
class P1906Specificity;
class P1906Medium;
class P1906CommunicationInterface;
class P1906Motion;

/**
 * \ingroup P1906 framework
 * \brief helps to manage and create nanomachines objects
 */
class P1906Helper
{
public:

  /**
   * \brief Create a the helper object for the P1906 framework
   */
   P1906Helper (void);
  ~P1906Helper (void);


  /**
   * Helper to enable all log components related to the P1906 framework
   */
  void EnableLogComponents (void);

  /**
   * Helper to connect components, attributes, and devices
   */
  void Connect (Ptr<Node>, Ptr<P1906NetDevice>, Ptr<P1906Medium> m, Ptr<P1906CommunicationInterface> c, Ptr<P1906Field>, Ptr<P1906Perturbation>, Ptr<P1906Specificity>);
};

} // namespace ns3

#endif /* P1906_HELPER_H */
