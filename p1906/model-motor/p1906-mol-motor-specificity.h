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


#ifndef P1906MOL_MOTOR_SPECIFICITY
#define P1906MOL_MOTOR_SPECIFICITY

#include "ns3/object.h"
#include "ns3/nstime.h"
#include "ns3/ptr.h"

#include "ns3/uinteger.h"
#include "ns3/double.h"
#include "ns3/traced-value.h"
#include "ns3/trace-source-accessor.h"

#include "ns3/p1906-specificity.h"

namespace ns3 {

/**
 * \ingroup P1906 framework
 *
 * \class P1906MOL_MOTORSpecificity
 *
 * \brief Base class implementing the Specificity component of
 * the P1906 framework, dedicated to the MOLECULAR Example
 */

class P1906MOL_MOTORSpecificity : public P1906Specificity
{
public:
  static TypeId GetTypeId (void);

  P1906MOL_MOTORSpecificity ();
  virtual ~P1906MOL_MOTORSpecificity ();

  virtual bool CheckRxCompatibility (Ptr<P1906CommunicationInterface> src, Ptr<P1906CommunicationInterface> dst, Ptr<P1906MessageCarrier> message);

  void SetDiffusionCoefficient (double d);
  double GetDiffusionConefficient (void);

private:
  double m_diffusionCoefficient;

  double m_sensitivity;
  double m_specificity;
};

}

#endif /* P1906MOL_MOTOR_SPECIFICITY */
