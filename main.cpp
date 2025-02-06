#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "Node.h"
#include "Resistor.h"

using namespace std;

Resistor** resistors =
    nullptr;  // Pointer that should point to an array of Resistor pointers
Node* nodes = nullptr;  // pointer that should hold address to an array of Nodes
int maxNodeNumber = 0;  // maximum number of nodes as set in the command line
int maxResistors = 0;  // maximum number of resistors as set in the command line
int resistorsCount = 0;  // count the number of resistors

string errorArray[10] = {
    "invalid command",                                  // 0
    "invalid argument",                                 // 1
    "negative resistance",                              // 2
    "node value is out of permitted range",             // 3
    "resistor name cannot be keyword \"all\"",          // 4
    "both terminals of resistor connect to same node",  // 5
    "too few arguments",                                // 6
};

// Function Prototypes
bool getInteger(stringstream& ss, int& x);
bool getString(stringstream& ss, string& s);
bool getDouble(stringstream& ss, double& s);

void handleMaxVal(stringstream& ss);   // done
void handleInsertR(stringstream& ss);  // done
void handleModifyR(stringstream& ss);  // done
void handlePrintR(stringstream& ss);   // done
void handleDeleteR(stringstream& ss);  // done
void handleSetV(stringstream& ss);
int countTerms(const string& line);

int main() {
  string line, command;
  cout << ">>> ";
  cout.flush();
  // TODO: Implement the main function here
  while (getline(cin, line)) {
    
    if (cin.eof()) {
      break;
    }

    // Create a stringstream from the input line
    stringstream ss(line);

    // Extract the command (the first word in the line)
    ss >> command;

    // Check which command was entered and call the appropriate function
    if (command == "maxVal") {
      handleMaxVal(ss);
    } else if (command == "insertR") {
      handleInsertR(ss);
    } else if (command == "modifyR") {
      handleModifyR(ss);
    } else if (command == "setV") {
      handleSetV(ss);
    } else if (command == "printR") {
      handlePrintR(ss);
    } else if (command == "deleteR") {
      handleDeleteR(ss);
    } else {
      // If the command is not recognized, print "invalid command"
      cout << "Error: " << errorArray[0] << endl;
    }

    // Print the prompt for the next command
    cout << ">>> ";
    cout.flush();
  }

  return 0;
}
int countTerms(const string& line) {
  stringstream ss(line);
  string term;
  int count = 0;

  // Extract each term and count them
  while (ss >> term) {
    count++;
  }

  return count;
}
bool getInteger(stringstream& ss, int& x) {
  ss >> x;
  if (ss.fail()) {
    ss.clear();
    return false;
  }
  return true;
}

bool getString(stringstream& ss, string& s) {
  ss >> s;
  if (ss.fail()) {
    ss.clear();
    return false;
  }
  return true;
}

bool getDouble(stringstream& ss, double& s) {
  ss >> s;
  if (ss.fail()) {
    ss.clear();
    return false;
  }
  return true;
}

void handleMaxVal(stringstream& ss) {
  int nn, rc;  // nn = max node number, rc = max resistors
               /*string term;
               int count;
               while (ss >> term) {
                 count++;
               }
               if (count < 4) {
                 cout << "Error: " << errorArray[6] << endl;
                 return;
               }*/
  if ((countTerms(ss.str())) < 2) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }
  if (!getInteger(ss, nn)) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  if (nn <= 0) {
    cout << "Error: " << errorArray[3] << endl;
    return;
  }

  if (!getInteger(ss, rc)) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  if (rc <= 0) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  maxNodeNumber = nn;
  maxResistors = rc;
  // check and clear your memory

  if (nodes != nullptr) {
    delete[] nodes;
    nodes = nullptr;
  }
  if (resistors != nullptr) {
    for (int i = 0; i < resistorsCount; i++) {
      delete resistors[i];
    }
    delete[] resistors;
    resistors = nullptr;
  }

  resistors = new Resistor*[maxResistors]();
  nodes = new Node[maxNodeNumber + 1]();
  resistorsCount = 0;

  cout << "New network: max node number is " << maxNodeNumber
       << "; max resistors is " << maxResistors << endl;
}

