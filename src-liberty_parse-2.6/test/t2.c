#include "si2dr_liberty.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	si2drErrorT err;
    si2drGroupsIdT groups;
    si2drGroupIdT group;

	si2drPIInit(&err);
    si2drReadLibertyFile("t2.lib", &err);
    groups = si2drPIGetGroups(&err);
    
	while( !si2drObjectIsNull((group=
    	                       si2drIterNextGroup(groups,&err)),&err))
    {
        //some processing of the group
        if (!si2drObjectIsNull(group, &err))
            si2drObjectDelete(group, &err); /* <--- CRASHES on the first call itself */
    }
    exit(0);
}
