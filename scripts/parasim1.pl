#!/usr/bin/perl
#
# --- ROCK-COPYRIGHT-NOTE-BEGIN ---
# 
# This copyright note is auto-generated by ./scripts/Create-CopyPatch.
# Please add additional copyright information _after_ the line containing
# the ROCK-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
# the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
# 
# ROCK Linux: rock-src/scripts/parasim1.pl
# ROCK Linux is Copyright (C) 1998 - 2003 Clifford Wolf
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version. A copy of the GNU General Public
# License can be found at Documentation/COPYING.
# 
# Many people helped and are helping developing ROCK Linux. Please
# have a look at http://www.rocklinux.org/ and the Documentation/TEAM
# file for details.
# 
# --- ROCK-COPYRIGHT-NOTE-END ---

my $size_x=70;
my $size_y=17;

my @data_val;
my @data_nr;
my $max_x = 0;
my $max_y = 0;

my ($x, $y);

open(F, $ARGV[0]) || die $!;
while (<F>) {
	@_ = split /\s+/;
	$max_x++;
	$max_y=$_[1] if $_[1] > $max_y;
}
close F;

open(F, $ARGV[0]) || die $!;
for ($x=0; <F>; $x++) {
	@_ = split /\s+/;
	$_ = int(($x*$size_x) / $max_x);

	$data_val[$_] = 0 unless defined $data_val[$_];
	$data_nr[$_]  = 0 unless defined $data_nr[$_];

	$data_val[$_] += $_[1];
	$data_nr[$_]++;
}
close(F);

$max_y=$ARGV[1] if $ARGV[1] > 0;

my @leftlabel=qw/. . P a r a l l e l . J o b s . ./;

print "\n  ----+----------------------------------------" .
      "------------------------------+\n";

for ($y=$size_y; $y>0; $y--) {
	if ($y == $size_y) { printf("  %3d |", $max_y); }
	elsif ($y == 1) { print "    1 |"; }
	else {
		print "    ", ($leftlabel[$size_y - $y] ne '.' ?
                              $leftlabel[$size_y - $y] : ' '), " |";
	}

	for ($x=0; $x<$size_x; $x++) {
		$_ = ($data_val[$x]*$size_y*2 / $data_nr[$x]) / $max_y;
		if ($_ >= $y*2-1) { print ":"; }
		elsif ($_ >= $y*2-2) { print "."; }
		else { print " "; }
	}
	print "|\n";
}

print "  ----+----------------------------------------" .
      "------------------------------+\n";

printf("      | 1                  Number of Jobs build so far  " .
       "               %5d |\n\n", $max_x);
