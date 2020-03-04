#pragma once

#include <string>

// measured in bytes
#define TCP_MIN_HEADER_LENGTH 20

// measured in bits
#define TCP_PORT_SIZE 16
#define TCP_SEQUENCE_NUMBER_SIZE 32
#define TCP_ACKNOWLEDGEMENT_NUMBER_SIZE 32
#define TCP_DATA_OFFSET_SIZE 4
#define TCP_RESERVED_FIELD_SIZE 6
#define TCP_FLAGS_SIZE 6
#define TCP_WINDOW_SIZE 16
#define TCP_CHECKSUM_SIZE 16
#define TCP_URGENT_POINTER_SIZE 16

class TcpFrame
{
private:
	unsigned sourcePort : TCP_PORT_SIZE;
	unsigned destinationPort : TCP_PORT_SIZE;
	unsigned sequenceNumber : TCP_SEQUENCE_NUMBER_SIZE;
	unsigned acknowledgementNumber : TCP_ACKNOWLEDGEMENT_NUMBER_SIZE;
	unsigned dataOffset : TCP_DATA_OFFSET_SIZE;
	unsigned flags : TCP_FLAGS_SIZE;
	unsigned window : TCP_WINDOW_SIZE;
	unsigned checkSum : TCP_CHECKSUM_SIZE;
	unsigned calculatedCheckSum : TCP_CHECKSUM_SIZE;
	unsigned urgentPointer : TCP_URGENT_POINTER_SIZE;

	std::string portToString(const unsigned&) const;
	void calculateCheckSum();
public:
	TcpFrame();
	TcpFrame(const char*);
	~TcpFrame();

	void fromBytes(const char*);

	std::string getSourcePortAsString() const;
	std::string getDestinationPortAsString() const;
	std::string getFlagsAsString() const;

	void setSourcePort(const unsigned&);
	void setDestinationPort(const unsigned&);
	void setSequenceNumber(const unsigned&);
	void setAcknowledgementNumber(const unsigned&);
	void setDataOffset(const unsigned&);
	void setFlags(const unsigned&);
	void setWindow(const unsigned&);
	void setCheckSum(const unsigned&);
	void setUrgentPointer(const unsigned&);

	unsigned getSourcePort() const;
	unsigned getDestinationPort() const;
	unsigned getSequenceNumber() const;
	unsigned getAcknowledgementNumber() const;
	unsigned getDataOffset() const;
	unsigned getFlags() const;
	unsigned getWindow() const;
	unsigned getCheckSum() const;
	unsigned getUrgentPointer() const;

	unsigned getCalculatedCheckSum() const;

	bool checkSumIsOk() const;
};
