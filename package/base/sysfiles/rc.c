/*
 * --- T2-COPYRIGHT-NOTE-BEGIN ---
 * This copyright note is auto-generated by ./scripts/Create-CopyPatch.
 * 
 * T2 SDE: package/.../sysfiles/rc.c
 * Copyright (C) 2004 - 2005 The T2 SDE Project
 * Copyright (C) 1998 - 2003 ROCK Linux Project
 * 
 * More information can be found in the files COPYING and README.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License. A copy of the
 * GNU General Public License can be found in the file COPYING.
 * --- T2-COPYRIGHT-NOTE-END ---
 */

#include <errno.h>
#include <grp.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

char env_PREVLEVEL[100]="PREVLEVEL=N";
char env_RUNLEVEL[100]="RUNLEVEL=N";
char env_TERM[100]="TERM=linux";

char * clean_env[] = {
	"PATH=/bin:/usr/bin",
	env_PREVLEVEL,
	env_RUNLEVEL,
	env_TERM,
	NULL
};

/* See bits/resource.h and asm/resource.h */
struct rlimit rlim_cpu      = { RLIM_INFINITY, RLIM_INFINITY };
struct rlimit rlim_fsize    = { RLIM_INFINITY, RLIM_INFINITY };
struct rlimit rlim_data     = { RLIM_INFINITY, RLIM_INFINITY };
struct rlimit rlim_stack    = { RLIM_INFINITY, RLIM_INFINITY };
struct rlimit rlim_core     = { RLIM_INFINITY, RLIM_INFINITY };
struct rlimit rlim_rss      = { RLIM_INFINITY, RLIM_INFINITY };
struct rlimit rlim_nofile   = {          1024,          1024 };
struct rlimit rlim_as       = { RLIM_INFINITY, RLIM_INFINITY };
struct rlimit rlim_nproc    = {          2048,          2048 };
struct rlimit rlim_memlock  = { RLIM_INFINITY, RLIM_INFINITY };
struct rlimit rlim_locks    = { RLIM_INFINITY, RLIM_INFINITY };

#define GROUP_LIST_SIZE 1
gid_t group_list[GROUP_LIST_SIZE] = { 0 };

/* This prototypes are missing in unistd.h */
int setresuid(uid_t ruid, uid_t euid, uid_t suid);
int setresgid(gid_t rgid, gid_t egid, gid_t sgid);

/* The first (and only) field in sched_param is sched_priority */
struct sched_param sp = { 0 };

#define handle_error(a) if ( a == -1 ) { perror("rc: " #a); return 1; }

