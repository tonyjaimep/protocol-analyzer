#include <iostream>
#include <fstream>
#include <iomanip>

#include <EthernetFrame.hpp>

using namespace std;

int main()
{
	ifstream frameFile("test/frame.bin", ios_base::binary);

	EthernetFrame ef(frameFile);

	cout << "START ETHERNET FRAME" << endl;
	cout << "Source MAC Address: ";
	cout << ef.getSourceAddressAsString() << endl;
	cout << "Destination MAC Address: ";
	cout << ef.getDestinationAddressAsString() << endl;
	cout << "Type: " << ef.getEthertypeAsString() << endl;
	cout << "START IP HEADER:" << endl;
	cout << "\tVersion: " << hex << ef.getIpFrame()->getVersion() << endl;
	cout << "\tIHL: " << ef.getIpFrame()->getIhl() << endl;
	cout << "\tTYPE OF SERVICE" << endl;
	cout << "\t\t" << "Precedence: " << ef.getIpFrame()->getPrecedence() << " (" << ef.getIpFrame()->getPrecedenceAsString() << ")" << endl;
	cout << "\t\t" << "Delay: " << ef.getIpFrame()->getDelayAsString() << endl;
	cout << "\t\t" << "Throughput: " << ef.getIpFrame()->getThroughputAsString() << endl;
	cout << "\t\t" << "Reliability: " << ef.getIpFrame()->getReliabilityAsString() << endl;
	cout << "\t\t" << "Reserved bits: " << (ef.getIpFrame()->getReservedTosBits() ? "NOT ZERO" : "00 (OK)") << endl;
	cout << "\tTotal Length: " << dec << ef.getIpFrame()->getTotalLength() << " bytes" << endl;
	cout << "\tID (hex): " << hex << ef.getIpFrame()->getId() << endl;
	cout << "\tDF: " << ef.getIpFrame()->getDf();

	if (ef.getIpFrame()->getDf())
		cout << " (Don't fragment)";
	else
		cout << " (Is fragmented)";
	cout << endl;

	cout << "\tMF: " << ef.getIpFrame()->getMf();
	
	if (ef.getIpFrame()->getMf()) {
		cout << " (More frames)";
	} else {
		cout << " (Is last frame)";
	}

	cout << endl;

	cout << "\tOffset: " << ef.getIpFrame()->getOffset() << endl;

	cout << "\tChecksum (hex): " << hex << ef.getIpFrame()->getCheckSum() << endl;
	cout << "\tCalculated checksum (hex): " << hex << ef.getIpFrame()->getCalculatedCheckSum() << endl;
	cout << '\t';
	if (ef.getIpFrame()->checksumIsOk())
		cout << "Checksum OK" << endl;
	else
		cout << "CHECKSUM NOT MATCHED" << endl;

	cout << "\tTTL: " << dec << ef.getIpFrame()->getTtl() << "s" << endl;
	cout << "\tProtocol (hex): " << hex << ef.getIpFrame()->getProtocol()
		<< "(" << ef.getIpFrame()->getProtocolAsString() << ")" << endl;
	cout << "\tSource Address: " << ef.getIpFrame()->getSourceAddressAsString() << endl;
	cout << "\tDestination Address: " << ef.getIpFrame()->getDestinationAddressAsString() << endl;

	cout << "END IP FRAME" << endl;

	return EXIT_SUCCESS;
}
