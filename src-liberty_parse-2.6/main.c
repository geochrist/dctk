/******************************************************************************
    Copyright (c) 1996-2005 Synopsys, Inc.    ALL RIGHTS RESERVED

  The contents of this file are subject to the restrictions and limitations
  set forth in the SYNOPSYS Open Source License Version 1.0  (the "License"); 
  you may not use this file except in compliance with such restrictions 
  and limitations. You may obtain instructions on how to receive a copy of 
  the License at

  http://www.synopsys.com/partners/tapin/tapinprogram.html. 

  Software distributed by Original Contributor under the License is 
  distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either 
  expressed or implied. See the License for the specific language governing 
  rights and limitations under the License.

******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "si2dr_liberty.h"
#include <sys/types.h>
#include <time.h>

static void print_version(void)
{
	printf("liberty_parse version %s\n\n", VERSION);
}


static void usage(void)
{
	print_version();
	
	printf("Usage: liberty_parse [options] filename\n\n");
	printf("Where [options are:\n\n");
	printf("--version  print version info and quit\n");
	printf("--write    for each lib in the file, write out a file with 'blatantcopy' suffix\n");
	printf("--help     print usage and quit\n");
	printf("\n");
}


/* test program for liberty parser */

int main(int argc, char **argv)
{
	si2drErrorT err;
	FILE *ifile;
	char buf1[1000];
	si2drGroupsIdT groups;
	si2drGroupIdT group;
	si2drStringT nam;
	int count, write_out, print_stats;
	char *dum;
	time_t startt,endt;
	/* extern liberty_parser_debug; */
	int version = 0;
	int help = 0;
	int strtab_tots = 0;
	char filename[1000];
	int i;
	extern char *curr_file;
	
	filename[0] = 0;
	count = 1;
	write_out = 0;
	print_stats = 1;
	/* liberty_parser_debug =1; */
	
	/* handle the usual arguments */

	/* if the list of possible arguments gets any longer, use the options package... */

	for(i=1;i< argc;i++)
	{
		if( !strcmp(argv[i],"--version") )
			version=1;
		else if( !strcmp(argv[i], "--help") )
			help=1;
		else if( !strcmp(argv[i], "--write") )
			write_out=1;
		else if( !strcmp(argv[i], "--strtab-totals") )
			strtab_tots=1;
		else if( i == argc-1  && argv[i][0] != '-' )
			strcpy(filename,argv[i]);
		else
			printf("Unrecognized argument '%s' ignored. Use --help for help.\n", argv[i]);
	}
	
	if( help )
	{
		usage();
		exit(0);
	}
	
	if( version )
	{
		print_version();
		exit(0);
	}

	if( !filename[0] )
	{
		printf("No filename specified!\n\n");
		usage();
		exit(0);
	}
	
#ifdef TRACE
	si2drPISetDebugMode(&err);
	si2drPISetTraceMode("x",&err); /* it's ok, maybe even more desirable to set trace mode before calling PIInit,
									  because then you'll get the PIInit in the generated program. */
#endif
	si2drPIInit(&err);
	
	print_version();
	
	printf("\n\nreading %s...\n", filename);
	time(&startt);
	curr_file = filename;
	
	si2drReadLibertyFile(filename,&err);
	if( err == SI2DR_INVALID_NAME )
	{
		printf("COULD NOT OPEN %s for parsing-- quitting...\n",filename);
		exit(301);
	}
	else if (err == SI2DR_SYNTAX_ERROR )
	{
		printf("\nSyntax Errors were detected in the input file!\n\n");
		exit(401);
	}
	
	
	time(&endt);

	printf("... Done. \nElapsed time= %d seconds\n", endt-startt);

	groups = si2drPIGetGroups(&err);
	
	while( !si2drObjectIsNull((group=si2drIterNextGroup(groups,&err)),&err) )
	{
		
		printf("\n\nChecking the database...\n\n");

		time(&startt);
		si2drCheckLibertyLibrary(group, &err);
		time(&endt);

		if( err == SI2DR_NO_ERROR )
			printf("Passed\n\n");
		else
			printf("Errors detected\n\n");
		
		printf("... Done. \nElapsed time= %d seconds\n", endt-startt);

	}
	si2drIterQuit(groups,&err);
	
	if( write_out )
	{
		
		groups = si2drPIGetGroups(&err);
		
		while( !si2drObjectIsNull((group=si2drIterNextGroup(groups,&err)),&err) )
		{
			si2drNamesIdT gnames;
			si2drStringT gname;
			
			gnames = si2drGroupGetNames(group, &err);
			gname = si2drIterNextName(gnames,&err);
			si2drIterQuit(gnames,&err);
			
			sprintf(buf1,"%s_blatantcopy%d.lib", gname, count++);
			printf("writing %s...\n", buf1);
			
			
			
			time(&startt);
			si2drWriteLibertyFile(buf1, group, NULL, &err);
			time(&endt);
			
			printf("... Done. Elapsed time= %d seconds\n", endt-startt);
			
		}
		si2drIterQuit(groups,&err);
	}
	
	if( print_stats )
	{
		int cellcount=0;
		
		printf("\n\nStats:\n");
		groups = si2drPIGetGroups(&err);
		
		while( !si2drObjectIsNull((group=si2drIterNextGroup(groups,&err)),&err) )
		{
			si2drNamesIdT gnames;
			si2drStringT gname;
			si2drGroupsIdT gs2;
			si2drGroupIdT g2;

			gnames = si2drGroupGetNames(group, &err);
			gname = si2drIterNextName(gnames,&err);
			si2drIterQuit(gnames,&err);

			if( !gname )
				gname = "<NONAME>";
			
			printf("\n\nLibrary %s:\n", gname);
			gs2 = si2drGroupGetGroups(group,&err);
			
			while( !si2drObjectIsNull((g2=si2drIterNextGroup(gs2,&err)),&err) )
			{
				si2drStringT gt = si2drGroupGetGroupType(g2,&err);

				if( !strcmp(gt,"cell") )
					cellcount++;
			}
			si2drIterQuit(gs2,&err);
			printf("   cells: %d\n\n", cellcount);
		}
		si2drIterQuit(groups,&err);
	}
	if( strtab_tots )
		print_strtab_stats();
	
	printf("Quitting..\n");
	
	si2drPIQuit(&err);

#ifdef TRACE
	si2drPIUnSetTraceMode(&err);
#endif
 	
	printf("Done....\n");
	exit(0);
}
