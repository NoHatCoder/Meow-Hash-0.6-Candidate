# Meow Hash patterns

An AES round will expand a difference in 1 byte to a difference in 4 fixed bytes, another round will expand those 4 bytes to all 16 bytes. Meanwhile 4 bytes of difference in a different pattern has a possibility of turning into a difference in just 1 byte. An attacker will typically try to exploit these patterns such that the mixing in the hash function will combine lanes with the same difference pattern, giving the differences a chance of cancelling each other.

When trying to produce collisions in Meow2 and Meow3 an attacker does not have insight into the values of the inner state, thus any attack is of probabilistic nature. While the probability of a single lane combination cancelling perfectly is pretty straight forward, multiple lane combinations relying partially on the same input differences are not truly independent, I estimate that in some attack patterns this may account for 1 or 2 cancellations being effectively free, while other patterns get harder or outright impossible to exploit.

By simplifying the inner state to 11 lanes that can each be in one of 6 different differential states I can exhaustively search for the most likely (according to the simplified model) path to producing a full inner collision.

The states are as follows:

* 0 - No difference.
* 1 - A difference in just 1 byte.
* 2 - A difference in 4 bytes, produced by AESing state 1.
* 3 - A difference in 16 bytes, produced by AESing state 2.
* 4 - A difference in 4 bytes that may AES into state 1 or state 2.
* 5 - Pattern 2 and 4 combined.

When propagating the state, a probability "cost" is calculated, approximately representing the inverse exponent of the probability of a state being reached.

State 1 cancellations are treated as free as they are typically just two pieces of identical input cancelling out.

State 2 cancellations rely on the original state 1 differences to be xored into either the same byte, or two bytes that are different such that they swap value, giving two 2^-8 chances. This is considered a cost of 1.

State 3 cancellations are effectively 4 state 2 cancellations in parallel, so the get a cost of 4.

A state 3 being AESed have effectively no chance of ever being cancelled meaningfully, one could set a cost of 16, but that would never end up on the most likely path, so this is simply considered a failure to produce an attack pattern.

A state 4 bing AESed must turn into either a state 1 or a state 2, for the state 2 to be further useful it must match the state 2s produced from state 1s, both of these events turn out to be very unlikely as the produced difference can be any of several hundred million possible patterns. Thus both events are given a possibly slightly conservative cost of 3.

Whenever two lanes where one difference is a superset of the other combine, the combined state is simply considered the bigger difference. Where equal difference lanes combine it is possible that they cancel fully, partially, or not at all, leaving the difference intact. State 5 can partially cancel with state 2, 4 or 5, at a cost of 1. State 5 can cancel fully with state 5, or down to state 1, at a cost of 2.

I note that many of the paths described are likely to make future cancellations less likely, or outright impossible. However, the addition operation will at times produce results that are different to xor just such that a useful partial cancellation can happen. Since the aim is not to find an attack, but rather to exclude the possibility of an easy attack, I choose to err on the side of including impossible attacks, rather than excluding possible ones.

Throughout various testing I have observed that the most likely attacks rarely rely on state 3, 4 and 5, they are simply too expensive. Thus a simpler model without state 4 and 5 proved useful for finding an algorithm.

The algorithm I ended up with has an exceptional likelihood cost of 18 in a state 0-1-2-3 scenario (not needing state 3), with a slightly lower cost of 16 when utilizing state 4.

## In the hash functions

In the design of the hash functions I have taken care to stick to the pattern that has been verified. This means that the addition operations are all 8 bit, in order to prevent an unlikely scenario where overflow from addition could morph one type of state into another by means of addition carries, and thus open a path that I have not accounted for. The symmetry-breaking variations all consist of reordering the input and occasionally adding a constant to a lane, as neither of these change the cancellation patterns.

Meow4 has to hold even against an attacker with full knowledge of the state, thus one could think that these probabilistic observations are of little relevance. However, as the same input is ingested multiple times, with full state avalanche in-between, a large part of an attack effectively becomes probabilistic.

If each point of likelihood cost is worth 7 bits of strength, then we expect Meow2 to have a strength of 112 bits. However, as the true value of the likelihood cost is somewhat uncertain, and a safety margin is desirable, a stated strength of 64 bits seem more appropriate. In practice 64 bits should be plenty to foil hash flood attacks, so there is little reason to claim more.

For Meow3 the most realistic route to an attack is probably to make an input stream that cancels out twice. Given the difference of ingestion order, this requires finding two different cancelling patterns that match, and be lucky that they both work at the same time. So this is easily at least double the strength of Meow2.

In the case of Meow4 the first of four rounds of ingestion can probably be made to cancel analytically. Thereafter I can't find anything that looks remotely viable. One might initially think that it is simply a matter finding 4 matching cancelling patterns and getting lucky with the last 3 of them, but the way input ingestion differs between rounds means that a normal relatively compact collision pattern would be spread out much wider in the following rounds. Realistically the cancelling patterns would all end up being spread across the entire input block, meaning that the unlikeliness of keeping a related inner state and cancelling out as necessary all the way through is more like Meow2 ten times over. Why denote it a level 4 function instead of a level 5 function then? Mostly because any strength beyond 256 bits is practically meaningless, calling it level 4 gives it gives a slightly lower chance of ending in a state of technically broken.

## Test program

Running the test program takes 3+ hours, it probably could be way faster as it is written with little concern for speed. The output is the following:

	2595207413824
	3955969
	48991997
	148877348
	173321877
	119003071
	51270411
	12525174
	1546674
	155724
	33788
	2595
	52
	0
	0
	0
	0
	0
	0
	0
	0
	0
	0
	0
	0
	00000000000 02 16
	01000200020 04 12
	00100020012 00 6
	20010002001 10 6
	00001000200 04 6

That is first a pointer value to verify that the table allocation succeeded. Then a series of numbers each detailing how many nodes were updated in each round of the graph search. And finally a detail of the most likely path in inverse order. Each row indicate the state of the 11 lanes after each round of input and mutation, then the input lanes for that round, and finally the accumulated cost. Please bear in mind that the detailed path is mostly just a debug output, there are many other paths, and it is quite likely that many of them are in practice more viable than the displayed one. The actual result is that none of those paths have a probability cost of less than 16.