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


#include "p1906-helper.h"
#include "ns3/simulator.h"
#include "ns3/packet.h"
#include "ns3/log.h"
#include "ns3/pointer.h"
#include <string>
#include "ns3/config.h"
#include "../model-core/p1906-net-device.h"
#include "../model-core/p1906-medium.h"
#include "../model-core/p1906-perturbation.h"
#include "../model-core/p1906-field.h"
#include "../model-core/p1906-specificity.h"
#include "../model-core/p1906-communication-interface.h"
#include "../model-core/p1906-transmitter-communication-interface.h"
#include "../model-core/p1906-receiver-communication-interface.h"
#include "../model-core/p1906-message-carrier.h"
#include "../model-em/p1906-em-message-carrier.h"
#include "../model-em/p1906-em-perturbation.h"
#include "../model-em/p1906-em-motion.h"
#include "../model-em/p1906-em-field.h"
#include "../model-em/p1906-em-specificity.h"
#include "../model-em/p1906-em-communication-interface.h"
#include "../model-em/p1906-em-transmitter-communication-interface.h"
#include "../model-em/p1906-em-receiver-communication-interface.h"
#include "../model-mol/p1906-mol-message-carrier.h"
#include "../model-mol/p1906-mol-perturbation.h"
#include "../model-mol/p1906-mol-motion.h"
#include "../model-mol/p1906-mol-field.h"
#include "../model-mol/p1906-mol-specificity.h"
#include "../model-mol/p1906-mol-communication-interface.h"
#include "../model-mol/p1906-mol-transmitter-communication-interface.h"
#include "../model-mol/p1906-mol-receiver-communication-interface.h"

#include "../model-motor/p1906-mol-motor.h"
#include "../model-motor/p1906-mol-motor-perturbation.h"
#include "../model-motor/p1906-mol-motor-motion.h"
#include "../model-motor/p1906-mol-motor-field.h"
#include "../model-motor/p1906-mol-motor-communication-interface.h"
#include "../model-motor/p1906-mol-motor-transmitter-communication-interface.h"
#include "../model-motor/p1906-mol-motor-receiver-communication-interface.h"

NS_LOG_COMPONENT_DEFINE ("P1906Helper");

namespace ns3 {

P1906Helper::P1906Helper (void)
{}

P1906Helper::~P1906Helper (void)
{}

void
P1906Helper::Connect (Ptr<Node> n, Ptr<P1906NetDevice> d, Ptr<P1906Medium> m, Ptr<P1906CommunicationInterface> c, Ptr<P1906Field> fi, Ptr<P1906Perturbation> p, Ptr<P1906Specificity> s)
{
  d->SetNode (n);
  n->AddDevice (d);
  c->SetP1906NetDevice (d);
  c->SetP1906Medium (m);
  c->GetP1906TransmitterCommunicationInterface ()->SetP1906Perturbation (p);
  c->GetP1906TransmitterCommunicationInterface ()->SetP1906Field (fi);
  c->GetP1906ReceiverCommunicationInterface ()->SetP1906Specificity (s);
  s->SetP1906CommunicationInterface (c);
  m->AddP1906CommunicationInterface (c);
}

void 
P1906Helper::EnableLogComponents (void)
{
  LogComponentEnable ("P1906NetDevice", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906Medium", LOG_LEVEL_ALL);

  LogComponentEnable ("P1906MessageCarrier", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906CommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906TransmitterCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906ReceiverCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906Field", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906Motion", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906Perturbation", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906Specificity", LOG_LEVEL_ALL);

  LogComponentEnable ("P1906EMMessageCarrier", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906EMCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906EMTransmitterCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906EMReceiverCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906EMField", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906EMMotion", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906EMPerturbation", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906EMSpecificity", LOG_LEVEL_ALL);


  LogComponentEnable ("P1906MOLMessageCarrier", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOLCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOLTransmitterCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOLReceiverCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOLField", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOLMotion", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOLPerturbation", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOLSpecificity", LOG_LEVEL_ALL);
  
  
  LogComponentEnable ("P1906MOL_Motor", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOL_MOTOR_Field", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOL_MOTOR_Motion", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOL_MOTOR_Perturbation", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOL_MOTOR_TransmitterCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOL_MOTOR_ReceiverCommunicationInterface", LOG_LEVEL_ALL);
  LogComponentEnable ("P1906MOL_MOTOR_CommunicationInterface", LOG_LEVEL_ALL);

}


} // namespace ns3
