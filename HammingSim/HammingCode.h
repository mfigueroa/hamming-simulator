#include <fstream>
#include <bitset>
#include <math.h>
#include <random>
#include <sstream>
#include <time.h>

void EncodeHamming(bool *message, bool(&codeword)[12]);
int * FindSyndrome(bool *codeword);
void TranslateToHamming(std::string originalFile, std::string newFile);
void DecodeHamming(int *syndrome, bool *codeword, bool *decodedMessage);
char BoolWordToByte(bool *message);