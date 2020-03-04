#pragma once

#include <iostream>
#include <fstream>

#include "IpFrame.hpp"

// ethernet II standard structures
// measured in octets
#define ETH_STD_PREAMBLE_LENGTH		0
#define ETH_STD_SFD_LENGTH			1
#define ETH_STD_ADDRESS_LENGTH		6
#define ETH_STD_ETHERTYPE_LENGTH	2
#define ETH_STD_MAX_PAYLOAD_LENGTH	1500
#define ETH_STD_MAX_FCS_LENGTH		4

#define ETHERTYPE_IPV4	0x0800

class EthernetFrame {

	static const unsigned PREAMBLE_LENGTH;
	static const unsigned SFD_LENGTH;
	static const unsigned ADDRESS_LENGTH;
	static const unsigned ETHERTYPE_LENGTH;
	static const unsigned MAX_PAYLOAD_LENGTH;
	static const unsigned MAX_FCS_LENGTH;

	private:
		char* preamble;
		char* destinationAddress;
		char* sourceAddress;
		char* type;
		char* payload;
		char* frameCheckSequence;
		IpFrame* ipFrame;

		const std::string addressToString(const char*) const;
		unsigned getPayloadLength(const char*) const;

		void init();
		void clean();

	public:
		// constructors / destructors
		EthernetFrame();
		EthernetFrame(std::istream& input);
		~EthernetFrame();

		const char* getPreamble() const;
		const char* getDestinationAddress() const;
		const char* getSourceAddress() const;
		const char* getType() const;
		const char* getPayload() const;
		const char* getFrameCheckSequence() const;

		const std::string getDestinationAddressAsString() const;
		const std::string getSourceAddressAsString() const;
		const std::string getEthertypeAsString() const;

		void setPreamble(const char*);
		void setDestinationAddress(const char*);
		void setSourceAddress(const char*);
		void setType(const char*);
		void setPayload(const char*);
		void setPayload(std::istream&);
		void setFrameCheckSequence(const char*);

		const IpFrame* getIpFrame() const;

		std::ofstream operator<< (EthernetFrame);
		std::ifstream operator>> (EthernetFrame);
};
