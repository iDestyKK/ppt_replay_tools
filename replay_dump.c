/*
 * PPT Tools: Replay Dumper
 *
 * Description:
 *     Dumps all replays in a "data.bin" file out. Each replay is separated
 *     into two parts in the save data, PREP and DATA. They are each in a fixed
 *     address and are of a fixed length, making extraction easy.
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

int main(int argc, char **argv) {
	//Argument Check
	if (argc != 2) {
		fprintf(stderr, "Usage: %s data.bin\n", argv[0]);
		return 1;
	}

	FILE        *fp;
	size_t       fsz;
	byte        *bin;
	unsigned int pos, i, replay_num;
	byte         out_name[18];

	//Load the file into memory
	fp = fopen(argv[1], "rb");

	//Get file size
	fseek(fp, 0, SEEK_END);
	fsz = ftell(fp);
	rewind(fp);

	//Allocate memory and read in all bytes
	bin = (byte *) malloc(sizeof(byte) * fsz);
	fread(bin, sizeof(byte), fsz, fp);

	//We are done here.
	fclose(fp);

	//Until 50, check for "PREP". This is how many replays there are.
	pos = PREP_LOC;
	for (replay_num = 0; replay_num < 50; replay_num++) {
		if (*(int*)& bin[pos] != 0x50455250)
			break;

		//Let the user know what data is there
		printf(
			"Found a save from: %04d/%02d/%02d @ %02d:%02d\n",

			2000 + *(ushort *)& bin[pos + 0x28],
			                    bin[pos + 0x2A],
			                    bin[pos + 0x2B],
			                    bin[pos + 0x2C],
			                    bin[pos + 0x2D]
		);

		pos += PREP_LEN;
	}

	//Now, go through the replays and dump them
	for (i = 0; i < replay_num; i++) {
		//Generate a filename based on time properties
		sprintf(
			(char *) out_name,
			"%04d%02d%02d_%02d%02d.dem",

			2000 + *(ushort *)& bin[PREP_LOC + (PREP_LEN * i) + 0x28],
			                    bin[PREP_LOC + (PREP_LEN * i) + 0x2A],
			                    bin[PREP_LOC + (PREP_LEN * i) + 0x2B],
			                    bin[PREP_LOC + (PREP_LEN * i) + 0x2C],
			                    bin[PREP_LOC + (PREP_LEN * i) + 0x2D]
		);

		/*
		 * Sweet. So we can make our own little format to dump the save.
		 * Let's be simple. First 0x168 bytes is the "PREP", and the remaining
		 * bytes is the data.
		 */

		fp = fopen((const char *) out_name, "wb");

		//Dump both sections
		fwrite(&bin[PREP_LOC + (PREP_LEN * i)], sizeof(byte), PREP_LEN, fp);
		fwrite(&bin[DATA_LOC + (DATA_LEN * i)], sizeof(byte), DATA_LEN, fp);

		fclose(fp);
	}

	printf("Dumped %d replays.\n", replay_num);

	//Clean up. We are done.
	free(bin);

	return 0;
}
