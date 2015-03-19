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


#ifndef P1906_SPECIFICITY
#define P1906_SPECIFICITY

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

namespace ns3 {

class P1906MessageCarrier;
class P1906Motion;
class P1906CommunicationInterface;
/**
 * \ingroup P1906 framework
 *
 * \class P1906Specificity
 *
 * \brief Base class implementing the Specificity component of
 * the P1906 framework
 */

class P1906Specificity : public Object
{
public:
  static TypeId GetTypeId (void);

  P1906Specificity ();
  virtual ~P1906Specificity ();

  virtual bool CheckRxCompatibility (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message);

  void SetP1906CommunicationInterface (Ptr<P1906CommunicationInterface> i);
  Ptr<P1906CommunicationInterface> GetP1906CommunicationInterface (void);

private:
  Ptr<P1906CommunicationInterface> m_p1906CommunicationInterface;
};

}

#endif /* P1906_SPECIFICITY */
