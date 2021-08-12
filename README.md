# Meow Hash 0.6 Candidate
The code in this repository is in development and will change before release, do not use the code in this repository for anything but development of Meow Hash.

Contents:

meow_hash_x64_aesni.h - The function code.
test.c - A basic application running and benchmarking the hash functions.

The following functions, with the following intended capabilities, are defined:

Meow1 - 128 bit level 1 hash
Meow2 - 64 bit level 2 hash (outputs 128 bits)
Meow3 - 128 bit level 3 hash
Meow4 - 512 bit level 4 hash
MeowExpandSeed - A slightly modified version of Meow2 that can output arbitrary length byte streams. Not considered a full crypto RNG, but it should be plenty good for key expansion.

Meow1 uses an 8x128 bit inner state, and operates on blocks of 128 bytes.
Meow2, 3 and 4 all use a 11x128 bit inner state, and are all built on the same permutation that ingest 32 bytes while doing 3 AES operations. This permutation is designed to make simple inner collisions impossible. More information on this design to come.
Meow2 operates on blocks of 128 bytes.
Meow3 operates on blocks of 512 bytes, iterating over each block twice, with token differences between the rounds.
Meow4 operates on blocks of 1024 bytes, iterating over each block four times, with token differences between the rounds, and switching around the order of ingestion for each round.

Meow3 and 4 use the large blocks, combined with multiple iterations, to create a system where a lot of state transformation will happen between the first and the last ingestion of any given piece of input, making self-cancelling differences hard to achieve.

In ideal conditions I have measured around 3.4 bytes per cycle for Meow4 on a Zen2 processor, with Meow3 and 2 scaling nicely to double and quadruple that speed. The code is currently missing prefetch instructions, therefore this is only achieved when hashing data in cache. The processor is running approximately 3 SIMD instructions per clock, 1.28 of those being AES instructions. While Zen2 is in some cases capable of running 4 SIMD instructions per clock, I have not been able to get close to that number in any constellation where AES instructions are involved, thus the current result seems pretty much optimal in terms of instruction density.

Meow1 runs at pretty much the exact same speed as Meow2 on the Zen2, despite doing only 2/3 the AES operations. I'm not completely sure why, but I expect that some combination of the permutation pattern and the smaller state is to blame for causing some instruction congestion. I expect that Meow1 is faster than Meow2 on a lot of other processors, in particular those that can only do 1 AES instruction per cycle.