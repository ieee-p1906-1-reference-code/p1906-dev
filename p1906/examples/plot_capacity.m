%/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
%/*
% *  Copyright � 2014 by IEEE.
% *
% *  This source file is an essential part of IEEE P1906.1,
% *  Recommended Practice for Nanoscale and Molecular
% *  Communication Framework.
% *  Verbatim copies of this source file may be used and
% *  distributed without restriction. Modifications to this source
% *  file as permitted in IEEE P1906.1 may also be made and
% *  distributed. All other uses require permission from the IEEE
% *  Standards Department (stds-ipr@ieee.org). All other rights
% *  reserved.
% *
% *  This source file is provided on an AS IS basis.
% *  The IEEE disclaims ANY WARRANTY EXPRESS OR IMPLIED INCLUDING
% *  ANY WARRANTY OF MERCHANTABILITY AND FITNESS FOR USE FOR A
% *  PARTICULAR PURPOSE.
% *  The user of the source file shall indemnify and hold
% *  IEEE harmless from any damages or liability arising out of
% *  the use thereof.
% *
% * Author: Giuseppe Piro - Telematics Lab Research Group
% *                         Politecnico di Bari
% *                         giuseppe.piro@poliba.it
% *                         telematics.poliba.it/piro
% */

 
 
close all
clc
clear all

d = linspace(1e-6,0.1,1e2);

em = load(strcat('RES_EM'));    
mol = load(strcat('RES_MOL'));    

figure('Name','Channel capacity (EM vs MOL)')
semilogy(d,em(:,2), '--*r');
hold on
semilogy(d,mol(:,2), '-sb');
ylabel ('Channel capacity [bps]',  'fontsize', 14);
xlabel ('Distance [m]',  'fontsize', 14);
set(gca, 'fontsize', 14)
legend ('EM communication', 'Molecular communication')
grid