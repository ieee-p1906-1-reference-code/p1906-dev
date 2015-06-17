%/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
%/*
% *  Copyright � 2015 by IEEE.
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


em = load(strcat('RES_EM'));    
mol = load(strcat('RES_MOL'));    

d=em(:,1);


figure('Name','Channel capacity (EM vs MOL)')
loglog(d,em(:,2), '-or');
hold on
loglog(d,mol(:,2), '--b');
ylabel ('Channel capacity [bps]',  'fontsize', 30);
xlabel ('Distance [m]',  'fontsize', 30);
set(gca, 'fontsize', 30)
legend ('EM communication', 'Molecular communication')
grid
xlim([1e-10 1e0])
ylim([1e-10 1e15])