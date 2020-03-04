#include <IpFrame.hpp>
#include <sstream>
#include <iomanip>

using namespace std;

void IpFrame::fromBytes(const char* frameBytes)
{
	setVersion(frameBytes[0] >> 4);
	setIhl(frameBytes[0] & 0xF);
	setService(frameBytes[1]);
	setTotalLength(frameBytes[2] * 0xFF + frameBytes[3]);
	setId(frameBytes[4] * 0x100 + frameBytes[5]);
	setDf((frameBytes[6] >> 6) & 1);
	setMf((frameBytes[6] >> 7) & 1);
	setOffset((frameBytes[6] & 0b11111) * 0x10 + frameBytes[7]);
	setTtl(frameBytes[8]);
	setProtocol(frameBytes[9]);
	setCheckSum(frameBytes[10] * 0x100 + static_cast<uint8_t>(frameBytes[11]));
	setSourceAddress((frameBytes[12] << 24) + (frameBytes[13] << 16) + (frameBytes[14] << 8) + (frameBytes[15]));
	setDestinationAddress((frameBytes[16] << 24) + (frameBytes[17] << 16) + (frameBytes[18] << 8) + (frameBytes[19]));
	calculateCheckSum();
}

IpFrame::IpFrame(const char* frameBytes)
{
	fromBytes(frameBytes);
}

IpFrame::IpFrame(istream& input)
{
	char buffer[IP_STD_MIN_HEADER_LENGTH];
	input.read(buffer, IP_STD_MIN_HEADER_LENGTH);
	fromBytes(buffer);
	if (getTotalLength() > IP_STD_MIN_HEADER_LENGTH)
		input.read(options, getTotalLength() - IP_STD_MIN_HEADER_LENGTH);
}

IpFrame::~IpFrame()
{
	free(options);
}

void IpFrame::setVersion(const unsigned& v) { version = v; }
void IpFrame::setIhl(const unsigned& i) { ihl = i; };
void IpFrame::setService(const unsigned& t) { service = t; }
void IpFrame::setTotalLength(const unsigned& tl) { totalLength = tl; }
void IpFrame::setId(const unsigned& i) { id = i; }
void IpFrame::setDf(const bool& d) { df = d; }
void IpFrame::setMf(const bool& m) { mf = m; }
void IpFrame::setOffset(const unsigned& o) { offset = o;  }
void IpFrame::setTtl(const unsigned& t) { ttl = t; }
void IpFrame::setProtocol(const unsigned& p) { protocol = p; }
void IpFrame::setSourceAddress(const unsigned& sa) { sourceAddress = sa; }
void IpFrame::setDestinationAddress(const unsigned& da) { destinationAddress = da; }
void IpFrame::setCheckSum(const unsigned& cs) { checkSum = cs; }

unsigned IpFrame::getVersion() const { return version; }
unsigned IpFrame::getIhl() const { return ihl; }
unsigned IpFrame::getService() const { return service; }
unsigned IpFrame::getTotalLength() const { return totalLength; }
unsigned IpFrame::getId() const { return id; }
bool IpFrame::getDf() const { return df; }
bool IpFrame::getMf() const { return mf; }
unsigned IpFrame::getOffset() const { return offset; }
unsigned IpFrame::getTtl() const { return ttl; }
unsigned IpFrame::getProtocol() const { return protocol; }
unsigned IpFrame::getCheckSum() const { return checkSum; }
unsigned IpFrame::getCalculatedCheckSum() const { return calculatedCheckSum; }
const unsigned IpFrame::getDestinationAddress() const { return destinationAddress; }
const unsigned IpFrame::getSourceAddress() const { return sourceAddress; }
unsigned IpFrame::getPrecedence() const { return service >> 5; }

string IpFrame::getSourceAddressAsString() const
{
	return addressToString(getSourceAddress());
}

string IpFrame::getDestinationAddressAsString() const
{
	return addressToString(getDestinationAddress());
}

string IpFrame::getDelayAsString() const
{
	return getDelay() ? "Low delay" : "Normal delay";
}

string IpFrame::getThroughputAsString() const
{
	return getThroughput() ? "High throughput" : "Normal throughput";
}

string IpFrame::getReliabilityAsString() const
{
	return getReliability() ? "High reliability" : "Normal reliability";
}

unsigned IpFrame::getDelay() const
{
	return getService() >> 4 & 1;
};

unsigned IpFrame::getThroughput() const
{
	return getService() >> 3 & 1;
};

unsigned IpFrame::getReliability() const
{
	return getService() >> 2 & 1;
};

unsigned IpFrame::getReservedTosBits() const
{
	return getService() & 0b11;
}

string IpFrame::getPrecedenceAsString() const
{
	switch (getPrecedence()) {
		case 0b111:
			return "Network Control";
			break;
		case 0b110:
			return "Internetwork Control";
			break;
		case 0b101:
			return "CRITIC / ECP";
			break;
		case 0b100:
			return "Flash Override";
			break;
		case 0b011:
			return "Flash";
			break;
		case 0b010:
			return "Immediate";
			break;
		case 0b001:
			return "Priority";
			break;
		case 0b000:
			return "Routine";
			break;
		default:
			return "Unknown";
			break;
	}
}

string IpFrame::getProtocolAsString() const
{
	switch (getProtocol()) {

	}
}

/* address is a bit field */
string IpFrame::addressToString(const unsigned& address) const
{
	stringstream ss;
	ss << (address / 0x1000000);
	ss << '.';
	ss << (address / 0x10000 & 0x0000FF);
	ss << '.';
	ss << (address / 0x100 & 0x00FF);
	ss << '.';
	ss << (address & 0xFF);
	return ss.str();
}

void IpFrame::calculateCheckSum()
{

	unsigned long cs(0);
	cs += ~(getVersion() * 0x1000 + getIhl() * 0x100 + getService());
	cs += ~(getTotalLength());
	cs += ~(getId());
	cs += ~((getDf() << 14) + (getMf() << 13) + getOffset());
	cs += ~(getTtl() * 0x100 + getProtocol());
	cs = cs + cs / 0x10000;
	cs = cs & 0xFFFF;
	// checksum is 0x0000
	// one's complement = 0xFFFF
	cs += 0xFFFF;
	cs += ~(getSourceAddress() / 0x10000);
	cs += ~(getSourceAddress() & 0xFFFF);
	cs += ~(getDestinationAddress() / 0x10000);
	cs += ~(getDestinationAddress() & 0xFFFF);
	cs += (cs & 0xFFFF0000 >> 32);
	this->calculatedCheckSum = cs;
}

bool IpFrame::checksumIsOk() const
{
	return getCheckSum() == getCalculatedCheckSum();
}