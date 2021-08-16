#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define AES 1
#define IN 2
#define MIX 3
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define COMPLEXITYLIMIT 3
#define LANES 11

#define SLOTSCOUNT (6*6*6*6*6 *6* 6*6*6*6*6)

typedef struct pattern {
	uint32_t origin;
	uint8_t cost;
	uint8_t changed;
	uint8_t input;
} pattern;

pattern* patterns;

//Algorithm structure
//{ AES, a, b } : Run a round of AESNI on internal state a, then xor internal state b into internal state a.
//{ IN, a, b } : Xor or add input digested input slice b into internal state a.
//Input slices must be numbered from 0 to 3, no input may be left out out of the algorithm if a higher numbered input is included.
//At the end of every round the internal state is shifted over so that state 1 becomes state 0 and so forth.

/*
// 8 complexity
uint8_t algorithm[][3] = {
	{ AES, 0, 3 }
	,{ IN, 3, 1 }
	,{ IN, 5, 2 }
	,{ AES, 5, 3 }
	,{ IN, 3, 0 }
	,{ IN, 7, 0 }
	,{ AES, 3, 4 }
	,{ IN, 4, 1 }
	,{ IN, 7, 3 }
	,{ AES, 7, 4 }
	,{ MIX, 2, 4 }
	,{ IN, 4, 2 }
	,{ AES, 2, 6 }
	,{ MIX, 6, 4 }
	,{ IN, 4, 3 }
};
*/

/*
uint8_t algorithm[][3] = {
	{ AES, 0, 5 }
	,{ IN, 2, 1 }
	,{ IN, 5, 3 }
	,{ AES, 5, 2 }
	,{ IN, 7, 2 }
	,{ IN, 2, 0 }
	,{ AES, 2, 4 }
	,{ IN, 4, 3 }
	,{ IN, 7, 1 }
	,{ AES, 7, 4 }
	,{ IN, 1, 0 }
	,{ IN, 4, 2 }
	,{ AES, 4, 1 }
	,{ IN, 6, 1 }
	,{ IN, 1, 2 }
};
*/

/*
uint8_t algorithm[][3] = {
	{ AES, 0, 5 }
	,{ IN, 6, 1 }
	,{ IN, 5, 0 }
	,{ AES, 5, 6 }
	,{ IN, 7, 3 }
	,{ IN, 6, 2 }
	,{ AES, 4, 7 }
	,{ IN, 6, 0 }
	,{ IN, 7, 1 }
	,{ AES, 3, 6 }
	,{ IN, 7, 2 }
	,{ IN, 6, 3 }
	,{ AES, 2, 7 }
};
*/

/*
//This one goes to 11
uint8_t algorithm[][3] = {
	{ AES, 0, 6 }
	,{ IN, 6, 0 }
	,{ IN, 3, 1 }
	,{ AES, 3, 6 }
	,{ IN, 1, 0 }
	,{ IN, 6, 1 }
	,{ AES, 6, 1 }
};
*/

/*
uint8_t algorithm[][3] = {
	{ AES, 0, 6 }
	,{ IN, 6, 0 }
	,{ IN, 3, 1 }
	,{ AES, 3, 6 }
	,{ IN, 1, 0 }
	,{ IN, 6, 1 }
	,{ AES, 6, 4 }
	,{ IN, 1, 1 }
	,{ IN, 4, 0 }
};
*/
/*
uint8_t algorithm[][3] = {
	{ AES, 0, 4 }
	,{ IN, 4, 0 }
	,{ IN, 6, 1 }
	,{ AES, 4, 1 }
	,{ IN, 1, 1 }
	,{ IN, 2, 0 }
};
*/
/*
uint8_t algorithm[][3] = {
	{ IN, 0, 0 }
	,{ IN, 1, 0 }
	,{ AES, 0, 3 }
};
*/
/*
uint8_t algorithm[][3] = {
	{ IN, 6, 0 }
	,{ IN, 3, 1 }
	,{ AES, 3, 6 }
	,{ IN, 6, 1 }
	,{ AES, 6, 1 }
	,{ IN, 1, 0 }
	,{ AES, 1, 7 }
};
*/
/*
uint8_t algorithm[][3] = {
	{ IN, 6, 1 }
	,{ IN, 4, 0 }
	,{ AES, 4, 1 }
	,{ IN, 2, 0 }
	,{ IN, 1, 1 }
	,{ AES, 1, 5 }
};
*/
/* 17
uint8_t algorithm[][3] = {
	{ IN, 2, 0 }
	,{ IN, 0, 1 }
	,{ AES, 0, 2 }
	,{ IN, 2, 1 }
	,{ AES, 2, 5 }
	,{ IN, 5, 0 }
	,{ AES, 5, 6 }
};
*/
//18 @ 11
uint8_t algorithm[][3] = {
	{ IN, 0, 0 }
	,{ AES, 0, 4 }
	,{ IN, 4, 1 }
	,{ AES, 4, 8 }
	,{ IN, 8, 1 }
	,{ AES, 8, 1 }
	,{ IN, 10, 0 }
};

