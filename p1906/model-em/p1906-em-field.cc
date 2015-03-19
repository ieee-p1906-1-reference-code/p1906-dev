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


#include "ns3/log.h"

#include "p1906-em-field.h"
#include "ns3/p1906-field.h"

namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("P1906EMField");

TypeId P1906EMField::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::P1906EMField")
    .SetParent<P1906Field> ();
  return tid;
}

P1906EMField::P1906EMField ()
{
  NS_LOG_FUNCTION (this);
  NS_LOG_FUNCTION (this << "Created EM Field Component");
}

P1906EMField::~P1906EMField ()
{
  NS_LOG_FUNCTION (this);
}

} // namespace ns3
