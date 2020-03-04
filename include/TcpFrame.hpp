#pragma once

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
	unsigned checksum : TCP_CHECKSUM_SIZE;
	unsigned urgentPointer : TCP_URGENT_POINTER_SIZE;
};