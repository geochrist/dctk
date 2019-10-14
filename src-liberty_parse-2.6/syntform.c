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
#include <stdlib.h>
#include "syntax.h"
#include <string.h>
#ifdef DMALLOC
#include <dmalloc.h>
#endif
#include "mymalloc.h"

extern liberty_hash_table *libsynt_groups;
extern liberty_hash_table *libsynt_allgroups;
extern liberty_hash_table *libsynt_attrs;
#include <sys/types.h>

void dump_group(libsynt_group_info *g, FILE *outC, FILE *outH);
void dump_attr(libsynt_attribute_info *a, FILE *outC, FILE *outH);

#if HAVE_DIRENT_H
# include <dirent.h>
# define NAMLEN(dirent) strlen((dirent)->d_name)
#else
# define dirent direct
# define NAMLEN(dirent) (dirent)->d_namelen
# if HAVE_SYS_NDIR_H
#  include <sys/ndir.h>
# endif
# if HAVE_SYS_DIR_H
#  include <sys/dir.h>
# endif
# if HAVE_NDIR_H
#  include <ndir.h>
# endif
#endif

extern  libsynt_technology libsynt_techs[20];
extern  int libsynt_tech_count;
extern FILE *syntax_parser_in;
int syntax_parser_parse(void);

int syntax_parser_parse(void);

