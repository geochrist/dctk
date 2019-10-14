%module liberty
%{
#include "si2dr_liberty.h"
%}


%include typemaps.i

/*
%apply int *INOUT { si2drErrorT *err };
this might have been an elegant way to map this sort of thing, but
it didn't work in swig 1.3.31; so.... I copy in the .h file,
and hack away to make this work better in perl...
*/

/******************************************************************************
    Copyright (c) 2007 Synopsys, Inc.    ALL RIGHTS RESERVED

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

/* define a PI for manipulating the liberty data */

/* basic data types */

/* ***********************************************************************
 File contents : LIBERTY-DR type definitions.
*********************************************************************** */
/*
 * These primitive types are compatible with the ANSI C Standard (ANSI/
 * X3.159-1990 Programming Language C Standard.) 
 */
typedef enum si2BooleanEnum { SI2_FALSE = 0, SI2_TRUE = 1 } si2BooleanT;
typedef long int  si2LongT;
typedef float     si2FloatT;
typedef double    si2DoubleT;
typedef char*     si2StringT;
typedef void      si2VoidT;

typedef enum si2TypeEnum {
   SI2_UNDEFINED_VALUETYPE = 0,
   SI2_BOOLEAN = 1,
   SI2_LONG = 2,
   SI2_FLOAT = 3,
   SI2_DOUBLE = 4,
   SI2_STRING = 5,
   SI2_VOID = 6,
   SI2_OID = 7,
   SI2_ITER = 8,
   SI2_EXPR = 9,
   SI2_MAX_VALUETYPE = 10
   } si2TypeT;

/*
 * Size limits
 */

#define SI2_LONG_MAX   2147483647
#define SI2_LONG_MIN  -2147483647
#define SI2_ULONG_MAX  4294967295


typedef struct si2OID  {void *v1, *v2;} si2ObjectIdT;
/*
 * Iterators are blind handles the size of a single pointer.
 */
typedef void *si2IteratorIdT;


#define SI2DR_MIN_FLOAT32  -1E+37
#define SI2DR_MAX_FLOAT32   1E+37
#define SI2DR_MIN_FLOAT64  -1.797693e+308
#define SI2DR_MAX_FLOAT64   1.797693e+308
#define SI2DR_MIN_INT32    -2147483647
#define SI2DR_MAX_INT32     2147483647

#define SI2DR_TRUE SI2_TRUE
#define SI2DR_FALSE SI2_FALSE

typedef si2BooleanT si2drBooleanT;

typedef si2LongT    si2drInt32T;
typedef si2FloatT   si2drFloat32T;
typedef si2DoubleT   si2drFloat64T;
typedef si2StringT  si2drStringT;
typedef si2VoidT    si2drVoidT;

typedef si2IteratorIdT	si2drIterIdT;
typedef si2ObjectIdT	si2drObjectIdT;

typedef si2drObjectIdT si2drGroupIdT;
typedef si2drObjectIdT si2drAttrIdT;
typedef si2drObjectIdT si2drDefineIdT;

typedef si2drIterIdT si2drObjectsIdT;
typedef si2drIterIdT si2drGroupsIdT;
typedef si2drIterIdT si2drAttrsIdT;
typedef si2drIterIdT si2drDefinesIdT;
typedef si2drIterIdT si2drNamesIdT;
typedef si2drIterIdT si2drValuesIdT;

typedef enum si2drSeverityT 
{
	SI2DR_SEVERITY_NOTE = 0,
	SI2DR_SEVERITY_WARN = 1,
	SI2DR_SEVERITY_ERR  = 2
} si2drSeverityT;


typedef enum si2drObjectTypeT 
{
    SI2DR_UNDEFINED_OBJECTTYPE =  0,
    SI2DR_GROUP                =  1,
    SI2DR_ATTR                 =  2,
    SI2DR_DEFINE               =  3,
	SI2DR_MAX_OBJECTTYPE       =  4
} si2drObjectTypeT;

typedef enum si2drAttrTypeT
{
	SI2DR_SIMPLE,
	SI2DR_COMPLEX
} si2drAttrTypeT;


