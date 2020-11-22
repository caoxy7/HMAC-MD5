#pragma once
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// s specifies the per-round shift amounts
unsigned int S[64] = {
	7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
	5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
	4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
	6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21
};

// Use binary integer part of the sines of integers (Radians) as constants:
unsigned int K[64] = {
	//0~15
	0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
	0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
	0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
	0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
	//16~31
	0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
	0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
	0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
	0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
	//32~47
	0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
	0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
	0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
	0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
	//48~63
	0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
	0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
	0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
	0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};



unsigned int leftRotate(unsigned x, unsigned c) {
	return ((x << c) | (x >> (32 - c)));
}
//运算的时候按一个字节一个字节的来
void msgPadding(unsigned char* msg, int n, unsigned int** words, int* lengthWords) {
	//计算填充后的msg的总长
	//至少添加一位，最多添加512位
	//最终的总长要为64的倍数（算上补上数据长度后的总长）
	//+1是因为取余运算的原因，这里我们必须向上取，所以需要多补一位
	//（+8）的原因是为了保证留下64位（8字节）添加数据长度（如果加上这8没有超过64的倍数则没影响，超过了就说明原来的长度也不够，需要多添一点，比如原长若只有60字节，则不能保证留下64位（8字节）空间给数据长度，这个+8使得原长溢出了64，说明了长度不够需要多补一位）
	int block = (n + 8) / 64 + 1;
	int finalLen = block * 64;

	//先添一个位的1，之后全0
	unsigned char* paddedMsg = (unsigned char*)malloc(finalLen);
	for (int i = 0; i < n; i++)
		paddedMsg[i] = msg[i];
	paddedMsg[n] = (unsigned char)0x80;
	for (int i = n + 1; i < finalLen - 8; i++)
		paddedMsg[i] = 0;

	//padding数据长度到最后两个字
	unsigned long long tmp = (unsigned long long)n * 8;
	memcpy(paddedMsg + finalLen - 8, &tmp, 8);

	int wordsLen = finalLen / 4;
	*lengthWords = wordsLen;
	*words = (unsigned int*)malloc(finalLen);
	//printf("test1=%d\n", words);
	for (int i = 0; i < finalLen; i += 4)
	{
		unsigned int tmp = 0;
		for (int j = i; j <= i + 3; j++)
		{
			int sB = (j - i) * 8;
			tmp += ((unsigned int)paddedMsg[j]) << sB;
			
		}
		(*words)[i / 4] = tmp;
	}

	free(paddedMsg);
}
//参考wiki实现
void ProcessMsg(unsigned int* a0, unsigned int* b0, unsigned int* c0, unsigned int* d0,unsigned int* words, int lengthWords) {
	// Initialize variables:
	*a0 = 0x67452301;   // A
	*b0 = 0xefcdab89;   // B
	*c0 = 0x98badcfe;   // C
	*d0 = 0x10325476;   // D
	unsigned int M[16];
	//参考wiki
	for (int i = 0; i < lengthWords / 16; i++) {
		unsigned  A = *a0;
		unsigned  B = *b0;
		unsigned  C = *c0;
		unsigned  D = *d0;
		for (int j = 0; j < 16; j++)
			M[j] = words[16 * i + j];

		for (int j = 0; j < 64; j++) {
			unsigned int F, g;
			if (j >= 0 && j <= 15) {
				F = (B & C) | ((~B) & D);
				g = j;
			}
			else if (j >= 16 && j <= 31) {
				F = (D & B) | ((~D) & C);
				g = (5 * j + 1) % 16;
			}
			else if (j >= 32 && j <= 47) {
				F = B ^ C ^ D;
				g = (3 * j + 5) % 16;
			}
			else if (j >= 48 && j <= 63) {
				F = C ^ (B | (~D));
				g = (7 * j) % 16;
			}
			F = F + A + K[j] + M[g];
			
			A = D;
			D = C;
			C = B;
			B = B + leftRotate(F, S[j]);
		}
		*a0 = *a0 + A;
		*b0 = *b0 + B;
		*c0 = *c0 + C;
		*d0 = *d0 + D;
	}
}
//flag=0时输出为字符串，flag=1时输出字节流
unsigned char* MD5(unsigned char* msg,int flag) {
	unsigned * words;
	int wordsLen;
	msgPadding(msg, strlen(msg), &words, &wordsLen);
	
	unsigned  A, B, C, D;
	ProcessMsg(&A, &B, &C, &D, words, wordsLen);

	unsigned char* res = (unsigned char*)malloc(33);
	res[0] = '\0';

	unsigned char* a = (unsigned char*)&A;
	unsigned char* b = (unsigned char*)&B;
	unsigned char* c = (unsigned char*)&C;
	unsigned char* d = (unsigned char*)&D;

	unsigned char* tmp[] = { a,b,c,d };
	
	if (flag == 0) {
		for (int i = 0; i < 4; i++) {
			unsigned char s[9];
			sprintf_s(s, sizeof(s), "%02x%02x%02x%02x\0", tmp[i][0] & 0xff, tmp[i][1] & 0xff, tmp[i][2] & 0xff, tmp[i][3] & 0xff);
			//printf("i=%d,s=%s\n", i, s);
			strcat_s(res, strlen(res) + strlen(s) + 1, s);
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			unsigned char s[5];
			sprintf_s(s, sizeof(s), "%c%c%c%c\0", tmp[i][0] & 0xff, tmp[i][1] & 0xff, tmp[i][2] & 0xff, tmp[i][3] & 0xff);
			//printf("i=%d,s=%s\n", i, s);
			strcat_s(res, strlen(res) + strlen(s) + 1, s);
		}
	}
	

	free(words);
	return res;

}
//参考wiki
unsigned char* HMAC(unsigned char* k, unsigned char* msg, int blockSize) {
	unsigned char* key;

	// Keys longer than blockSize are shortened by hashing them
	if (strlen(k) > blockSize)key = MD5(k,1);
	else key = k;

	// Keys shorter than blockSize are padded to blockSize by padding with zeros on the right
	if (strlen(key) < blockSize) {
		unsigned char* tmp = (unsigned char*)malloc(blockSize);
		for (int i = 0; i < strlen(key); i++)
			tmp[i] = key[i];
		for (int i = strlen(key); i < blockSize; i++)
			tmp[i] = 0x00;
		tmp[blockSize] = '\0';
		key = tmp;
	}

	unsigned char o_pad = 0x5c;
	unsigned char i_pad = 0x36;
	unsigned char* o_key_pad = (unsigned char*)malloc(blockSize);// Outer padded key
	unsigned char* i_key_pad = (unsigned char*)malloc(blockSize);// Inner padded key
	
	//异或操作
	for (int i = 0; i < blockSize; i++) {
		o_key_pad[i] = key[i] ^ o_pad;
		i_key_pad[i] = key[i] ^ i_pad;
	}
	o_key_pad[blockSize] = '\0';
	i_key_pad[blockSize] = '\0';

	/*
		*计算hash(o_key_pad ∥ hash(i_key_pad ∥ message))
		* ||表示连接起来
		* hash的时候需要采用字节流的形式，所以先用字节流计算最后再转回字符串形式输出
	*/
	int len1 = strlen(i_key_pad) + strlen(msg) + 1;
	int len2 = strlen(o_key_pad) + len1 + 1;

	unsigned char* res1 = (unsigned char*)malloc(len1);
	unsigned char* res2 = (unsigned char*)malloc(len2);
	res1[0] = '\0';
	res2[0] = '\0';

	strcat_s(res1, len1, i_key_pad);
	strcat_s(res1, len1, msg);
	res1 = MD5(res1, 1);
	strcat_s(res2, len2, o_key_pad);
	strcat_s(res2, len2, res1);
	unsigned char* res = MD5(res2,0);
	
	free(res1);
	free(res2);

	return res;
}