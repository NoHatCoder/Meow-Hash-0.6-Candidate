//#define MEOW_DEF
#include "meow_hash_x64_aesni.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define BENCHSIZE (1024*1024*1024*6ull)
#define BENCHRUNS (5)

char outbuffer[10000];
int outbufferused=0;

void printhex(uint8_t* str, uint64_t len){
	uint64_t a;
	for(a=0;a<len;a++){
		outbufferused+=sprintf(outbuffer+outbufferused,"%.2X", str[a]);
	}
	outbufferused+=sprintf(outbuffer+outbufferused,"\n");
}

void printtime(double time){
	outbufferused += sprintf(outbuffer + outbufferused, "%f\n", time);
}

char* bencharray;

uint8_t testseed[176];
uint8_t testseed2[176];

void allhashes(uint64_t inputlen, void* input){
	double startTime;
	double endTime;
	meow_u128 hash;
	uint8_t* hashbuffer=(uint8_t*)&hash;
	uint8_t bighashbuffer[64];
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	hash=Meow1Hash(inputlen, input);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	hash=Meow2Hash(inputlen, input,(uint8_t*)0);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	hash=Meow2Hash(inputlen, input,testseed);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	hash=Meow2Hash(inputlen, input,testseed2);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	hash=Meow3Hash(inputlen, input,testseed);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	hash=Meow3Hash(inputlen, input,testseed2);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);

	startTime = (double) clock() / CLOCKS_PER_SEC;
	Meow4Hash(inputlen, input, bighashbuffer);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(bighashbuffer,64);
	printtime(endTime-startTime);
	printhex(bighashbuffer,0);
}

void allhashesrepeat(uint64_t inputlen, void* input, uint64_t repeats){
	double startTime;
	double endTime;
	meow1_state s1;
	meow2_state s2;
	meow3_state s3;
	meow4_state s4;
	meow_u128 hash;
	uint8_t* hashbuffer=(uint8_t*)&hash;
	uint8_t bighashbuffer[64];
	uint64_t a;
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	Meow1Begin(&s1);
	for(a=0;a<repeats;a++){
		Meow1Absorb(&s1,inputlen,input);
	}
	hash=Meow1End(&s1);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	Meow2Begin(&s2,(uint8_t*)0);
	for(a=0;a<repeats;a++){
		Meow2Absorb(&s2,inputlen,input);
	}
	hash=Meow2End(&s2);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	Meow2Begin(&s2,testseed);
	for(a=0;a<repeats;a++){
		Meow2Absorb(&s2,inputlen,input);
	}
	hash=Meow2End(&s2);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	Meow2Begin(&s2,testseed2);
	for(a=0;a<repeats;a++){
		Meow2Absorb(&s2,inputlen,input);
	}
	hash=Meow2End(&s2);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	Meow3Begin(&s3,testseed);
	for(a=0;a<repeats;a++){
		Meow3Absorb(&s3,inputlen,input);
	}
	hash=Meow3End(&s3);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);
	
	startTime = (double) clock() / CLOCKS_PER_SEC;
	Meow3Begin(&s3,testseed2);
	for(a=0;a<repeats;a++){
		Meow3Absorb(&s3,inputlen,input);
	}
	hash=Meow3End(&s3);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(hashbuffer,16);
	printtime(endTime-startTime);

	startTime = (double) clock() / CLOCKS_PER_SEC;
	Meow4Begin(&s4);
	for(a=0;a<repeats;a++){
		Meow4Absorb(&s4,inputlen,input);
	}
	Meow4End(&s4,bighashbuffer);
	endTime = (double) clock() / CLOCKS_PER_SEC;
	printhex(bighashbuffer,64);
	printtime(endTime-startTime);
	printhex(bighashbuffer,0);
}

int main(){
	char teststring[]="abcdefgh";

	MeowExpandSeed(8,teststring,176,testseed);
	MeowExpandSeed(9,teststring,176,testseed2);

	allhashes(8,teststring);
	
	bencharray=(char*)malloc(BENCHSIZE+3);
	uint64_t a,b,c;
	for(c=0;c<2;c++) {
		for (a = 0; a < BENCHSIZE; a += 8) {
			((uint64_t *) (bencharray + a))[0] = a * a;
		}
		allhashes(BENCHSIZE,bencharray);
		allhashesrepeat(1024*32,bencharray,BENCHSIZE/1024/8);
		bencharray+=3;
	}
	printf("%s",outbuffer);
}