//
//  Resistor.cpp
//  Lab 3 The Resistor Network Program
// Author:
// Date:

#include "Resistor.h"

#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

Resistor::Resistor(string cname_, double cresistance_, int cendpoints_[2]) {
  name = cname_;
  resistance = cresistance_;
  endpointNodeIDs[2] = cendpoints_[2];
}
string Resistor::getName(){
  return name;
}

double Resistor::getResistance(){ 
  return resistance;}
void Resistor::setResistance(double resistance_){
  resistance = resistance_;
}

int Resistor::getOtherEndpoint(int nodeIndex){
  if (endpointNodeIDs[0] == nodeIndex) {
    return endpointNodeIDs[1];  
  } 
  else {
    return endpointNodeIDs[0];  
  }

}  

void Resistor::print() {
  cout << std::left << std::setw(20) << name << std::right << std::setw(8)
       << std::fixed << std::setprecision(2) << resistance << " Ohms "
       << endpointNodeIDs[0] << " -> " << endpointNodeIDs[1] << endl;
}
