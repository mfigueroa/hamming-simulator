#include "HammingCode.h"
// Bit error probability
const double PROBABILITY = 0.05;

// Mersenne Twister 19937 Generator
std::random_device rd;
std::mt19937 mt(time(0));
std::uniform_real_distribution<double> dist(0, 1);

// Generator matrix
const int gMatrix[8][12] = {
	{ 1,1,1,0,0,0,0,0,0,0,0,0 },
	{ 1,0,0,1,1,0,0,0,0,0,0,0 },
	{ 0,1,0,1,0,1,0,0,0,0,0,0 },
	{ 1,1,0,1,0,0,1,0,0,0,0,0 },
	{ 1,0,0,0,0,0,0,1,1,0,0,0 },
	{ 0,1,0,0,0,0,0,1,0,1,0,0 },
	{ 1,1,0,0,0,0,0,1,0,0,1,0 },
	{ 0,0,0,1,0,0,0,1,0,0,0,1 }
};

// Parity-check matrix
const int pMatrix[4][12] = {
	{ 1,0,1,0,1,0,1,0,1,0,1,0 },
	{ 0,1,1,0,0,1,1,0,0,1,1,0 },
	{ 0,0,0,1,1,1,1,0,0,0,0,1 },
	{ 0,0,0,0,0,0,0,1,1,1,1,1 }
};

void EncodeHamming(bool *message, bool(&codeword)[12]) {
	for (int i = 0; i < 12; i++) {
		int result = 0;
		for (int j = 0; j < 8; j++) {
			result ^= gMatrix[j][i] * message[j];
		}
		codeword[i] = result;
	}
}

int * FindSyndrome(bool *codeword) {
	int *syndrome = new int[4];
	int result;

	for (int i = 0; i < 4; i++) {
		result = 0;
		for (int g = 0; g < 12; g++) {
			result ^= pMatrix[i][g] * codeword[g];
		}
		syndrome[i] = result;
	}
	return syndrome;
}

// Convert bool array to single char
char BoolWordToByte(bool *message) {
	char decodedWord = 0x0;
	for (int i = 0; i < 8; i++) {
		if (message[i])
			decodedWord |= 1 << 7 - i;
	}
	return decodedWord;
}

// Receives: 4-bit syndrome array, 12-bit codeword array
// Returns: 8-bit decodedMessage array
void DecodeHamming(int *syndrome, bool *codeword, bool *decodedMessage) {
	if (syndrome[0] + syndrome[1] + syndrome[2] + syndrome[3] == 0) {
		decodedMessage[0] = codeword[2];
		decodedMessage[1] = codeword[4];
		decodedMessage[2] = codeword[5];
		decodedMessage[3] = codeword[6];
		decodedMessage[4] = codeword[8];
		decodedMessage[5] = codeword[9];
		decodedMessage[6] = codeword[10];
		decodedMessage[7] = codeword[11];
	}
	// Case 2: Error detected, attempt single error correction
	else {
		// Find error position
		int pos = 0;
		int j = 1;
		for (int i = 0; i < 4; i++) {
			if (syndrome[i])
				pos += j;
			j = j << 1;
		}
		// Apply error correction
		if (pos < 12)
			codeword[pos - 1] = codeword[pos - 1] ^ 1;

		decodedMessage[0] = codeword[2];
		decodedMessage[1] = codeword[4];
		decodedMessage[2] = codeword[5];
		decodedMessage[3] = codeword[6];
		decodedMessage[4] = codeword[8];
		decodedMessage[5] = codeword[9];
		decodedMessage[6] = codeword[10];
		decodedMessage[7] = codeword[11];
	}
}

void TranslateToHamming(std::string originalFile, std::string newFile) {
	// Open binary files
	std::ifstream oldFH = std::ifstream();
	oldFH.open(originalFile, std::ifstream::binary);

	std::ofstream newFH = std::ofstream();
	newFH.open(newFile, std::ofstream::binary);

	while (oldFH.good()) {
		char binaryString;
		oldFH.read(&binaryString, 1);

		bool b8_BinaryString[8];
		for (int i = 0; i < 8; i++) {
			b8_BinaryString[7 - i] = (binaryString & (1 << i)) != 0;
		}
		bool b12_BinaryString[12];

		EncodeHamming(b8_BinaryString, b12_BinaryString);

		// Simulate 10% bit error
		for (int j = 0; j < 8; j++) {
			double rand_t = dist(mt);
			if (rand_t <= PROBABILITY)
				b12_BinaryString[j] = b12_BinaryString[j] ^ 1;
		}

		int *syndrome = FindSyndrome(b12_BinaryString);

		bool d_Message[8];
		DecodeHamming(syndrome, b12_BinaryString, d_Message);

		char decodedWord = BoolWordToByte(d_Message);

		newFH << decodedWord;
	}
	newFH.close();
	oldFH.close();
}