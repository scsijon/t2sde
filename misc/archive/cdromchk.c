/* gcc -O2 -Wall cdromchk.c -o cdromchk
 *
 * --- ROCK-COPYRIGHT-NOTE-BEGIN ---
 * 
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * Please add additional copyright information _after_ the line containing
 * the ROCK-COPYRIGHT-NOTE-END tag. Otherwise it might get removed by
 * the ./scripts/Create-CopyPatch script. Do not edit this copyright text!
 * 
 * ROCK Linux: rock-src/misc/archive/cdromchk.c
 * ROCK Linux is Copyright (C) 1998 - 2003 Clifford Wolf
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version. A copy of the GNU General Public
 * License can be found at Documentation/COPYING.
 * 
 * Many people helped and are helping developing ROCK Linux. Please
 * have a look at http://www.rocklinux.org/ and the Documentation/TEAM
 * file for details.
 * 
 * --- ROCK-COPYRIGHT-NOTE-END ---
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char ** argv) {
	int cdrom,file;
	char buf1[4096];
	char buf2[4096];
	int rc1,rc2,rc3;
	int c1=0,c2=0;
	
	if (argc != 3) {
		fprintf(stderr,"Usage: %s <cdrom> <iso-file>\n",argv[0]);
		return 1;
	}
	
	fprintf(stderr,"Checking CD-ROM ...");
	
	if ( (cdrom=open(argv[1],O_RDONLY|O_SYNC)) == 0 ) {
		fprintf(stderr,"\n%s: Can't open %s: %s\n",
		        argv[0],argv[1],strerror(errno));
		return 1;
	}
	
	if ( (file=open(argv[2],O_RDONLY)) == 0 ) {
		fprintf(stderr,"\n%s: Can't open %s: %s\n",
		        argv[0],argv[2],strerror(errno));
		return 1;
	}
	
	while ( (rc1=read(file,buf1,4096)) > 0 ) {
		for (rc2=0; rc2 < rc1; rc2+=rc3) {
			rc3=read(cdrom,buf2+rc2,rc1-rc2);
			if (rc3 == -1) {
				fprintf(stderr,"\n%s: cdrom read error: %s\n",
				        argv[0],strerror(errno));
				return 1;
			}
			if (rc3 == 0) break;
		}
		if (rc2 != rc1) {
			fprintf(stderr,"\n%s: cdrom read error: %d of "
			        "%d bytes\n",argv[0],rc2,rc1);
			return 1;
		}
		if (memcmp(buf1,buf2,rc2)) {\
			fprintf(stderr,"\n%s: cdrom data differs from "
			        "the iso file\n",argv[0]);
			return 1;
		}
		c1+=rc2; c2+=rc2;
		if (c1 > (1048576*16)) {
			c1=0;
			fprintf(stderr,".");
		}
	}

	if (rc1 == -1) {
		fprintf(stderr,"\n%s: file read error: %s\n",
		        argv[0],strerror(errno));
		return 1;
	}
	
	fprintf(stderr," OK (%d MB).\n",c2/1048576);
	return 0;
}
