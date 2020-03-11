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
	cout << "\tEND TYPE OF SERVICE" << endl;
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
		<< " (" << ef.getIpFrame()->getProtocolAsString() << ")" << endl;
	cout << "\tSource Address: " << ef.getIpFrame()->getSourceAddressAsString() << endl;
	cout << "\tDestination Address: " << ef.getIpFrame()->getDestinationAddressAsString() << endl;

	cout << "END IP FRAME" << endl;

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
