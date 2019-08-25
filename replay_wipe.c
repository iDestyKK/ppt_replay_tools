/*
 * PPT Tools: Replay Wiper
 *
 * Description:
 *     Wipes out memory from 0x2970 to 0x6FC0 and from 0x21F50 to 0x92D90 in
 *     the game's "data.bin". This effectively wipes out all 50 replays stored
 *     in the player's save data.
 *
 * Author:
 *     Clara Nguyen (@iDestyKK)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util/ppt_addr.h"

typedef unsigned char  byte;
typedef unsigned short ushort;

int main(int argc, char** argv) {
	//Argument Check
	if (argc != 2) {
		fprintf(stderr, "Usage: %s data.bin\n", argv[0]);
		return 1;
	}

	//Generate chunks
	byte *pchunk = (byte *) calloc(PREP_LEN * 50, sizeof(byte));
	byte *dchunk = (byte *) calloc(DATA_LEN * 50, sizeof(byte));

	FILE *fp;

	//Do some damage
	fp = fopen(argv[1], "r+");

	//Wipe out PREP
	fseek(fp, PREP_LOC, SEEK_SET);
	fwrite(pchunk, sizeof(byte), PREP_LEN * 50, fp);

	//Wipe out replay input data
	fseek(fp, DATA_LOC, SEEK_SET);
	fwrite(dchunk, sizeof(byte), DATA_LEN * 50, fp);

	//Clean up
	fclose(fp);

	free(pchunk);
	free(dchunk);

	return 0;
}
