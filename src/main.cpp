// Copyright (c) 2013, Jan Winkler <winkler@cs.uni-bremen.de>
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//     * Neither the name of Universität Bremen nor the names of its
//       contributors may be used to endorse or promote products derived from
//       this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include <iostream>

#include "cflie/CCrazyflie.h"

using namespace std;


int main(int argc, char **argv) {
  int nReturnvalue = 0;
  int nThrust = 0;//10001;
  
  string strRadioURI = "radio://0/10/250K";
  
  cout << "Opening radio URI '" << strRadioURI << "'" << endl;
  CCrazyRadio *crRadio = new CCrazyRadio(strRadioURI);
  
  bool bDongleConnected = false;
  bool bGoon = true;
  bool bDongleNotConnectedNotified = false;
  
  while(bGoon) {
    // Is the dongle connected? If not, try to connect it.
    if(!bDongleConnected) {
      while(!crRadio->startRadio()) {
	if(!bDongleNotConnectedNotified) {
	  cout << "Waiting for dongle." << endl;
	  bDongleNotConnectedNotified = true;
	}
	
	sleep(0.5);
      }
      
      cout << "Dongle connected, radio started." << endl;
    }
    
    bDongleNotConnectedNotified = false;
    bDongleConnected = true;
    
    CCrazyflie *cflieCopter = new CCrazyflie(crRadio);
    cflieCopter->setThrust(nThrust);
    
    while(bGoon && bDongleConnected) {
      if(cflieCopter->cycle()) {
	if(cflieCopter->copterInRange()) {
	  cout << "In range" << endl;
	} else {
	  cout << "Not in range" << endl;
	}
      } else {
	cerr << "Connection to radio dongle lost." << endl;
	bDongleConnected = false;
      }
    }
    
    delete cflieCopter;
  }
  
  cout << "Cleaning up" << endl;
  delete crRadio;
  
  return nReturnvalue;
}
