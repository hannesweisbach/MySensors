/*
 Disabled signing backend. Does not provide any security measures.
 
 Created by Patrick "Anticimex" Fallberg <patrick@fallberg.net>
*/
#include "MyConfig.h"
#include "MySigning.h"
#include "MySigningNone.h"

// Uncomment this to get some useful serial debug info (Serial.print and Serial.println expected)
//#define DEBUG_SIGNING

#ifdef DEBUG_SIGNING
#define DEBUG_SIGNING_PRINTLN(args) Serial.println(args)
#else
#define DEBUG_SIGNING_PRINTLN(args)
#endif

#ifdef DEBUG_SIGNING
static void DEBUG_NONE_PRINTBUF(const __FlashStringHelper* str, uint8_t* buf, uint8_t sz)
{
	int i;
	Serial.println(str);
	for (int i=0; i<sz; i++)
	{
		if (buf[i] < 0x10)
		{
			Serial.print('0'); // Because Serial.print does not 0-pad HEX
		}
		Serial.print(buf[i], HEX);
	}
	Serial.println();
}
#else
#define DEBUG_NONE_PRINTBUF(str, buf, sz)
#endif

MySigningNone::MySigningNone() : MySigning() {
}

bool MySigningNone::getNonce(MyMessage &msg) {
	return true;
}

bool MySigningNone::checkTimer() {
	return true;
}

bool MySigningNone::putNonce(MyMessage &msg) {
	return true;
}

bool MySigningNone::signMsg(MyMessage &msg) {
	// If we cannot fit any signature in the message, refuse to sign it
	if (mGetLength(msg) > MAX_PAYLOAD-2) {
		DEBUG_SIGNING_PRINTLN(F("MTOL")); // Message too large for signature to fit
		return false; 
	}
	mSetSigned(msg, 1); // make sure signing flag is set before signature is calculated
	msg.data[mGetLength(msg)] = SIGNING_IDENTIFIER; // Add signing identifier
	return true;
}

bool MySigningNone::verifyMsg(MyMessage &msg) {
	if (msg.data[mGetLength(msg)] != SIGNING_IDENTIFIER)
		return false;
	else
		return true;
}