int main(int argc, char ** argv) {
	char command[1024];
	int use_btee = 1;
	int btee_pipe[2];
	int i;

	/* Copy some environment variables to the new environment if set */
	if ( getenv("PREVLEVEL") )
		sprintf(env_PREVLEVEL, "PREVLEVEL=%.50s", getenv("PREVLEVEL"));
	if ( getenv("RUNLEVEL") )
		sprintf(env_RUNLEVEL, "RUNLEVEL=%.50s", getenv("RUNLEVEL"));
	if ( getenv("TERM") )
		sprintf(env_TERM, "TERM=%.50s", getenv("TERM"));

	/* we never need argv[0] - skipt it */
	argv++; argc--;

	/* Handle --nobtee option */
	if ( argc > 0 && !strcmp(*argv, "--nobtee") ) {
		use_btee = 0;
		argv++; argc--;
	}

	/* Display help message */
	if ( argc < 2 ) {
		fprintf(stderr,
"\n"
"  Run SystemV Init-Scripts with a clean environment and detached from\n"
"  the terminal.\n"
"\n"
"  Usage: rc [ --nobtee ] <service> { start | stop | ... | help } [ script options ]\n");
	}
	if ( argc == 0 ) {
		fprintf(stderr, "\n"
"  <service> might be one of:\n"
"\n");
		fflush(stderr);
		system("ls /etc/rc.d/init.d >&2");
	}
	if ( argc < 2 ) {
		fprintf(stderr, "\n");
		return 1;
	}

	/* No btee when viewing the help screen for this service */
	if ( !strcmp(argv[1], "help") ) use_btee = 0;

	/* Forward output to a 'btee' process */
	if ( use_btee ) {
		if ( pipe(btee_pipe) ) {
			perror("rc: Can't create pipe for btee");
			return 1;
		}
		if ( fork() == 0 ) {
			dup2(btee_pipe[0], 0);
			close(btee_pipe[0]);
			close(btee_pipe[1]);
			execl("/sbin/btee", "btee", "a",
			      "/var/log/init.msg", NULL);
			perror("rc: Can't exec btee command");
			return 1;
		} else {
			dup2(btee_pipe[1], 1);
			close(btee_pipe[0]);
			close(btee_pipe[1]);
		}
	}

	/* Set umask, process-group, nice-value and current directory */
	handle_error( umask(022) );
	handle_error( setpgid(0,0) );
	handle_error( setpriority(PRIO_PROCESS, 0, 0) );
	handle_error( chdir("/") );

	/* Set all ulimits */
	handle_error( setrlimit(RLIMIT_CPU,     &rlim_cpu) );
	handle_error( setrlimit(RLIMIT_FSIZE,   &rlim_fsize) );
	handle_error( setrlimit(RLIMIT_DATA,    &rlim_data) );
	handle_error( setrlimit(RLIMIT_STACK,   &rlim_stack) );
	handle_error( setrlimit(RLIMIT_CORE,    &rlim_core) );
	handle_error( setrlimit(RLIMIT_RSS,     &rlim_rss) );
	handle_error( setrlimit(RLIMIT_NOFILE,  &rlim_nofile) );
	handle_error( setrlimit(RLIMIT_AS,      &rlim_as) );
	handle_error( setrlimit(RLIMIT_NPROC,   &rlim_nproc) );
	handle_error( setrlimit(RLIMIT_MEMLOCK, &rlim_memlock) );
	handle_error( setrlimit(RLIMIT_LOCKS,   &rlim_locks) );

	/* Reset all signal handlers */
	for (i=1; i<NSIG; i++) {
		if ( i == SIGKILL ) continue;
		if ( i == SIGSTOP ) continue;
		if ( i >= __SIGRTMIN && i < SIGRTMIN ) continue;
		if( signal(i, SIG_DFL) == SIG_ERR ) {
			fprintf(stderr, "rc: Can't reset signal #%d: "
			        "%s\n", i, strerror(errno) );
			return 1;
		}
	}

	/* Close all file-descriptors > 2 (1024 seams to be a good value -
	 * linux/fs.h defines NR_OPEN to 1024*1024, which is too big ;-) */
	for (i=3; i<=1024; i++) close(i);

	/* Set user and group ids */
	handle_error( setgroups(GROUP_LIST_SIZE, group_list) );
	handle_error( setresuid(0, 0, 0) );
	handle_error( setresgid(0, 0, 0) );

	/* clear rt scheduling */
	handle_error( sched_setscheduler(0, SCHED_OTHER, &sp) );

	/* Run the command in a (non-interactive) login shell (i.e. read
	 * /etc/profile) and send \004 after running the script if we are
	 * using btee. */
	i = snprintf(command, sizeof(command), "%s%s",
	         strchr(*argv, '/') ? "" : "/etc/rc.d/init.d/", *argv);
	argv++;
	while (*argv) /* copy args */
		i += snprintf(command+i, sizeof(command)-i, " %s", *argv++);
	i+=snprintf(command+i, sizeof(command)-i, " </dev/null 2>&1%s",
	         use_btee ? "; echo -ne '\004'" : "");
	execle("/bin/bash", "-bash", "-l", "-c", command, NULL, clean_env);

	/* Oups! Can't exec the shell. */
	if ( use_btee ) write(1, "\004", 1);
	perror("rc: Can't execute shell for spawning init script");
	return 1;
}