/*
uint8_t algorithm[][3] = {
	{ IN, 1, 1 }
	,{ IN, 0, 0 }
	,{ AES, 0, 2 }
	,{ IN, 2, 0 }
	,{ IN, 5, 1 }
	,{ AES, 5, 2 }
};
*/

uint16_t cancelcost[4] = { 0,0,1,4 };

void printpattern(uint32_t patternid) {
	size_t a;
	for (a = 0; a < LANES; a++) {
		printf("%i", patternid % 6);
		patternid /= 6;
	}
	printf("\n");
}

void innerpropagate(uint32_t patternid, uint32_t origpatternid, uint8_t input, uint16_t cost, uint8_t cycle) {
	if (cycle == NELEMS(algorithm)) {
		if (cost < patterns[patternid].cost) {
			patterns[patternid].cost = cost;
			patterns[patternid].origin = origpatternid;
			patterns[patternid].changed = 1;
			patterns[patternid].input = input;
		}
	}
	else {
		if (algorithm[cycle][0] == AES) {
			uint8_t aescol = algorithm[cycle][1];
			uint16_t aescolval = (patternid / (uint32_t)(pow(6, aescol) + .5)) % 6;
			uint8_t xorcol = algorithm[cycle][2];
			uint16_t xorcolval = (patternid / (uint32_t)(pow(6, xorcol) + .5)) % 6;
			uint16_t aescost = 0;
			uint16_t repeats = 1;
			if (aescolval == 3) {
				return;
			}
			else {
				if (aescolval == 4) {
					aescost = 3;
					aescolval = 1;
					patternid -= ((uint32_t)(pow(6, aescol) + .5)) * 3;
					repeats = 2;
				}
				else if (aescolval == 5) {
					aescost = 3;
					aescolval = 3;
					patternid -= ((uint32_t)(pow(6, aescol) + .5)) * 2;
				}
				else if (aescolval > 0) {
					aescolval++;
					patternid += (uint32_t)(pow(6, aescol) + .5);
				}
				while (repeats > 0) {
					uint32_t newpatternid = patternid;
					if (aescolval == xorcolval) {
						innerpropagate(newpatternid, origpatternid, input, cost + aescost, cycle + 1);
						int16_t newcolval;
						for (newcolval = xorcolval - 1; newcolval >= 0; newcolval--) {
							newpatternid -= (uint32_t)(pow(6, aescol) + .5);
							innerpropagate(newpatternid, origpatternid, input, cost + cancelcost[aescolval] + aescost, cycle + 1);
						}
					}
					else if (aescolval > xorcolval) {
						innerpropagate(newpatternid, origpatternid, input, cost + aescost, cycle + 1);
					}
					else if (xorcolval == 4) {
						if (aescolval == 2) {
							newpatternid += ((uint32_t)(pow(6, aescol) + .5)) * 3;
						}
						else if (aescolval < 2) {
							newpatternid += (xorcolval - aescolval) * (uint32_t)(pow(6, aescol) + .5);
						}
						innerpropagate(newpatternid, origpatternid, input, cost + aescost, cycle + 1);
					}
					else if (xorcolval == 5) {
						if (aescolval == 2) {
							newpatternid += ((uint32_t)(pow(6, aescol) + .5)) * 2;
							innerpropagate(newpatternid, origpatternid, input, cost + aescost + 1, cycle + 1);
							newpatternid += ((uint32_t)(pow(6, aescol) + .5));
						}
						else if (aescolval < 2) {
							newpatternid += (xorcolval - aescolval) * (uint32_t)(pow(6, aescol) + .5);
						}
						innerpropagate(newpatternid, origpatternid, input, cost + aescost, cycle + 1);
					}
					else {
						newpatternid += (xorcolval - aescolval) * (uint32_t)(pow(6, aescol) + .5);
						innerpropagate(newpatternid, origpatternid, input, cost + aescost, cycle + 1);
					}
					repeats--;
					patternid += (uint32_t)(pow(6, aescol) + .5);
					aescolval++;
				}
			}
		}
		else if (algorithm[cycle][0] == IN) {
			uint8_t col = algorithm[cycle][1];
			uint16_t colval = (patternid / (uint32_t)(pow(6, col) + .5)) % 6;
			uint8_t inid = algorithm[cycle][2];
			uint16_t inval = (input / (uint32_t)(pow(6, inid) + .5)) % 6;
			if (inval < 4 && colval < 4) {
				if (colval == inval) {
					innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					int16_t newcolval;
					for (newcolval = inval - 1; newcolval >= 0; newcolval--) {
						patternid -= (uint32_t)(pow(6, col) + .5);
						innerpropagate(patternid, origpatternid, input, cost + cancelcost[inval], cycle + 1);
					}
				}
				else if (colval > inval) {
					innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
				}
				else {
					patternid += (inval - colval) * (uint32_t)(pow(6, col) + .5);
					innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
				}
			}
			else {
				patternid -= colval * (uint32_t)(pow(6, col) + .5);
				if (inval > colval) {
					inval ^= colval;
					colval ^= inval;
					inval ^= colval;
				}
				if (colval == 5) {
					if (inval == 5) {
						innerpropagate(patternid, origpatternid, input, cost + 2, cycle + 1);
						patternid += (uint32_t)(pow(6, col) + .5);
						innerpropagate(patternid, origpatternid, input, cost + 2, cycle + 1);
						patternid += (uint32_t)(pow(6, col) + .5);
						innerpropagate(patternid, origpatternid, input, cost + 1, cycle + 1);
						patternid += (uint32_t)(pow(6, col) + .5) * 2;
						innerpropagate(patternid, origpatternid, input, cost + 1, cycle + 1);
						patternid += (uint32_t)(pow(6, col) + .5);
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
					else if (inval == 4) {
						patternid += (uint32_t)(pow(6, col) + .5) * 2;
						innerpropagate(patternid, origpatternid, input, cost + 1, cycle + 1);
						patternid += (uint32_t)(pow(6, col) + .5) * 3;
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
					else if (inval == 3) {
						patternid += (uint32_t)(pow(6, col) + .5) * 3;
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
					else if (inval == 2) {
						patternid += (uint32_t)(pow(6, col) + .5) * 4;
						innerpropagate(patternid, origpatternid, input, cost + 1, cycle + 1);
						patternid += (uint32_t)(pow(6, col) + .5);
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
					else {
						patternid += (uint32_t)(pow(6, col) + .5) * 5;
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
				}
				else if (colval == 4) {
					if (inval == 4) {
						innerpropagate(patternid, origpatternid, input, cost + 1, cycle + 1);
						patternid += (uint32_t)(pow(6, col) + .5);
						innerpropagate(patternid, origpatternid, input, cost + 1, cycle + 1);
						patternid += (uint32_t)(pow(6, col) + .5) * 3;
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
					else if (inval == 3) {
						patternid += (uint32_t)(pow(6, col) + .5) * 3;
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
					else if (inval == 2) {
						patternid += (uint32_t)(pow(6, col) + .5) * 5;
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
					else {
						patternid += (uint32_t)(pow(6, col) + .5) * 4;
						innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
					}
				}
			}
		}
		else {
			printf("Illegal branch\n");
			/*
			uint8_t col = algorithm[cycle][1];
			uint16_t colval = (patternid >> (col * 2)) & 3;
			uint8_t inid = algorithm[cycle][2];
			uint16_t inval = (patternid >> (inid * 2)) & 3;
			if (colval == inval) {
				innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
				int16_t newcolval;
				for (newcolval = inval - 1; newcolval >= 0; newcolval--) {
					patternid -= 1 << (col * 2);
					innerpropagate(patternid, origpatternid, input, cost + cancelcost[inval], cycle + 1);
				}
			}
			else if (colval > inval) {
				innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
			}
			else {
				patternid += (inval - colval) << (col * 2);
				innerpropagate(patternid, origpatternid, input, cost, cycle + 1);
			}
			*/
		}
	}
}

void propagate(uint32_t patternid, uint16_t cost, uint32_t maxin) {
	size_t input;
	uint32_t opattern = patternid;
	//patternid = ((patternid >> 2) | (patternid << ((LANES * 2)-2))) & ((1<<(LANES * 2))-1);
	patternid = (patternid / 6) + (patternid % 6) * (SLOTSCOUNT / 6);
	for (input = (patternid == 0 ? 1 : 0); input < maxin;) {
		innerpropagate(patternid, opattern, input, cost, 0);
		input++;
		/*
		if (COMPLEXITYLIMIT == 2) {
			size_t input3;
			while (input3 = input & (input >> 1) & 0x5555) {
				input += input3;
			}
		}
		*/
	}
}

int mainsingle() {
	size_t a,b;
	for (a = 0; a < SLOTSCOUNT; a++) {
		patterns[a].origin = 0;
		patterns[a].cost = 255;
		patterns[a].changed = 0;
	}
	size_t maxin = 0;
	for (a = 0; a < NELEMS(algorithm); a++) {
		if (algorithm[a][0] == IN) {
			if (algorithm[a][2] > maxin) {
				maxin = algorithm[a][2];
			}
		}
	}
	maxin = pow(6, maxin + 1)+.5;
	propagate(0, 0, maxin);
	for (b = 0; b < 24; b++) {
		size_t updates = 0;
		for (a = 0; a < SLOTSCOUNT; a++) {
			if (patterns[a].changed) {
				propagate(a, patterns[a].cost, maxin);
				patterns[a].changed = 0;
				updates++;
			}
		}
		printf("%lu\n", updates);
	}
	size_t cpattern = 0;
	do {
		for (a = 0; a < LANES; a++) {
			printf("%lu", (cpattern / (uint32_t)(pow(6, a) + 0.5)) % 6);
		}
		printf(" ");
		for (a = 0; a < 2; a++) {
			printf("%i", (patterns[cpattern].input / (uint32_t)(pow(6, a) + 0.5)) % 6);
		}
		printf(" %i\n", patterns[cpattern].cost);
		cpattern = patterns[cpattern].origin;
	} while (cpattern != 0);
	return patterns[0].cost;
}
/*
uint8_t algorithm[][3] = {
	{ IN, 6, 1 }
	,{ IN, 0, 0 }
	,{ AES, 0, 1 }
	,{ IN, 2, 0 }
	,{ IN, 4, 1 }
	,{ AES, 4, 5 }
};
*/

uint8_t aesp[3][6] = {
	{1,2,4,5,8,9}
	,{1,2,5,6,8,9}
	,{1,2,5,6,9,10}
};

int main() {
	patterns = malloc(sizeof(pattern) * (uint64_t)SLOTSCOUNT);
	printf("%llu\n", (uint64_t)patterns);
	mainsingle();
	getchar();
	return 0;
}

int mainlong8() {
	int maxcost = 0;
	int p, q, r, s, t;
	FILE* fp;
	fp = fopen("results8-2.txt", "w");
	for (p = 0; p < 8; p++) {
		for (q = 0; q < 8; q++) {
			for (r = 0; r < 8; r++) {
				for (s = 0; s < 8; s++) {
					algorithm[0][1] = p;
					algorithm[3][1] = q;
					algorithm[2][2] = r;
					algorithm[5][2] = s;
					if (r == 0 || s == 5) {
						continue;
					}
					int cost = mainsingle();
					if (cost > maxcost) {
						maxcost = cost;
					}
					printf("Cost %i, %i %i %i %i\n", cost, p, q, r, s);
					if (cost + 4 >= maxcost) {
							
						fprintf(fp, "Cost %i, %i %i %i %i\n", cost, p, q, r, s);
					}
					printf("Maxcost %i\n", maxcost);
				}
			}
		}
	}
	fclose(fp);
	getchar();
	return 0;
}

int mainlong() {
	int maxcost = 0;
	int p, q, r, s, t;
	FILE* fp;
	fp = fopen("results.txt", "w");
	for (p = 0; p < 3; p++) {
		for (q = 0; q < 6; q++) {
			for (r = 0; r < 6; r++) {
				for (s = 0; s < 6; s++) {
					for (t = 0; t < 11; t++) {
						algorithm[0][1] = t;
						algorithm[1][2] = (p == 0 ? 0 : 1);
						algorithm[3][2] = (p == 1 ? 0 : 1);
						algorithm[5][2] = (p == 2 ? 0 : 1);
						algorithm[2][2] = aesp[0][q];
						algorithm[4][2] = aesp[1][r];
						algorithm[6][2] = aesp[2][s];
						int cost = mainsingle();
						if (cost > maxcost) {
							maxcost = cost;
						}
						if (cost + 4 >= maxcost) {
							printf("Cost %i, %i %i %i %i %i\n", cost, p, q, r, s, t);
							fprintf(fp, "Cost %i, %i %i %i %i %i\n", cost, p, q, r, s, t);
						}
						printf("Maxcost %i\n", maxcost);
					}
				}
			}
		}
	}
	fclose(fp);
	getchar();
	return 0;
}