int main(int argc, char **argv)
{
	DIR *dirp;
	struct dirent *dp;
	char *p;
	FILE *fp, *outC, *outH;
	int i;
	liberty_hash_bucket *hb;
	

	libsynt_allgroups = liberty_hash_create_hash_table(503, 1, 0);
	libsynt_attrs = liberty_hash_create_hash_table(503, 1, 0);
	
	dirp = opendir("./desc");
	while( (dp = readdir(dirp)) != NULL )
	{
		if( strncmp(dp->d_name,"syntax.",7) == 0 && strlen(dp->d_name) > 8)
		{
			char fname[1000];

			/* foreach file in the desc dir */
			
			/* form the master hash tables */
			libsynt_techs[libsynt_tech_count].group_hash = liberty_hash_create_hash_table(503, 1, 0);
			libsynt_groups = libsynt_techs[libsynt_tech_count].group_hash;
			
			libsynt_techs[libsynt_tech_count].name = (char*)my_malloc(NAMLEN(dp)+1);
			strcpy(libsynt_techs[libsynt_tech_count].name, dp->d_name+7);
			p = strchr(libsynt_techs[libsynt_tech_count].name,'.');
			if( *p )
				*p = 0;
			/* parse the file right here  -- the tech count gets bumped when
			   the file is parsed */
			sprintf(fname,"desc/%s",dp->d_name);

			printf("Processing %s\n", fname);
			
			fp = fopen(fname,"r");
			if( !fp )
			{
				printf("Had trouble opening %s. Fix the problem and re-run\n", fname);
				exit(23);
			}

			/* parse the file in question */
			
			syntax_parser_in = fp;

			syntax_parser_parse();

			fclose(fp);
		}
	}
	closedir(dirp);
	libsynt_techs[libsynt_tech_count].name = 0;
	
	/* dump all of the 8 different structs defined in syntax.h, and the hash tables, too */

	printf("Generating syntax_decls.c and syntax_decls.h...\n");
	
	outC = fopen("syntax_decls.c","w");
	outH = fopen("syntax_decls.h","w");
	if( !outC || !outH )
	{
		printf("Couldn't open 'syntax_decls.c' or 'syntax_decls.h'; Solve the problem and re-run.\n");
		exit(130);
	}

	fprintf(outC,"#include \"syntax.h\"\n");
	fprintf(outC,"#include \"syntax_decls.h\"\n");
	
	for(i=0;i< libsynt_tech_count; i++)
	{
		fprintf(outH,"\n\n/* Technology: %s       */\n\n",
				libsynt_techs[i].name);
		fprintf(outC,"\n\n/* Technology: %s       */\n\n",
				libsynt_techs[i].name);

		dump_group(libsynt_techs[i].lib, outC, outH);
		dump_group_hash(libsynt_techs[i].group_hash, outC, outH);
	}
	fprintf(outC,"libsynt_technology libsynt_techs[%d] = {\n", libsynt_tech_count );
	for(i=0;i< libsynt_tech_count; i++)
	{
		fprintf(outC,"{\"%s\", %s, %s },\n", 
				libsynt_techs[i].name, make_rep("group", libsynt_techs[i].lib), make_rep("ht", libsynt_techs[i].group_hash));
	}
	fprintf(outC,"};\n");
	fprintf(outC,"int libsynt_tech_num = %d;\n", libsynt_tech_count);
	
	fclose(outC);
	fclose(outH);

	printf("Generating attr_enum.h...\n");

	outH = fopen("attr_enum.h","w");

	if( !outH )
	{
		printf("Couldn't open attr_enum.h. Fix the problem and rerun.\n");
		exit(130);
	}
	fprintf(outH,"\n\ntypedef enum\n{\n" );

	i=1;
	
	for(hb = libsynt_attrs->all_list; hb ; hb=hb->all_next)
	{
		fprintf(outH,"LIBERTY_ATTRENUM_%s					= %d,\n", hb->name, i++);
	}
	
	fprintf(outH,"} attr_enum;\n\n");
	
	fclose(outH);

	printf("Generating group_enum.h...\n");

	outH = fopen("group_enum.h","w");

	if( !outH )
	{
		printf("Couldn't open group_enum.h. Fix the problem and rerun.\n");
		exit(130);
	}
	fprintf(outH,"\n\ntypedef enum\n{\n" );

	i=1;
	
	for(hb = libsynt_allgroups->all_list; hb ; hb=hb->all_next)
	{
		fprintf(outH,"LIBERTY_GROUPENUM_%s					= %d,\n", hb->name, i++);
	}
	
	
	fprintf(outH,"} group_enum;\n\n");
	
	fclose(outH);

	printf("Generating group_lookup...\n");

	outC = fopen("group_lookup","w");

	if( !outC )
	{
		printf("Couldn't open group_lookup. Fix the problem and rerun.\n");
		exit(130);
	}
	
	fprintf(outC,"%%{\n#include \"group_enum.h\"\n%%}\n" );
	fprintf(outC,"struct libGroupMap { char *name; group_enum type; };\n%%%%\n");
	

	for(hb = libsynt_allgroups->all_list; hb ; hb=hb->all_next)
	{
		fprintf(outC,"%s,	LIBERTY_GROUPENUM_%s\n", hb->name,hb->name);
	}
	
	fprintf(outC,"%%%%\n");
	
	fclose(outC);

	printf("Generating attr_lookup...\n");

	outC = fopen("attr_lookup","w");

	if( !outC )
	{
		printf("Couldn't open attr_lookup. Fix the problem and rerun.\n");
		exit(130);
	}
	
	fprintf(outC,"%%{\n#include \"attr_enum.h\"\n%%}\n" );
	fprintf(outC,"struct libGroupMap { char *name; attr_enum type; };\n%%%%\n");
	

	for(hb = libsynt_attrs->all_list; hb ; hb=hb->all_next)
	{
		fprintf(outC,"%s,	LIBERTY_ATTRENUM_%s\n", hb->name,hb->name);
	}

	fprintf(outC,"%%%%\n");
	
	fclose(outC);

	
	printf("...Done.\n");
	exit(0);
}

char *attr_type_to_string(libsynt_attr_type t);

char *attr_type_to_string(libsynt_attr_type t)
{
	switch( t )
	{
	case SYNTAX_ATTRTYPE_STRING:
		return "SYNTAX_ATTRTYPE_STRING";
		
	case SYNTAX_ATTRTYPE_VIRTUAL:
		return "SYNTAX_ATTRTYPE_VIRTUAL";
		
	case SYNTAX_ATTRTYPE_ENUM:
		return "SYNTAX_ATTRTYPE_ENUM";
		
	case SYNTAX_ATTRTYPE_FLOAT:
		return "SYNTAX_ATTRTYPE_FLOAT";
		
	case SYNTAX_ATTRTYPE_INT:
		return "SYNTAX_ATTRTYPE_INT";
		
	case SYNTAX_ATTRTYPE_BOOLEAN:
		return "SYNTAX_ATTRTYPE_BOOLEAN";
		
	case SYNTAX_ATTRTYPE_COMPLEX:
		return "SYNTAX_ATTRTYPE_COMPLEX";

	case SYNTAX_ATTRTYPE_COMPLEX_UNKNOWN:
		return "SYNTAX_ATTRTYPE_COMPLEX_UNKNOWN";

	case SYNTAX_ATTRTYPE_COMPLEX_LIST:
		return "SYNTAX_ATTRTYPE_COMPLEX_LIST";
	}
	return "SYNTAX_ATTRTYPE_COMPLEX_UNKNOWN";
}