typedef enum si2drValueTypeT 
{
    SI2DR_UNDEFINED_VALUETYPE = SI2_UNDEFINED_VALUETYPE,
    SI2DR_INT32               = SI2_LONG,
    SI2DR_STRING              = SI2_STRING,
    SI2DR_FLOAT64             = SI2_DOUBLE,
    SI2DR_BOOLEAN             = SI2_BOOLEAN,
	SI2DR_EXPR                = SI2_EXPR,
    SI2DR_MAX_VALUETYPE       = SI2_MAX_VALUETYPE
} si2drValueTypeT;

typedef enum si2drExprTypeT
{
	SI2DR_EXPR_VAL = 0,     /* unary ops just have left arg set: add, sub, logs, paren */
	SI2DR_EXPR_OP_ADD =  1,
	SI2DR_EXPR_OP_SUB =  2,  
	SI2DR_EXPR_OP_MUL = 3,
	SI2DR_EXPR_OP_DIV =  4,
	SI2DR_EXPR_OP_PAREN =  5,  
	SI2DR_EXPR_OP_LOG2 =  6, /* pretty much flight of fancy from here on? */
	SI2DR_EXPR_OP_LOG10 =  7,
	SI2DR_EXPR_OP_EXP  = 8, /* exponent? */
	
} si2drExprTypeT;
	

typedef struct si2drExprT
{
	si2drExprTypeT type;
	union
	{
		si2drInt32T i;
		si2drFloat64T d;
		si2drStringT s;    /* most likely an identifier */
		si2drBooleanT b;
	} u;
	
	si2drValueTypeT	valuetype; /* if the type is a fixed value */
	
	struct si2drExprT *left; /* the exprs form a classic binary tree rep of an arithmetic expression */
	struct si2drExprT *right;
} si2drExprT;

	
typedef enum si2drErrorT 
{
	SI2DR_NO_ERROR                    	      = 0,
	SI2DR_INTERNAL_SYSTEM_ERROR               = 1,
	SI2DR_INVALID_VALUE                       = 4,
	SI2DR_INVALID_NAME                        = 5,
	SI2DR_INVALID_OBJECTTYPE                  = 6,
	SI2DR_INVALID_ATTRTYPE                    = 10,
	SI2DR_UNUSABLE_OID                        = 7,
	SI2DR_OBJECT_ALREADY_EXISTS               = 8,
	SI2DR_OBJECT_NOT_FOUND                    = 9,
	SI2DR_SYNTAX_ERROR                        = 2,
	SI2DR_TRACE_FILES_CANNOT_BE_OPENED        = 3,
	SI2DR_PIINIT_NOT_CALLED                   = 11,
	SI2DR_SEMANTIC_ERROR                      = 12,
    SI2DR_REFERENCE_ERROR                     = 13,
	SI2DR_MAX_ERROR                           = 14
} si2drErrorT;

