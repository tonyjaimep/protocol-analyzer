#pragma once

#include <iostream>
#include <fstream>

#include <TcpFrame.hpp>

// measured in bits
#define IP_STD_VERSION_LENGTH 4
#define IP_STD_IHL_LENGTH 4
#define IP_STD_SERVICE_LENGTH 8
#define IP_STD_TOTAL_LENGTH_LENGTH 16
#define IP_STD_ID_LENGTH 16
#define IP_STD_OFFSET_LENGTH 13
#define IP_STD_TTL_LENGTH 8
#define IP_STD_PROTOCOL_LENGTH 8
#define IP_STD_CHECKSUM_LENGTH 16
#define IP_STD_ADDRESS_LENGTH 32
// in bytes
#define IP_STD_MIN_HEADER_LENGTH 20

enum {
	IP_PROTOCOL_HOPOPT,
	IP_PROTOCOL_ICMP,
	IP_PROTOCOL_IGMP,
	IP_PROTOCOL_GGP,
	IP_PROTOCOL_IP_IN_IP,
	IP_PROTOCOL_ST,
	IP_PROTOCOL_TCP,
	IP_PROTOCOL_CBT,
};

class IpFrame {
private:
	unsigned version : IP_STD_VERSION_LENGTH;
	unsigned ihl : IP_STD_IHL_LENGTH;
	unsigned service : IP_STD_SERVICE_LENGTH;
	unsigned totalLength : IP_STD_TOTAL_LENGTH_LENGTH;
	unsigned id : IP_STD_ID_LENGTH;
	bool df;
	bool mf;
	unsigned offset : IP_STD_OFFSET_LENGTH;
	unsigned ttl : IP_STD_TTL_LENGTH;
	unsigned protocol : IP_STD_PROTOCOL_LENGTH;
	unsigned checkSum : IP_STD_CHECKSUM_LENGTH;
	unsigned calculatedCheckSum : IP_STD_CHECKSUM_LENGTH;
	unsigned sourceAddress : IP_STD_ADDRESS_LENGTH;
	unsigned destinationAddress : IP_STD_ADDRESS_LENGTH;
	char* options;
	char* payload;

	TcpFrame* tcpFrame;

	unsigned getPayloadLength() const;
	unsigned getOptionsLength() const;
	std::string addressToString(const unsigned&) const;
	void calculateCheckSum();
	void constructPayload();

public:
	IpFrame();
	IpFrame(const char*);
	IpFrame(std::istream&);
	~IpFrame();

	void fromBytes(const char*);

	std::string getSourceAddressAsString() const;
	std::string getDestinationAddressAsString() const;
	std::string getPrecedenceAsString() const;
	std::string getDelayAsString() const;
	std::string getThroughputAsString() const;
	std::string getReliabilityAsString() const;
	std::string getProtocolAsString() const;

	// setters
	void setVersion(const unsigned&);
	void setIhl(const unsigned&);
	void setService(const unsigned&);
	void setTotalLength(const unsigned&);
	void setId(const unsigned&);
	void setDf(const bool&);
	void setMf(const bool&);
	void setOffset(const unsigned&);
	void setTtl(const unsigned&);
	void setProtocol(const unsigned&);
	void setCheckSum(const unsigned&);
	void setSourceAddress(const unsigned&);
	void setDestinationAddress(const unsigned&);
	void setOptions(const char*);
	void setPayload(const char*);

	// getters
	unsigned getVersion() const;
	unsigned getIhl() const;
	unsigned getService() const;
	unsigned getTotalLength() const;
	unsigned getId() const;
	bool getDf() const;
	bool getMf() const;
	unsigned getOffset() const;
	unsigned getTtl() const;
	unsigned getProtocol() const;
	unsigned getCheckSum() const;
	unsigned getCalculatedCheckSum() const;
	const unsigned getSourceAddress() const;
	const unsigned getDestinationAddress() const;
	unsigned getPrecedence() const;
	unsigned getDelay() const;
	unsigned getThroughput() const;
	unsigned getReliability() const;
	unsigned getReservedTosBits() const;
	const char* getOptions(void) const;
	const char* getPayload(void) const;
	const TcpFrame* getTcpFrame(void) const;

	bool checksumIsOk() const;
};
