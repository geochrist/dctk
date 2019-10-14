#include <stdio.h>

#include "si2dr_liberty.h"



main(int argc,char **argv)
{
	/* a simple test case */
	si2drErrorT e;
	si2drGroupIdT g1,g2,g3;
	si2drStringT  s1,s2,s3;
	si2drNamesIdT ns;
	
	si2drPIInit (&e);
	
	g1 = si2drPICreateGroup("lib1", "library", &e);
	

	g2 = si2drGroupCreateGroup(g1, "cell1", "cell", &e);
	

	si2drGroupAddName(g1,"lib2",&e);
	si2drGroupAddName(g1,"lib3",&e);
	si2drGroupAddName(g1,"lib4",&e);
	si2drGroupAddName(g1,"lib5",&e);
	
	printf("Before\n");
	ns = si2drGroupGetNames(g1,&e);
	while( (s1 = si2drIterNextName(ns,&e)) )
	{
		printf("Lib Name: %s\n",s1);
	}
	si2drIterQuit(ns,&e);
	ns = si2drGroupGetNames(g2,&e);
	while( (s1 = si2drIterNextName(ns,&e)) )
	{
		printf("Cell Name: %s\n",s1);
	}
	si2drIterQuit(ns,&e);
	printf("======================================\n\n");
	

	si2drGroupDeleteName(g1,"lib5",&e);
	si2drGroupAddName(g1,"lib5a",&e);
	
	si2drGroupDeleteName(g2,"cell1",&e);
	si2drGroupAddName(g2,"cell1a",&e);
	
	printf("After\n");
	ns = si2drGroupGetNames(g1,&e);
	while( (s1 = si2drIterNextName(ns,&e)) )
	{
		printf("Lib Name: %s\n",s1);
	}
	si2drIterQuit(ns,&e);
	ns = si2drGroupGetNames(g2,&e);
	while( (s1 = si2drIterNextName(ns,&e)) )
	{
		printf("Cell Name: %s\n",s1);
	}
	si2drIterQuit(ns,&e);
	printf("======================================\n\n");

	si2drPIQuit(&e);
	
	return 0;
}