/* I dropped the MessageHandler stuff... defining a callback func
   in perl is a bit above and beyond... */

	si2drGroupIdT  si2drPICreateGroup( char * name,
									 char * group_type,
									 int  *REFERENCE);
	

	si2drAttrIdT   si2drGroupCreateAttr( si2drGroupIdT  group,
										 char *   name,
										 si2drAttrTypeT type,     /* si2drAttrTypeT */
										 int   *REFERENCE);

	int  si2drAttrGetAttrType( si2drAttrIdT  attr,
								int   *REFERENCE);
	
	char *   si2drAttrGetName( si2drAttrIdT  attr,
							   int   *REFERENCE);
	
	si2drVoidT     si2drComplexAttrAddInt32Value( si2drAttrIdT attr,
												   si2drInt32T intgr,
												   int *REFERENCE );
	
	si2drVoidT     si2drComplexAttrAddStringValue( si2drAttrIdT attr,
													char *    string,
													int     *REFERENCE );

	si2drVoidT     si2drComplexAttrAddBooleanValue( si2drAttrIdT attr,
													 si2drBooleanT   boolval,
													 int     *REFERENCE );
	
	si2drVoidT     si2drComplexAttrAddFloat64Value( si2drAttrIdT attr,
													 si2drFloat64T   float64,
													 int     *REFERENCE );

    si2drVoidT     si2drComplexAttrAddExprValue( si2drAttrIdT attr,
												   si2drExprT    *expr,
												   int     *REFERENCE );

	si2drValuesIdT si2drComplexAttrGetValues( si2drAttrIdT     attr,
											  int     *REFERENCE );

    si2drAttrComplexValIdT si2drIterNextComplex( si2drValuesIdT iter,
												 int     *REFERENCE );

	si2drValueTypeT  si2drComplexValGetValueType( si2drAttrComplexValIdT attr,
												  int     *REFERENCE );

	si2drInt32T      si2drComplexValGetInt32Value( si2drAttrComplexValIdT attr,
												   int     *REFERENCE );
	
	si2drFloat64T    si2drComplexValGetFloat64Value( si2drAttrComplexValIdT attr,
												     int     *REFERENCE );
	
	si2drStringT     si2drComplexValGetStringValue( si2drAttrComplexValIdT attr,
												    int     *REFERENCE );

	si2drBooleanT    si2drComplexValGetBooleanValue( si2drAttrComplexValIdT attr,
												     int     *REFERENCE );

	si2drExprT      *si2drComplexValGetExprValue( si2drAttrComplexValIdT attr,
												  int     *REFERENCE );

	int  si2drSimpleAttrGetValueType( si2drAttrIdT attr,
		 								int     *REFERENCE );

	int      si2drSimpleAttrGetInt32Value( si2drAttrIdT attr,
												   int     *REFERENCE );
	
	double    si2drSimpleAttrGetFloat64Value( si2drAttrIdT attr,
												   int     *REFERENCE );
	
	char *     si2drSimpleAttrGetStringValue( si2drAttrIdT attr,
											   int     *REFERENCE );

	int    si2drSimpleAttrGetBooleanValue( si2drAttrIdT attr,
												   int     *REFERENCE );

	si2drExprT      *si2drSimpleAttrGetExprValue( si2drAttrIdT attr,
												   int     *REFERENCE );


	
	si2drVoidT       si2drSimpleAttrSetInt32Value( si2drAttrIdT attr,
												   int     intgr,
												   int     *REFERENCE );
	
	si2drVoidT       si2drSimpleAttrSetBooleanValue( si2drAttrIdT   attr,
												   int   intgr,
												   int      *REFERENCE );
	
	si2drVoidT       si2drSimpleAttrSetFloat64Value( si2drAttrIdT attr,
													   double   float64,
													   int     *REFERENCE );
	
	si2drVoidT       si2drSimpleAttrSetStringValue( si2drAttrIdT attr,
													   char *    string,
													   int     *REFERENCE );
	
	si2drVoidT       si2drSimpleAttrSetExprValue( si2drAttrIdT   attr,
												   si2drExprT    *expr,
												   int   *REFERENCE );

	int    si2drSimpleAttrGetIsVar( si2drAttrIdT attr,
											int     *REFERENCE );

	si2drVoidT       si2drSimpleAttrSetIsVar( si2drAttrIdT   attr,
												int      *REFERENCE );

/* helper functions for expr creation, building, and destruction */

    si2drVoidT       si2drExprDestroy( si2drExprT   *expr,  /* recursively free the structures */
                                        int  *REFERENCE);

    si2drExprT      *si2drCreateExpr( si2drExprTypeT type, /*  malloc an Expr and return it */
                                      int  *REFERENCE);

si2drExprT  *si2drCreateBooleanValExpr( int b,
										 int  *REFERENCE );

si2drExprT  *si2drCreateDoubleValExpr( double d,
										int  *REFERENCE );

si2drExprT  *si2drCreateStringValExpr( char * s,
										int  *REFERENCE );

si2drExprT  *si2drCreateIntValExpr( int i,
									 int  *REFERENCE );

si2drExprT  *si2drCreateBinaryOpExpr( si2drExprT *left,
									   si2drExprTypeT  optype,
									   si2drExprT *right,
									   int  *REFERENCE );

