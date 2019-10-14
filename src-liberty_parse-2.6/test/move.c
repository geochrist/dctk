#include "si2dr_liberty.h"
#include <stdlib.h>
main(int argc,char **argv)
{
	si2drErrorT err;
	si2drGroupIdT lib,g1,g2,g3;
	si2drAttrIdT attr1;
	
	si2drPIInit(&err);

	lib = si2drPICreateGroup("lib1", "lib", &err);

	attr1 = si2drGroupCreateAttr(lib, "default_operating_conditions", SI2DR_SIMPLE, &err);
	si2drSimpleAttrSetStringValue(attr1, "oc1", &err);
	
	g1 = si2drGroupCreateGroup( lib, "g1", "cell", &err);
	g2 = si2drGroupCreateGroup( lib, "g2", "cell", &err);
	g3 = si2drGroupCreateGroup( lib, "g3", "cell", &err);
	
	si2drWriteLibertyFile( "g1g2g3.lib", lib, "", &err);
  
	/* check end conditions */
	si2drGroupMoveAfter(g2,g1,&err);
	si2drWriteLibertyFile( "g1g2g3a.lib", lib, "", &err);
	si2drGroupMoveAfter(g2,g2,&err);
	si2drWriteLibertyFile( "g1g2g3b.lib", lib, "", &err);
	si2drGroupMoveBefore(g1,g2,&err);
	si2drWriteLibertyFile( "g1g2g3c.lib", lib, "", &err);
	si2drGroupMoveBefore(g2,g2,&err);
	si2drWriteLibertyFile( "g1g2g3d.lib", lib, "", &err);

 	si2drGroupMoveAfter(g3,g1,&err);
	
	si2drWriteLibertyFile("g1g3g2.lib", lib, "", &err);
	
	si2drGroupMoveBefore(g2,g1,&err);
	
	si2drWriteLibertyFile("g2g1g3.lib", lib, "", &err);

	si2drGroupMoveBefore(g3, g1, &err);
	si2drWriteLibertyFile( "g2g3g1.lib", lib, "", &err);
	
	si2drGroupMoveAfter( g2, g1, &err);
	si2drWriteLibertyFile( "g3g1g2.lib", lib, "", &err);
	
	si2drGroupMoveAfter( g1, g2, &err);
	si2drWriteLibertyFile( "g3g2g1.lib", lib, "", &err);
	si2drPIQuit(&err);
	
}
