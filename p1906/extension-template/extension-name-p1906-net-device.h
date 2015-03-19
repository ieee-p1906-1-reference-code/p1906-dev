/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *  Copyright � 2015 by IEEE.
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

#ifndef EXTENSION_NAME_P1906_NET_DEVICE_H
#define EXTENSION_NAME_P1906_NET_DEVICE_H

#include <string.h>
#include <ns3/node.h>
#include <ns3/address.h>
#include <ns3/net-device.h>
#include <ns3/callback.h>
#include <ns3/packet.h>
#include <ns3/traced-callback.h>
#include <ns3/ptr.h>
#include <list>
#include "ns3/p1906-net-device.h"

namespace ns3 {

/**
 * \ingroup P1906 framework
 *
 * \class P1906NetDevice
 *
 * \brief This class implements the network device that will handle all
 * the functionalities conceived within the P1906 framework. Basically, it is
 * conceived as a container of the communication interface, i.e., the object
 * storing the transmission entity and the reception entity. It enable the
 * interaction between low-layer components of the P1906 framework and
 * upper layers of the protocol stack.
 */

class ExtensionNameP1906NetDevice : public P1906NetDevice
{
public:
  static TypeId GetTypeId (void);

  ExtensionNameP1906NetDevice ();
  virtual ~ExtensionNameP1906NetDevice ();

  virtual void DoDispose (void);

};


} // namespace ns3

#endif /* EXTENSION_NAME_P1906_NET_DEVICE_H */