void handleInsertR(stringstream& ss) {
  string name;
  double resistance;
  int nodeid1, nodeid2;

  if ((countTerms(ss.str())) < 5) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }

  if (!getString(ss, name)) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }

  if (name == "all") {
    cout << "Error: " << errorArray[4] << endl;
    return;
  }

  if (!getDouble(ss, resistance) ||
      (!getInteger(ss, nodeid1) || !getInteger(ss, nodeid2))) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  if (resistance < 0) {
    cout << "Error: " << errorArray[2] << endl;
    return;
  }

  if (nodeid1 < 1 || nodeid1 > maxNodeNumber || nodeid2 < 1 ||
      nodeid2 > maxNodeNumber) {
    cout << "Error: " << errorArray[3] << endl;
    return;
  }

  if (nodeid1 == nodeid2) {
    cout << "Error: " << errorArray[5] << endl;
    return;
  }

  if (!nodes[nodeid1].canAddResistor()) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  if (!nodes[nodeid2].canAddResistor()) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  nodes[nodeid1].addResistor(resistorsCount);
  nodes[nodeid2].addResistor(resistorsCount);

  int dum[2] = {nodeid1, nodeid2};

  resistors[resistorsCount] = new Resistor(name, resistance, dum);
  resistorsCount++;

  cout << "Inserted: resistor " << name << " " << fixed << setprecision(2)
       << resistance << " Ohms " << nodeid1 << " -> " << nodeid2 << endl;

  return;
}

void handleModifyR(stringstream& ss) {
  string name;
  double newResistance;

  if (!getString(ss, name)) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }

  if (name == "all") {
    cout << "Error: " << errorArray[4] << endl;
    return;
  }

  if (!getDouble(ss, newResistance)) {
    cout << "Error: " << errorArray[1] << endl;
    return;
  }

  if (newResistance < 0) {
    cout << "Error: " << errorArray[2] << endl;
    return;
  }

  double oldResistance;
  for (int i = 0; i < resistorsCount; ++i) {
    if (resistors[i] != nullptr && resistors[i]->getName() == name) {
      oldResistance = resistors[i]->getResistance();
      resistors[i]->setResistance(newResistance);
      cout << "Modified: resistor " << name << " from " << fixed
           << setprecision(2) << oldResistance << " Ohms to " << newResistance
           << " Ohms" << endl;
      return;  // Exit the function after modification
    }
  }
  // If no matching resistor is found, print an error message.
  cout << "Error: resistor name not found" << endl;
  return;
}

void handlePrintR(stringstream& ss) {
  string name;
  if (!getString(ss, name)) {
    cout << "Error: " << errorArray[6] << endl;
    return;
  }

  if (name == "all") {
    cout << "Error: " << errorArray[4] << endl;
    return;
  }
  for (int i = 0; i < resistorsCount; ++i) {
    if (resistors[i]->getName() == name) {
      resistors[i]->print();
    }
  }
}

void handleDeleteR(stringstream& ss) {
  string name;
  if (!getString(ss, name)) {
    cout << "Error: " << errorArray[0] << endl;
    return;
  }

  if (name != "all") {
    cout << "Error: " << errorArray[0] << endl;
    return;
  } else {
    for (int i = 0; i < resistorsCount; i++) {
      delete resistors[i];
    }
    delete[] resistors;
    resistors = nullptr;
    return;
  }
}
void handleSetV(stringstream& ss) {
  int nodeid;
  double voltage;

  if (!getInteger(ss, nodeid)) {
    cout << "Error: " << errorArray[1] << endl;  // Print "invalid argument"
    return;
  }

  // Validate that the node ID is within range
  if (nodeid < 1 || nodeid > maxNodeNumber) {
    cout << "Error: " << errorArray[3]
         << endl;  // Print "node value is out of permitted range"
    return;
  }

  // Extract the voltage value using getDouble
  if (!getDouble(ss, voltage)) {
    cout << "Error: " << errorArray[1] << endl;  // Print "invalid argument"
    return;
  }

  // Check for extra input beyond the required two arguments
  string extra;
  if (ss >> extra) {
    cout << "Error: " << errorArray[1] << endl;  // Print "invalid argument"
    return;
  }

  // Set the voltage for the specified node (Assume setVoltage is a method of
  // Node)
  nodes[nodeid].setVoltage(voltage);

  // Output the success message
  cout << "Set: node " << nodeid << " to " << fixed << setprecision(2)
       << voltage << " Volts" << endl;
}
