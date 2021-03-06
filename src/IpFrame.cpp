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

	// if packet has options
	if (getIhl() * getVersion() > 0x20) {
		unsigned optionsSize(getIhl() * getVersion() - 0x20);
		char optionsOnBytes[optionsSize];

		for (unsigned i(0); i < optionsSize; i++) {
			optionsOnBytes[i] = frameBytes[i + 19];
		}
		setOptions(optionsOnBytes);
	}

	// if there is a payload
	if (getIhl() * getVersion() > getTotalLength()) {
		const unsigned payloadStartByte = 19 + getIhl() * getVersion() - 0x20;
		char payloadOnBytes[getPayloadLength()];

		for (unsigned i(0); i < getPayloadLength(); i++) {
			payloadOnBytes[i] = frameBytes[i + payloadStartByte];
		}

		setPayload(payloadOnBytes);
	}
}

IpFrame::IpFrame(const char* frameBytes)
{
	fromBytes(frameBytes);
	constructPayload();
}

IpFrame::IpFrame(istream& input)
{
	char buffer[IP_STD_MIN_HEADER_LENGTH];
	input.read(buffer, IP_STD_MIN_HEADER_LENGTH);
	fromBytes(buffer);

	// if packet has options
	if (getIhl() * getVersion() > 0x20) {
		const unsigned optionsSize(getIhl() * getVersion() - 0x20);
		input.read(options, optionsSize);
	}

	// if there is a payload
	if (getIhl() * getVersion() > getTotalLength()) {
		input.read(payload, getPayloadLength());
	}

	constructPayload();
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

void IpFrame::setPayload(const char* bytes)
{
	for (unsigned i(0); i < getPayloadLength(); i++)
		payload[i] = bytes[i];

}

void IpFrame::setOptions(const char* bytes)
{
	for (unsigned i(0); i < getOptionsLength(); i++)
		options[i] = bytes[i];
}

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
const char* IpFrame::getPayload() const { return payload; }

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
	case IP_PROTOCOL_HOPOPT:
		return "HOPOPT";
		break;
	case IP_PROTOCOL_ICMP:
		return "ICMP";
		break;
	case IP_PROTOCOL_IGMP:
		return "IGMP";
		break;
	case IP_PROTOCOL_GGP:
		return "GGP";
		break;
	case IP_PROTOCOL_IP_IN_IP:
		return "IP-in-IP";
		break;
	case IP_PROTOCOL_ST:
		return "ST";
		break;
	case IP_PROTOCOL_TCP:
		return "TCP";
		break;
	default:
		return "Unknown";
		break;
	}
}

unsigned IpFrame::getPayloadLength() const
{
	return getTotalLength() - getIhl() * getVersion();
}

unsigned IpFrame::getOptionsLength() const
{
	return getIhl() * getVersion() - 0x20;
}

void IpFrame::constructPayload()
{
	switch (getProtocol()) {
	case IP_PROTOCOL_TCP:
		tcpFrame = new TcpFrame(getPayload());
		break;
	}
}

const TcpFrame* IpFrame::getTcpFrame() const
{
	return tcpFrame;
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
	unsigned long cs;
	cs = ~(getVersion() * 0x1000 + getIhl() * 0x100 + getService());
	cs += ~(getTotalLength());
	cs += ~(getId());
	cs += ~((getDf() << 14) + (getMf() << 13) + getOffset());
	cs += ~(getTtl() * 0x100 + getProtocol());
	cs += ~(0xFFFF);
	cs += ~(getSourceAddress() / 0x10000);
	cs += ~(getSourceAddress() & 0xFFFF);
	cs += ~(getDestinationAddress() / 0x10000);
	cs += ~(getDestinationAddress() & 0xFFFF);
	cs += ((cs & 0xFFFF0000) >> 32);
	this->calculatedCheckSum = cs;
}

bool IpFrame::checksumIsOk() const
{
	return getCheckSum() == getCalculatedCheckSum();
}