/* dump attr */
void dump_attr(libsynt_attribute_info *a, FILE *outC, FILE *outH)
{
	int i;
	libsynt_argument *arg;

	if( !a )
		return;
	
	switch( a->type )
	{
	case SYNTAX_ATTRTYPE_STRING:
		fprintf(outH,"libsynt_attribute_info attr_%lx;\n", a);
		
		fprintf(outC,"libsynt_attribute_info attr_%lx = { \"%s\", %s, %d, {0}, %s};\n",
				a,
				a->name,
				attr_type_to_string(a->type), 
				a->lineno, 
				make_rep("attr",a->next));
				
		break;
		
	case SYNTAX_ATTRTYPE_VIRTUAL:
		fprintf(outH,"libsynt_attribute_info attr_%lx;\n", a);
		
		fprintf(outC,"libsynt_attribute_info attr_%lx = { \"%s\", %s, %d, {0}, %s};\n",
				a,
				a->name,
				attr_type_to_string(a->type), a->lineno, make_rep("attr",a->next));
		
		break;
		
	case SYNTAX_ATTRTYPE_ENUM:
		fprintf(outC,"char *enum_%lx[%d] = {",
				a->u.stringenum->array, a->u.stringenum->size);
		for(i=0;i<a->u.stringenum->size;i++)
		{
			fprintf(outC,"\"%s\",", a->u.stringenum->array[i]);
		}
		fprintf(outC,"};\n");
		fprintf(outC,"libsynt_string_enum strenum_%lx = { enum_%lx, %d };\n", a->u.stringenum, a->u.stringenum->array, a->u.stringenum->size);
		fprintf(outH,"libsynt_attribute_info attr_%lx;\n", a);
		
		fprintf(outC,"libsynt_attribute_info attr_%lx = { \"%s\", %s, %d, {(void*)%s}, %s};\n",
				a,
				a->name,
				attr_type_to_string(a->type), a->lineno,
				make_rep("strenum",a->u.stringenum), make_rep("attr",a->next));
		
		break;
		
	case SYNTAX_ATTRTYPE_FLOAT:
		if( a->u.floatcon && a->u.floatcon->refd_default )
			fprintf(outC,"libsynt_float_constraint floatcons_%lx = {%g, %g, %g, \"%s\", %d, %d, %d, %d};\n",
					a->u.floatcon,
					a->u.floatcon->greater_than, a->u.floatcon->less_than, a->u.floatcon->default_value,
					a->u.floatcon->refd_default,
					a->u.floatcon->greater_specd, a->u.floatcon->less_specd, a->u.floatcon->default_specd, a->u.floatcon->ref_default_specd );
		
		else if( a->u.floatcon && !a->u.floatcon->refd_default )
			fprintf(outC,"libsynt_float_constraint floatcons_%lx = {%g, %g, %g, 0, %d, %d, %d, %d};\n",
					a->u.floatcon,
					a->u.floatcon->greater_than, a->u.floatcon->less_than, a->u.floatcon->default_value,
					a->u.floatcon->greater_specd, a->u.floatcon->less_specd, a->u.floatcon->default_specd, a->u.floatcon->ref_default_specd );
		fprintf(outH,"libsynt_attribute_info attr_%lx;\n", a);
		fprintf(outC,"libsynt_attribute_info attr_%lx = { \"%s\", %s, %d, {(void*)%s}, %s};\n",
				a,
				a->name,
				attr_type_to_string(a->type), a->lineno,
				make_rep("floatcons",a->u.stringenum), make_rep("attr",a->next));
		break;
		
	case SYNTAX_ATTRTYPE_INT:
		if( a->u.intcon && a->u.intcon->refd_default )
			fprintf(outC,"libsynt_int_constraint intcons_%lx = {%d, %d, %d, \"%s\", %d, %d, %d, %d};\n",
					a->u.intcon,
					a->u.intcon->greater_than, a->u.intcon->less_than, a->u.intcon->default_value,
					a->u.intcon->refd_default,
					a->u.intcon->greater_specd, a->u.intcon->less_specd, a->u.intcon->default_specd, a->u.intcon->ref_default_specd );
		
		else if( a->u.intcon && !a->u.intcon->refd_default )
			fprintf(outC,"libsynt_int_constraint intcons_%lx = {%d, %d, %d, 0, %d, %d, %d, %d};\n",
					a->u.intcon,
					a->u.intcon->greater_than, a->u.intcon->less_than, a->u.intcon->default_value,
					a->u.intcon->greater_specd, a->u.intcon->less_specd, a->u.intcon->default_specd, a->u.intcon->ref_default_specd );
		fprintf(outH,"libsynt_attribute_info attr_%lx;\n", a);
		fprintf(outC,"libsynt_attribute_info attr_%lx = { \"%s\", %s, %d, {(void*)%s}, %s};\n",
				a,
				a->name,
				attr_type_to_string(a->type), a->lineno,
				make_rep("intcons",a->u.stringenum), make_rep("attr",a->next));
		break;
		
	case SYNTAX_ATTRTYPE_BOOLEAN:
		fprintf(outH,"libsynt_attribute_info attr_%lx;\n", a);
		
		fprintf(outC,"libsynt_attribute_info attr_%lx = { \"%s\", %s, %d, {0}, %s};\n",
				a,
				a->name,
				attr_type_to_string(a->type), a->lineno, make_rep("attr",a->next));
		
		break;
		
	case SYNTAX_ATTRTYPE_COMPLEX:
	case SYNTAX_ATTRTYPE_COMPLEX_UNKNOWN:
	case SYNTAX_ATTRTYPE_COMPLEX_LIST:
		for(arg=a->u.args;arg;arg=arg->next)
		{
			fprintf(outH,"libsynt_argument arg_%lx;\n", arg);
			fprintf(outC,"libsynt_argument arg_%lx = {%s, %s};\n", arg, attr_type_to_string(arg->type), make_rep("arg",arg->next) );
		}
		fprintf(outH,"libsynt_attribute_info attr_%lx;\n", a);
		fprintf(outC,"libsynt_attribute_info attr_%lx = { \"%s\", %s, %d, {(void*)%s}, %s};\n",
				a,
				a->name,
				attr_type_to_string(a->type), a->lineno,
				make_rep("arg",a->u.stringenum), make_rep("attr",a->next));
		break;
		
	}
}

