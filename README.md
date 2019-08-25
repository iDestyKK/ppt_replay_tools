# Puyo Puyo Tetris Replay Tools
*A set of command line tools for handling replays.*

## Synopsis
Puyo Puyo Tetris got a PC port in 2018. However, the replay limit was still
restricted to 50 replays. Fortunately, these are stored locally in a file
called `data.bin`, where they can be extracted and imported by simply copying
bytes. These tools serve to artificially remove the 50 replay limit by
storing replays as individual files (as it should be), as well as import them
(or custom replays) back into the save.

## Tool Information
There are three here:

* **replay\_dump** - When given a `data.bin` file, it will dump all replays
as `YYYYMMDD_HHMM.dem` files (e.g. a replay from 2019/08/24 @ 22:24 would be
named as `20190824_2224.dem`).

* **replay\_wipe** - When given a `data.bin` file, it will zero out all bytes
that store replay information, swiftly deleting all replays. You should use
this after dumping all replays.

* **replay\_inject** - When given a `data.bin` file and a `.dem` file, this
will let you inject a replay into your save file to be played back in-game.
You may specify a specific replay to replace, or just add it as a new replay.
**However, if the save already has 50 replays, it will not insert a 51th
replay.**

I'll make a GUI eventually which can serve to let users drag and drop replays
in one utility.

## Format Details
I haven't looked too much into the format of `data.bin`, but figuring out how
replays were stored was as simple as making a clean save, making a replay, and
then comparing changes to the file.

Replays are stored in two parts: `PREP` (replay information) and `DATA` (key
presses, events, etc). Each of these are of a fixed length. Each `PREP` section
takes up exactly 360 bytes, and each `DATA` takes up 92480 bytes. There is a
limit to how large a replay can be. Since they are all a fixed length, my tools
simply jump exactly to where the data is stored and dump the contents to a
file. Simple huh?

Addresses and lengths can be found in `util/ppt_addr.h`. DEM files simply
concatenate the `PREP` and `DATA` sections into a single file. The reverse
simply splits the file and injects the bytes at the appropriate addresses.

## Usage
**Replay Dump**
```bash
UNIX> ./replay_dump data.bin
```

**Replay Wipe**
```bash
UNIX> ./replay_wipe data.bin
```

**Replay Inject**
```bash
UNIX> ./replay_inject data.bin replay.dem [position]
```

## Compilation

### Windows
**Method 1:** Get Visual Studio, make a new project, and drag these files in. Add `_CRT_SECURE_NO_WARNINGS` to the **Preprocessor Definitions**, and compile.
**Method 2:** I'm a nice person, and I have compiled them for you. They are in the `bin/` directory.

### Linux, Mac, etc.
Run `make`.
