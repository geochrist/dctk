#include <stdio.h>
#include <stdlib.h>
#include <si2dr_liberty.h>

void printAttrs (si2drGroupIdT _gp) {
  si2drErrorT err;
  si2drGroupsIdT gps = si2drGroupGetGroups (_gp, &err);
  si2drAttrsIdT  ats = si2drGroupGetAttrs (_gp, &err);
  si2drGroupIdT  gp;
  si2drAttrIdT   at;

  while (!si2drObjectIsNull ((at = si2drIterNextAttr (ats, &err)), &err)) {
    if (si2drAttrGetAttrType (at, &err) != SI2DR_SIMPLE) break;
    printf ("%s : ", si2drAttrGetName (at, &err));
    switch (si2drSimpleAttrGetValueType (at, &err)) {
      case SI2DR_STRING:  printf ("STRING= %s\n",si2drSimpleAttrGetStringValue(at,&err)); break;
      case SI2DR_FLOAT64: printf ("FLOAT= %g\n",si2drSimpleAttrGetFloat64Value(at,&err)); break;
      case SI2DR_INT32:   printf ("INT= %d\n",si2drSimpleAttrGetInt32Value(at,&err)); break;
      case SI2DR_BOOLEAN: printf ("BOOLEAN= %s\n",(si2drSimpleAttrGetBooleanValue(at,&err)?"True":"False")); break;
      case SI2DR_EXPR:    printf ("EXPR= %s\n",si2drExprToString(si2drSimpleAttrGetExprValue(at,&err),&err)); break;
    }
  } si2drIterQuit (ats, &err);

  while (!si2drObjectIsNull ((gp = si2drIterNextGroup (gps, &err)), &err)) {
    printAttrs (gp);
  } si2drIterQuit (gps, &err);
}

main (int argc, char **argv) {
  si2drErrorT err;
  si2drGroupsIdT gps;
  si2drGroupIdT gp;

  if( argc < 2 )
  {
	  printf("Make sure to include an argument == the file name to read in!\n");
	  exit(14);
  }
  
  si2drPIInit (&err);
  si2drReadLibertyFile (argv[1], &err);

	if( err == SI2DR_SYNTAX_ERROR )
		exit(101);

  gps = si2drPIGetGroups (&err);
  gp = si2drIterNextGroup (gps, &err);
  si2drIterQuit (gps, &err);
  si2drWriteLibertyFile ("out.lib", gp, "", &err);

  printAttrs (gp);
}
