#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "si2dr_liberty.h"



main(int argc,char **argv)
{
	/* a simple test case */
	si2drErrorT e;
	si2drGroupIdT g1,g2,g3;
	si2drStringT  s1,s2,s3;
	si2drNamesIdT ns;
	si2drAttrIdT a1,a2,a3;
	si2drAttrIdT b1,b2,b3;
	si2drDefineIdT d1,d2,d3;
	si2drDefineIdT e1,e2,e3;
	
	char buf1[100];

	si2drStringT defname, defallowed;
	si2drValueTypeT deftype;
	
	si2drPIInit (&e);
	
	g1 = si2drPICreateGroup("lib1", "library", &e);
	

	g2 = si2drGroupCreateGroup(g1, "cell1", "cell", &e);

	strcpy(buf1,"attr1");
	a1 = si2drGroupCreateAttr(g2, buf1, SI2DR_SIMPLE, &e);
	si2drSimpleAttrSetInt32Value(a1,12,&e);
	strcpy(buf1,"attr2");
	a2 = si2drGroupCreateAttr(g2, buf1, SI2DR_SIMPLE, &e);
	si2drSimpleAttrSetBooleanValue(a2,SI2_TRUE, &e);
	strcpy(buf1,"attr3");
	a3 = si2drGroupCreateAttr(g2, buf1, SI2DR_SIMPLE, &e);
	si2drSimpleAttrSetFloat64Value(a3, 3.141592653, &e);
	strcpy(buf1,"attr3");
	a3 = si2drGroupCreateAttr(g2, buf1, SI2DR_SIMPLE, &e);
	si2drSimpleAttrSetFloat64Value(a3, 3.141592653, &e);

	strcpy(buf1,"def1");
	d1 = si2drGroupCreateDefine(g2, buf1, "cell1", SI2DR_INT32, &e);
	strcpy(buf1,"def2");
	d2 = si2drGroupCreateDefine(g2, buf1, "cell1", SI2DR_INT32, &e);
	strcpy(buf1,"def3");
	d3 = si2drGroupCreateDefine(g2, buf1, "cell1", SI2DR_INT32, &e);

	strcpy(buf1,"bogus1");
	

	b1 = si2drGroupFindAttrByName(g2,"attr1",&e);
	if( e != SI2DR_NO_ERROR )
	{
		printf("Couldn't find attr1!!!!!\n");
	}
	else
	{
		printf("Found attr1, val=%d\n", si2drSimpleAttrGetInt32Value(b1,&e));
	}
	
	b2 = si2drGroupFindAttrByName(g2,"attr2",&e);
	if( e != SI2DR_NO_ERROR )
	{
		printf("Couldn't find attr2!!!!!\n");
	}
	else
	{
		printf("Found attr2, val=%d\n", si2drSimpleAttrGetBooleanValue(b2,&e));
	}

	b3 = si2drGroupFindAttrByName(g2,"attr3",&e);
	if( e != SI2DR_NO_ERROR )
	{
		printf("Couldn't find attr3!!!!!\n");
	}
	else
	{
		printf("Found attr3, val=%g\n", si2drSimpleAttrGetFloat64Value(b3,&e));
	}

	
	e1 = si2drGroupFindDefineByName(g2,"def1",&e);
	if( e != SI2DR_NO_ERROR )
	{
		printf("Couldn't find def1!!!!!\n");
	}
	else
	{
		si2drDefineGetInfo(e1, &defname, &defallowed, &deftype, &e);
		printf("Found def1:  name=%s, allowed=%s, type=%d\n", defname, defallowed, deftype);
	}

	e2 = si2drGroupFindDefineByName(g2,"def2",&e);
	if( e != SI2DR_NO_ERROR )
	{
		printf("Couldn't find def2!!!!!\n");
	}
	else
	{
		si2drDefineGetInfo(e2, &defname, &defallowed, &deftype, &e);
		printf("Found def2:  name=%s, allowed=%s, type=%d\n", defname, defallowed, deftype);
	}

	e3 = si2drGroupFindDefineByName(g2,"def3",&e);
	if( e != SI2DR_NO_ERROR )
	{
		printf("Couldn't find def3!!!!!\n");
	}
	else
	{
		si2drDefineGetInfo(e3, &defname, &defallowed, &deftype, &e);
		printf("Found def3:  name=%s, allowed=%s, type=%d\n", defname, defallowed, deftype);
	}

	// this should not be necessary! Everything should come out OK.  si2drWriteLibertyFile( "t4.lib", g1, "", &e);
	
	si2drPIQuit(&e);
	
	return 0;
}
