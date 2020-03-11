#include <iostream>
#include <fstream>
#include <iomanip>

#include <EthernetFrame.hpp>

using namespace std;

typedef enum {
	OPT_FILE=1,
	OPT_INTERFACE,
	OPT_EXIT
} MenuOption;

void clearScreen();

void analizeFile(string);
void analizeInterface();

MenuOption menu();

int main()
{
	MenuOption menuOption;

	do {
		menuOption = menu();
		clearScreen();
		switch (menuOption) {
		case OPT_FILE:
			analizeFile("test/frame.bin");
			break;
		case OPT_INTERFACE:
			analizeInterface();
			break;
		case OPT_EXIT:
			cout << "Exiting" << endl;
			break;
		default:
			cout << "Pick a valid option" << endl;
		}
	} while (menuOption != OPT_EXIT);
}

void analizeFile(string filename)
{
	ifstream frameFile(filename, ios_base::binary);
	if (!frameFile.is_open()) {
		cout << "Error opening file: " << filename << endl;
		return;
	}

	EthernetFrame ef(frameFile);
	const IpFrame* ipf(ef.getIpFrame());
	const TcpFrame* tcpf(ipf->getTcpFrame());

	cout << "START ETHERNET FRAME" << endl;
	cout << "Source MAC Address: ";
	cout << ef.getSourceAddressAsString() << endl;
	cout << "Destination MAC Address: ";
	cout << ef.getDestinationAddressAsString() << endl;
	cout << "Type: " << ef.getEthertypeAsString() << endl;
	cout << "START IP HEADER:" << endl;
	cout << "\tVersion: " << hex << ipf->getVersion() << endl;
	cout << "\tIHL: " << ipf->getIhl() << endl;
	cout << "\tTYPE OF SERVICE" << endl;
	cout << "\t\t" << "Precedence: " << ipf->getPrecedence() << " (" << ipf->getPrecedenceAsString() << ")" << endl;
	cout << "\t\t" << "Delay: " << ipf->getDelayAsString() << endl;
	cout << "\t\t" << "Throughput: " << ipf->getThroughputAsString() << endl;
	cout << "\t\t" << "Reliability: " << ipf->getReliabilityAsString() << endl;
	cout << "\t\t" << "Reserved bits: " << (ipf->getReservedTosBits() ? "NOT ZERO" : "00 (OK)") << endl;
	cout << "\tEND TYPE OF SERVICE" << endl;
	cout << "\tTotal Length: " << dec << ipf->getTotalLength() << " bytes" << endl;
	cout << "\tID (hex): " << hex << ipf->getId() << endl;
	cout << "\tDF: " << ipf->getDf();

	if (ipf->getDf())
		cout << " (Don't fragment)";
	else
		cout << " (Is fragmented)";
	cout << endl;

	cout << "\tMF: " << ipf->getMf();
	
	if (ipf->getMf()) {
		cout << " (More frames)";
	} else {
		cout << " (Is last frame)";
	}

	cout << endl;

	cout << "\tOffset: " << ipf->getOffset() << endl;

	cout << "\tChecksum (hex): " << hex << ipf->getCheckSum() << endl;
	cout << "\tCalculated checksum (hex): " << hex << ipf->getCalculatedCheckSum() << endl;
	cout << '\t';
	if (ipf->checksumIsOk())
		cout << "Checksum OK" << endl;
	else
		cout << "CHECKSUM NOT MATCHED" << endl;

	cout << "\tTTL: " << dec << ipf->getTtl() << "s" << endl;
	cout << "\tProtocol (hex): " << hex << ipf->getProtocol()
		<< " (" << ipf->getProtocolAsString() << ")" << endl;
	cout << "\tSource Address: " << ipf->getSourceAddressAsString() << endl;
	cout << "\tDestination Address: " << ipf->getDestinationAddressAsString() << endl;

	cout << "\tSTART TCP HEADER" << endl;
	cout << "\t\tSource Port: " << tcpf->getSourcePortAsString() << endl;
	cout << "\t\tDestination Port: " << tcpf->getDestinationPortAsString() << endl;
	cout << "\t\tSequence Number: " << tcpf->getSequenceNumber() << endl;
	cout << "\t\tAcknowledgement Number: " << tcpf->getAcknowledgementNumber() << endl;
	cout << "\t\tData Offset: " << tcpf->getDataOffset() << endl;
	cout << "\t\tFlags: " << tcpf->getFlagsAsString() << endl;
	cout << "\t\tWindow: " << tcpf->getWindow() << endl;
	cout << "\t\tCheckSum: " << tcpf->getCheckSum() << endl;
	cout << "\t\tUrgent Pointer: " << tcpf->getUrgentPointer() << endl;
	cout << "\tEND TCP HEADER" << endl;

	cout << "END IP HEADER" << endl;

	frameFile.close();
}

void analizeInterface()
{
	cout << "Working on it!" << endl;
}

MenuOption menu()
{
	MenuOption option;
	unsigned optionBuffer;
	cout << "------------------------------" << endl;
	cout << "\t\tMENU:" << endl;
	cout << "------------------------------" << endl;
	cout << OPT_FILE << ") Analize a file" << endl;
	cout << OPT_INTERFACE << ") Analize interface traffic" << endl;
	cout << OPT_EXIT << ") Exit" << endl;
	cout << "Choose an option: ";
	cin >> optionBuffer;
	option = static_cast<MenuOption>(optionBuffer);
	return option;
}

void clearScreen()
{
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}
