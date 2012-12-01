#include <uC++.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstdlib>
#include "MPRNG.h"
#include "config.h"
#include "student.h"
#include "watcard.h"
#include "office.h"
#include "bank.h"
#include "parent.h"
#include "vending.h"
#include "nameserver.h"
#include "plant.h"
#include "truck.h"
#include "printer.h"


using namespace std;

MPRNG mprng;  // random number generator

/*
 * Convert buffer to an unsigned int
 */
bool convert( unsigned int &val, char *buffer ) {
    int int_val = -1;
    stringstream ss( buffer );
    ss >> dec >> int_val;
    if (int_val < 0) {
        return false;
    }
    val = (unsigned int)int_val;
    return !ss.fail() &&
        string( buffer ).find_first_not_of( " ", ss.tellg() ) == string::npos;
}

/*
 * Prints usages message and exits
 */
void print_usage (char *cmd)
{
    cout <<
        "Usage: " <<
        cmd <<
        " [ config-file [ random-seed (> 0) ] ]" <<
        endl;
    exit(EXIT_FAILURE);
}

void uMain::main()
{
    string config_file("soda.config");
    unsigned int seed = (unsigned int)getpid();
    ifstream *fin;

    switch(argc)
    {
        case 3:
            if (!convert(seed, argv[2]) || seed == 0) print_usage(argv[0]);
        case 2:
            config_file = argv[1];
        case 1:
            try {
                fin = new ifstream(config_file.c_str(), ifstream::in);
            } catch (uFile::Failure) {
                cerr << "Error! could not open input file \"" << config_file << "\"" << endl;
                exit(EXIT_FAILURE);
            }
            break;
        default:
            print_usage(argv[0]);
    }
    delete fin;

    // reading config
    ConfigParms params;
    processConfigFile(config_file.c_str(), params);

    vector<VendingMachine *> machines;
    vector<Student *> students;

    // printer, bank, parent, office, name server, vending machines, plant, 
    // stduents
    Printer printer(params.numStudents, params.numVendingMachines, params.numCouriers);
    Bank bank(params.numStudents);
    Parent parent(printer, bank, params.numStudents, params.parentalDelay);
    WATCardOffice office(printer, bank, params.numCouriers);
    NameServer server(printer, params.numVendingMachines, params.numStudents);
    for (int i=0; i<params.numVendingMachines; i++) {
        machines.push_back(new VendingMachine(printer, server, i, params.sodaCost, params.maxStockPerFlavour));
    }
    BottlingPlant *plant = new BottlingPlant(printer, server, params.numVendingMachines, params.maxShippedPerFlavour, params.maxStockPerFlavour, params.timeBetweenShipments);
    for (int i=0; i<params.numStudents; i++) {
        students.push_back(new Student(printer, server, office, i, params.maxPurchases));
    }

    for (int i=0; i<params.numStudents; i++) {
        delete students[i];
    }

    delete plant;

    for (int i=0; i<params.numVendingMachines; i++) {
        delete machines[i];
    }

}
