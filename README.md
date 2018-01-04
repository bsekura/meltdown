# meltdown

Simple test of meltdown vulnerability (covert channel via rogue data cache load).
Instructions pipelined and discarded after exception has been raised
force a cached load based on result loaded from specified address.
Reads memory via cache flush + reload