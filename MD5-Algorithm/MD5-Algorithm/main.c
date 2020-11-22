#include<stdio.h>
//#include<cstring>
#include <string.h>
#include <ctype.h>
#include "md5.h"
//MD5 example
unsigned char* MD5_cases[] = {
	"",
	"The quick brown fox jumps over the lazy dog",
	"a",
	"abc",
	"message digest",
	"abcdefghijklmnopqrstuvwxyz",
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789",
	"12345678901234567890123456789012345678901234567890123456789012345678901234567890"
};

unsigned char* MD5_expected[] = {
	"d41d8cd98f00b204e9800998ecf8427e",
	"9e107d9d372bb6826bd81d3542a419d6",
	"0cc175b9c0f1b6a831c399e269772661",
	"900150983cd24fb0d6963f7d28e17f72",
	"f96b697d7cb7938d525a2f31aaf161d0",
	"c3fcd3d76192e4007dfb496cca67e13b",
	"d174ab98d277d9f5a5611c2c9f419d9f",
	"57edf4a22be3c955ac49da2e2107b67a"
};
//hmac_md5 example
unsigned char* HMAC_MD5_cases_keys[] = {
	"key",
	"key",
	"key",
	"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
	"12345678901234567890123456789012345678901234567890123456789012345678901234567890",
	"Jefe",
	"sysu",
	"sysu",
	"sysu12345caoxy7didhavenotlike4326094tekken4367originoftesigsn93586",
	"sysu12345caoxy7didhavenotlike4326094tekken4367originoftesigsn93586"
};

unsigned char* HMAC_MD5_cases_data[] = {
	"Hi There",
	"The quick brown fox jumps over the lazy dog",
	"Test Using Larger Than Block-Size Key and Larger Than One Block-Size Data",
	"Hi There",
	"The quick brown fox jumps over the lazy dog",
	"what do ya want for nothing?",
	"Hello World",
	"an iterative hash function breaks up a message into blocks of a fixed size and iterates over them with a compression function",
	"an iterative hash function breaks up a message into blocks of a fixed size and iterates over them with a compression function",
	"Hello World"
};
unsigned char* HMAC_MD5_expected[] = {
	"eb01ff92f00d651abcdd1f56f1a74725",
	"80070713463e7749b90c2dc24911e275",
	"93ec1c6a0bc2889f1e87da3f88f0fca6",
	"5d84b4bd8ce8797ffea3869fc0097e70",
	"02021d12d54c48fcfaf832345ed8904d",
	"750c783e6ab0b503eaa86e310a5db738",
	"29e94cd403ddc6e7e3bcea35f8e91579",
	"b2ea01d8bd414bd666c41f03aefc6bb2",
	"cad00d67e16f47f3258954485301c5d7",
	"7a0005e9e5f7c6e1a408eb6f4af74386"
};
int main() {
	printf("MD5 Test:\n\n\n\n");
	for (int i = 0; i < 8; i++)
	{
		unsigned char* myResult = MD5(MD5_cases[i],0);
		printf("Expected  : %s\n", MD5_expected[i]);
		printf("Got       : %s\n", myResult);
		if (strcmp(myResult, MD5_expected[i]) == 0)
			printf("Asserted equal!\n");
		else
			printf("Not equal!\n");
		printf("\n");
	}

	printf("\n\n\n\nHMAC_MD5 Test:\n\n\n\n");
	for (int i = 0; i < 10; i++)
	{
		unsigned char* key = HMAC_MD5_cases_keys[i];
		unsigned char* data = HMAC_MD5_cases_data[i];
		unsigned char* expeted = HMAC_MD5_expected[i];
		unsigned char* myResult=(unsigned char*)malloc(65);
		myResult[0] = '\n';
		
		unsigned char* s=HMAC(key, data, 64, &myResult);
		printf("Expected : %s\n", expeted);
		printf("Got      : %s\n", s);
		if (strcmp(s, expeted) == 0)
			printf("Asserted equal!\n");
		else
			printf("Not equal!\n");
		printf("\n");
	}


	
}