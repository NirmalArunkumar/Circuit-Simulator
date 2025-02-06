//
//  Node.cpp
//  Lab 3 The Resistor Network Program
// Author: 
// Date:
#include "Node.h"
#include <iostream>
#include <string>
#include "Resistor.h"

using namespace std;

Node::Node(){
    int numRes = 0 ;                              
  int resIDArray[MAX_RESISTORS_PER_NODE];
  for (int i = 0; i < MAX_RESISTORS_PER_NODE; ++i) {
        resIDArray[i] = 0;
    }
  double voltage = 0.0;                          
  bool voltageIsSet = false;
}

bool Node::canAddResistor(){
    return numRes<MAX_RESISTORS_PER_NODE;
}

void Node::addResistor(int rIndex){
    resIDArray[numRes] = rIndex;
    numRes++;
}

double Node::getVoltage(){return voltage;}

void Node::setVoltage(double voltage_){
    voltage = voltage_;
    voltageIsSet=true;
}
void Node::setVoltageInternally(double voltage_){
    voltage = voltage_;
}

bool Node::isVoltageSet(){
    return voltageIsSet;
}

int Node::getNumRes(){
    return numRes;
}

int* Node::getResIDArray(){
    return resIDArray;
}



