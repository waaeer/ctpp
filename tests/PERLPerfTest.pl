#!/usr/bin/perl -w
use strict;
use Time::HiRes qw(gettimeofday tv_interval);

my $t0 = [gettimeofday];

my $NI = 0;
my @AArray;
for ($NI = 0; $NI < 10000; ++$NI)
{
	if ($NI % 2 == 0)  { $AArray[$NI] = $NI; }
	else
	{
		my $NJ = 0;
		for ($NJ = 0; $NJ < 100; ++$NJ)
		{
			my $SKey = sprintf("_%d_%d_", $NI, $NJ);
			$AArray[$NI] -> {$SKey} -> [10] = $NI;
		}
	}

}

my $elapsed = tv_interval($t0);
print("Time: " . $elapsed . "\n")
