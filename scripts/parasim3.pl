#!/usr/bin/perl
#
# --- ROCK-COPYRIGHT-NOTE-BEGIN ---
# 
# This copyright note is auto-generated by ./scripts/Create-CopyPatch.
# Please add additional copyright information _after_ the line containing
# the ROCK-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
# the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
# 
# ROCK Linux: rock-src/scripts/parasim3.pl
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

my ($size_x, $max_x) = (68, 0);
my $size_y=int( 16 / ($#ARGV+1) );
my $file;

foreach $file (@ARGV) {
	open(F, $file) || die $!;
	while (<F>) {
		@_ = split /\s+/;
		$_[0]=int($_[0] * 100);
		$max_x=$_[0] if $_[0] > $max_x;
	}
	close F;
}

print "\n   -----+-----------------------------------" .
      "---------------------------------+\n";

foreach $file (reverse @ARGV) {
	my @data_val;
	my @data_nr;
	my $max_y = 0;

	my ($x, $newx, $y);

	open(F, $file) || die $!;
	while (<F>) {
		@_ = split /\s+/;
		$max_y=$_[1] if $_[1] > $max_y;
	}
	close F;

	open(F, $file) || die $!;
	for ($x=0; <F>; ) {
		@_ = split /\s+/;
		for ($newx=int($_[0] * 100); $x <= $newx; $x++) {
			$_ = int(($x*$size_x) / $max_x);

			$data_val[$_] = 0 unless defined $data_val[$_];
			$data_nr[$_]  = 0 unless defined $data_nr[$_];

			$data_val[$_] += $_[1];
			$data_nr[$_]++;
		}
	}
	close(F);

	for ($y=$size_y; $y>0; $y--) {
		if ($y == $size_y) { printf("    %3d |", $max_y); }
		elsif ($y == 1) { print "      1 |"; }
		else { print "        |"; }

		for ($x=0; $x<$size_x; $x++) {
			if (defined $data_val[$x]) {
				$_ = ($data_val[$x]*$size_y*2 /
				      $data_nr[$x]) / $max_y;
				if ($_ >= $y*2-1) { print ":"; }
				elsif ($_ >= $y*2-2) { print "."; }
				else { print " "; }
			} else {
				print " ";
			}
		}
		print "|\n";
	}

	print "   -----+-----------------------------------" .
	      "---------------------------------+\n";
}

printf "   Jobs | 00:00                       Time  " .
       "                           %02d:%02d |\n\n",
       $max_x / 100, ($max_x * 0.6 ) % 60;

