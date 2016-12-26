#include "HammingCode.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <bitset>

TEST_CASE("Encode Hamming", "[EncodeHamming]") {
	// 
	bool message[8] = { true, false, false, true, true, false, true, false };
	bool codeword[12];
	bool e_codeword[12] = { false, true, true, true, false, false, true, false, true, false, true, false };

	EncodeHamming(message, codeword);

	REQUIRE(std::memcmp(codeword, e_codeword, 12) == 0);
}

TEST_CASE("Find Syndrome", "[FindSyndrome]") {
	bool codeword[12] = { false, true, true, true, false, false, true, false, true, true, true, false };
	int e_syndrome[4] = { 0, 1, 0, 1 };
	auto syndrome = FindSyndrome(codeword);

	REQUIRE(std::memcmp(syndrome, e_syndrome, 4) == 0);
}

TEST_CASE("Decode Hamming", "[DecodeHamming]") {
	bool e_decodedMessage[8] = { true, false, false, true, true, false, true, false };
	bool decodedMessage[8];
	bool codeword[12] = { false, true, true, true, false, false, true, false, true, true, true, false };

	auto syndrome = FindSyndrome(codeword);

	DecodeHamming(syndrome, codeword, decodedMessage);
	char dMessage = BoolWordToByte(decodedMessage);

	REQUIRE(std::memcmp(e_decodedMessage, decodedMessage, 8) == 0);
	REQUIRE(BoolWordToByte(decodedMessage) == (char)0x9A);
}