si2drExprT  *si2drCreateUnaryOpExpr(  si2drExprTypeT optype, /*  */
									  si2drExprT *expr,
									  int  *REFERENCE );

char *    si2drExprToString( si2drExprT  *expr,
							  int  *REFERENCE );

si2drExprTypeT  si2drExprGetType( si2drExprT  *expr,
								  int  *REFERENCE );

si2drValueTypeT  si2drValExprGetValueType( si2drExprT  *expr,
										   	 int  *REFERENCE );

int    si2drIntValExprGetInt( si2drExprT  *expr,
									   int  *REFERENCE );

double    si2drDoubleValExprGetDouble( si2drExprT  *expr,
											  int  *REFERENCE );

int      si2drBooleanValExprGetBoolean( si2drExprT  *expr,
												  int  *REFERENCE );
												
char *      si2drStringValExprGetString( si2drExprT  *expr,
										 int  *REFERENCE );

si2drExprT   *si2drOpExprGetLeftExpr( si2drExprT  *expr,  /* will apply to Unary & binary Ops */
										int  *REFERENCE );

si2drExprT   *si2drOpExprGetRightExpr( si2drExprT  *expr,
										int  *REFERENCE );

/* Define related funcs */
	
	si2drDefineIdT si2drGroupCreateDefine( si2drGroupIdT group,
											 char * name,
											 char * allowed_group_name,
											 si2drValueTypeT valtype,
											 int  *REFERENCE);
	
    char *   si2drDefineGetName( si2drDefineIdT def,
								 int  *REFERENCE);
	
    char *   si2drDefineGetAllowedGroupName( si2drDefineIdT def,
											 int  *REFERENCE);
	
    si2drValueTypeT   si2drDefineGetValueType( si2drDefineIdT def,
											   int  *REFERENCE);

	si2drGroupIdT  si2drGroupCreateGroup( si2drGroupIdT group,
											 char * name,
											 char * group_type,
											 int  *REFERENCE);

	char *   si2drGroupGetGroupType( si2drGroupIdT group,
										 int  *REFERENCE);
	
	char *   si2drGroupGetComment( si2drGroupIdT group,
										 int  *REFERENCE);
	
	si2drVoidT     si2drGroupSetComment( si2drGroupIdT group,
										 char * comment,
										 int  *REFERENCE);
	
    char *   si2drAttrGetComment( si2drAttrIdT attr,
							 	  int  *REFERENCE);

    si2drVoidT     si2drAttrSetComment( si2drAttrIdT attr,
										char * comment,
										int  *REFERENCE);

    char *   si2drDefineGetComment( si2drDefineIdT def,
									 int  *REFERENCE);

    si2drVoidT     si2drDefineSetComment( si2drDefineIdT def,
										 char * comment,
										 int  *REFERENCE);

	si2drVoidT     si2drGroupAddName( si2drGroupIdT group,
										char * name,
										int  *REFERENCE);
	
	si2drVoidT     si2drGroupDeleteName( si2drGroupIdT group,
										 char * name,
										 int  *REFERENCE);
	
	
	si2drGroupIdT  si2drPIFindGroupByName( char * name,
											char * type,
											int  *REFERENCE);
	
	si2drGroupIdT  si2drGroupFindGroupByName( si2drGroupIdT group,
											 char * name,
											 char * type,
											 int  *REFERENCE);
	
	si2drAttrIdT   si2drGroupFindAttrByName( si2drGroupIdT group,
											 char * name,
											 int  *REFERENCE);
	
	si2drDefineIdT si2drGroupFindDefineByName( si2drGroupIdT group,
												 char * name,
												 int  *REFERENCE);
	

	si2drDefineIdT si2drPIFindDefineByName( char * name,
											  int  *REFERENCE);
	


	si2drGroupsIdT  si2drPIGetGroups( int  *REFERENCE);
	

	si2drGroupsIdT  si2drGroupGetGroups( si2drGroupIdT group,
											int  *REFERENCE);

	si2drNamesIdT   si2drGroupGetNames( si2drGroupIdT group,
										int  *REFERENCE);

	si2drAttrsIdT   si2drGroupGetAttrs( si2drGroupIdT group,
										int  *REFERENCE);

	si2drDefinesIdT si2drGroupGetDefines( si2drGroupIdT group,
											int  *REFERENCE);

	si2drGroupIdT   si2drIterNextGroup( si2drGroupsIdT iter,
									   int  *REFERENCE);

	char *    si2drIterNextName( si2drNamesIdT iter,
								   int  *REFERENCE);

	si2drAttrIdT    si2drIterNextAttr( si2drAttrsIdT iter,
									   int  *REFERENCE);

	si2drDefineIdT  si2drIterNextDefine( si2drDefinesIdT iter,
									   int  *REFERENCE);

	si2drVoidT      si2drIterQuit( si2drIterIdT iter,
								   int  *REFERENCE);

	si2drVoidT      si2drObjectDelete( si2drObjectIdT object,
									   int  *REFERENCE);

	char *     si2drPIGetErrorText( int errorCode,
								 int  *REFERENCE);
	
	si2drObjectIdT   si2drPIGetNullId( int  *REFERENCE);

	si2drVoidT       si2drPIInit( int  *REFERENCE);

	si2drVoidT       si2drPIQuit( int  *REFERENCE);

	si2drObjectTypeT  si2drObjectGetObjectType( si2drObjectIdT object,
									int  *REFERENCE);

	si2drObjectIdT   si2drObjectGetOwner( si2drObjectIdT object,
										 int  *REFERENCE);

	si2drBooleanT    si2drObjectIsNull( si2drObjectIdT object,
										 int  *REFERENCE);
	
	si2drBooleanT    si2drObjectIsSame( si2drObjectIdT object1,
										 si2drObjectIdT object2,
										 int  *REFERENCE);
	
	si2drBooleanT    si2drObjectIsUsable( si2drObjectIdT object,
										 int  *REFERENCE);

	si2drVoidT       si2drObjectSetFileName( si2drObjectIdT object,
											 char *   filename,
											 int  *REFERENCE);

	si2drVoidT       si2drObjectSetLineNo( si2drObjectIdT object,
										 int    lineno,
										 int  *REFERENCE);

	si2drInt32T      si2drObjectGetLineNo( si2drObjectIdT object,
											 int  *REFERENCE);

	char *     si2drObjectGetFileName( si2drObjectIdT object,
										 int  *REFERENCE);

	si2drVoidT       si2drReadLibertyFile( char *filename,
											 int  *REFERENCE);

	si2drVoidT       si2drWriteLibertyFile( char *filename,
											 si2drGroupIdT group,
											 int  *REFERENCE);
	
    si2drVoidT       si2drCheckLibertyLibrary( si2drGroupIdT group,
												 int  *REFERENCE);
	

	si2drBooleanT  si2drPIGetTraceMode(int  *REFERENCE);
	
	si2drVoidT     si2drPIUnSetTraceMode(int  *REFERENCE);
	
	si2drVoidT     si2drPISetTraceMode(char *fname,
									   int  *REFERENCE);
	

	si2drVoidT       si2drPISetDebugMode(int  *REFERENCE);
	
	si2drVoidT       si2drPIUnSetDebugMode(int  *REFERENCE);
	
	si2drBooleanT    si2drPIGetDebugMode(int  *REFERENCE);
																										  
	
	si2drVoidT     si2drPISetNocheckMode(int  *REFERENCE);
	
	si2drVoidT     si2drPIUnSetNocheckMode(int  *REFERENCE);
	
	si2drBooleanT  si2drPIGetNocheckMode(int  *REFERENCE);
		 

	si2drVoidT     si2drGroupMoveAfter(si2drGroupIdT groupToMove, 
										 si2drGroupIdT targetGroup,
										 int   *REFERENCE);
	
	si2drVoidT     si2drGroupMoveBefore(si2drGroupIdT groupToMove, 
										  si2drGroupIdT targetGroup, 
										  int *REFERENCE);




