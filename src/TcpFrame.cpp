#include <sstream>
#include <iomanip>
#include <TcpFrame.hpp>

using namespace std;

void TcpFrame::fromBytes(const char* frameBytes)
{
	setSourcePort(frameBytes[0] * 0x100 + frameBytes[1]);
	setDestinationPort(frameBytes[2] * 0x100 + frameBytes[3]);
	setSequenceNumber(frameBytes[4] * 0x1000000 + frameBytes[5] * 0x10000 + frameBytes[6] * 0x100 + frameBytes[7]);
	setAcknowledgementNumber(frameBytes[8] * 0x1000000 + frameBytes[9] * 0x10000 + frameBytes[10] * 0x100 + frameBytes[11]);
	setDataOffset(frameBytes[12] >> 4);
	setFlags(frameBytes[13] & 0b111111);
	setWindow(frameBytes[14] * 0x100 + frameBytes[15]);
	setCheckSum(frameBytes[16] * 0x100 + frameBytes[17]);
	setUrgentPointer(frameBytes[18] * 0x100 + frameBytes[19]);
	calculateCheckSum();
}

TcpFrame::TcpFrame(const char* frameBytes)
{
	fromBytes(frameBytes);
}

TcpFrame::~TcpFrame() {}

std::string TcpFrame::getSourcePortAsString() const
{
	return portToString(getSourcePort());
}

std::string TcpFrame::getDestinationPortAsString() const
{
	return portToString(getDestinationPort());
}

std::string TcpFrame::getFlagsAsString() const
{
	return "FLAGS";
}

std::string TcpFrame::portToString(const unsigned& port) const
{
	stringstream ss;
	ss << "::" << port;
	return ss.str();
}

void TcpFrame::setSourcePort(const unsigned& s) { sourcePort = s; };
void TcpFrame::setDestinationPort(const unsigned& d) { destinationPort = d; };
void TcpFrame::setSequenceNumber(const unsigned& s) { sequenceNumber = s; };
void TcpFrame::setAcknowledgementNumber(const unsigned& a) { acknowledgementNumber = a; };
void TcpFrame::setDataOffset(const unsigned& d) { dataOffset = d; };
void TcpFrame::setFlags(const unsigned& f) { flags = f; };
void TcpFrame::setWindow(const unsigned& w) { window = w; };
void TcpFrame::setCheckSum(const unsigned& c) { checkSum = c; };
void TcpFrame::setUrgentPointer(const unsigned& u) { urgentPointer = u; };

unsigned TcpFrame::getSourcePort() const { return sourcePort; } 
unsigned TcpFrame::getDestinationPort() const { return destinationPort; }
unsigned TcpFrame::getSequenceNumber() const { return sequenceNumber; }
unsigned TcpFrame::getAcknowledgementNumber() const { return acknowledgementNumber; }
unsigned TcpFrame::getDataOffset() const { return dataOffset; }
unsigned TcpFrame::getFlags() const { return flags; }
unsigned TcpFrame::getWindow() const { return window; }
unsigned TcpFrame::getCheckSum() const { return checkSum; }
unsigned TcpFrame::getUrgentPointer() const { return urgentPointer; }
unsigned TcpFrame::getCalculatedCheckSum() const { return calculatedCheckSum; }

void TcpFrame::calculateCheckSum()
{
	unsigned long cs(0);
	this->calculatedCheckSum = cs;
}

bool TcpFrame::checkSumIsOk() const
{
	return getCheckSum() == getCalculatedCheckSum();
}
