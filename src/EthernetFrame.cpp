#include "EthernetFrame.hpp"

#include <iomanip>
#include <sstream>
#include <cstring>

using namespace std;

/* CLASS CONSTANTS */
const unsigned EthernetFrame::PREAMBLE_LENGTH = ETH_STD_PREAMBLE_LENGTH;
const unsigned EthernetFrame::SFD_LENGTH = ETH_STD_SFD_LENGTH;
const unsigned EthernetFrame::ADDRESS_LENGTH = ETH_STD_ADDRESS_LENGTH;
const unsigned EthernetFrame::ETHERTYPE_LENGTH = ETH_STD_ETHERTYPE_LENGTH;
const unsigned EthernetFrame::MAX_PAYLOAD_LENGTH = ETH_STD_MAX_PAYLOAD_LENGTH;
const unsigned EthernetFrame::MAX_FCS_LENGTH = ETH_STD_MAX_FCS_LENGTH;

/* CONSTRUCTORS AND DESTRUCTORS */

EthernetFrame::EthernetFrame()
{
	init();
}

EthernetFrame::EthernetFrame(istream& input)
{
	init();

	input.read(preamble, PREAMBLE_LENGTH);
	input.read(destinationAddress, ADDRESS_LENGTH);
	input.read(sourceAddress, ADDRESS_LENGTH);
	input.read(type, ETHERTYPE_LENGTH);
	setPayload(input);
	input.read(frameCheckSequence, MAX_FCS_LENGTH);
}

EthernetFrame::~EthernetFrame()
{
	clean();
}

/* INTIALIZATION AND CLEANING */

void EthernetFrame::init()
{
	preamble = static_cast<char*>(malloc(PREAMBLE_LENGTH));
	destinationAddress = static_cast<char*>(malloc(ADDRESS_LENGTH));
	sourceAddress = static_cast<char*>(malloc(ADDRESS_LENGTH));
	type = static_cast<char*>(malloc(ETHERTYPE_LENGTH));
	payload = static_cast<char*>(malloc(MAX_PAYLOAD_LENGTH));
	frameCheckSequence = static_cast<char*>(malloc(MAX_FCS_LENGTH));
}

void EthernetFrame::clean()
{
	free(preamble);
	free(destinationAddress);
	free(sourceAddress);
	free(type);
	free(payload);
	free(frameCheckSequence);
}

/* REGULAR GETTERS */
const char* EthernetFrame::getPreamble() const { return this->preamble; }
const char* EthernetFrame::getDestinationAddress() const { return this->destinationAddress; }
const char* EthernetFrame::getSourceAddress() const { return this->sourceAddress; }
const char* EthernetFrame::getType() const { return this->type; }
const char* EthernetFrame::getPayload() const { return this->payload; }
const char* EthernetFrame::getFrameCheckSequence() const { return this->frameCheckSequence; }

/* COMPUTED GETTERS */
const string EthernetFrame::getDestinationAddressAsString() const
{
	return addressToString(getDestinationAddress());
}

const string EthernetFrame::getSourceAddressAsString() const
{
	return addressToString(getSourceAddress());
}

const string EthernetFrame::getEthertypeAsString() const
{
	stringstream ss;

	unsigned ethType;
	ethType = type[0] * 0x100;
	ethType += type[1];

	ss << setfill('0') << setw(4) << hex << ethType;

	switch (ethType) {
	case ETHERTYPE_IPV4:
		ss << " (IPv4)";
		break;
	default:
		ss << " (UNKNOWN)";
		break;
	}
	return ss.str();
}

/* REGULAR SETTERS */
void EthernetFrame::setPreamble(const char* p) { memcpy(this->preamble, p, PREAMBLE_LENGTH); }
void EthernetFrame::setDestinationAddress(const char* d) { memcpy(this->destinationAddress, d, ADDRESS_LENGTH); }
void EthernetFrame::setSourceAddress(const char* s) { memcpy(this->sourceAddress, s, ADDRESS_LENGTH); }
void EthernetFrame::setType(const char* t) { memcpy(this->type, t, ETHERTYPE_LENGTH); }

void EthernetFrame::setPayload(const char* p)
{
	memcpy(this->payload, p, getPayloadLength(p));
}

void EthernetFrame::setFrameCheckSequence(const char* f) { memcpy(this->frameCheckSequence, f, MAX_FCS_LENGTH); }

/* SETTERS FROM INPUT STREAM */
void EthernetFrame::setPayload(istream& is)
{
	string p;
	char payloadBuffer[MAX_PAYLOAD_LENGTH - 4];

	is.get(payloadBuffer, 5);
	p.append(payloadBuffer, 4);

	unsigned length(getPayloadLength(p.c_str()));

	is.get(payloadBuffer, length - 4);

	p.append(payloadBuffer, length - 4);

	memcpy(this->payload, p.c_str(), length);

	ipFrame = new IpFrame(this->payload);
}

/* HELPERS */
const string EthernetFrame::addressToString(const char* bytes) const
{
	// unsigned numBytes = sizeof(bytes);
	stringstream resultStream;

	for (unsigned i(0); i < ADDRESS_LENGTH; i++) {
		resultStream << setfill('0') << setw(2) << hex << (unsigned int)static_cast<unsigned char>(bytes[i]);
		if (i + 1 < ADDRESS_LENGTH)
			resultStream << ":" ;
	}

	return resultStream.str();
}

unsigned EthernetFrame::getPayloadLength(const char* p) const
{
	return static_cast<unsigned char>(p[2]) * 0xFF + static_cast<unsigned char>(p[3]);
}

const IpFrame* EthernetFrame::getIpFrame() const {
	return this->ipFrame;
}