char *name_cons_to_str(libsynt_name_constraints x);

char *name_cons_to_str(libsynt_name_constraints x)
{
	switch( x )
	{
	case SYNTAX_GNAME_NONE:
		return "SYNTAX_GNAME_NONE";
		
	case SYNTAX_GNAME_ONE:
		return "SYNTAX_GNAME_ONE";
		
	case SYNTAX_GNAME_TWO:
		return "SYNTAX_GNAME_TWO";
		
	case SYNTAX_GNAME_THREE:
		return "SYNTAX_GNAME_THREE";
		
	case SYNTAX_GNAME_NONE_OR_ONE:
		return "SYNTAX_GNAME_NONE_OR_ONE";
		
	case SYNTAX_GNAME_ONE_OR_MORE:
		return "SYNTAX_GNAME_ONE_OR_MORE";
		
	}
}

/* dump group */

void dump_group(libsynt_group_info *g, FILE *outC, FILE *outH)
{
	libsynt_group_info *ng;
	libsynt_attribute_info *at;

	g->mark = 1;
	
	for(ng = g->group_list; ng; ng=ng->next)
	{
		if( !ng->mark )
		{
			dump_group(ng, outC, outH);
			ng->mark++;
		}
	}
	for(at = g->attr_list; at; at=at->next)
	{
		dump_attr(at, outC, outH);
	}
	dump_group_hash(g->group_hash, outC, outH);
	dump_attr_hash(g->attr_hash, outC, outH);
	fprintf(outC,"libsynt_group_info group_%lx = {%d, %s, \"%s\", %d, %d, %s, %s, %s, %s, %s, %s};\n",
			g, g->mark, name_cons_to_str(g->name_constraints), g->type, g->ID, g->lineno,
			make_rep("attr",g->attr_list), make_rep("group",g->group_list),
			make_rep("ht",g->attr_hash), make_rep("ht",g->group_hash), make_rep("group",g->next), make_rep("group",g->ref));
	fprintf(outH,"libsynt_group_info group_%lx;\n", g);
}
