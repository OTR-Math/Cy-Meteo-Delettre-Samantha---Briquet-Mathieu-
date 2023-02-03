// ===============================================================
// SORT_NODES_C
// ===============================================================
#include "headers/utils.h"

#include "headers/sortNodes.h"


// ===============================================================
// STRUCTS FUNCTIONS
// ===============================================================
#define max(a,b) (a>=b?a:b)
#define min(a,b) (a<=b?a:b)

#define t1_pattern_scan "%[^;];%f;"
#define t1_pattern_scan_agregated "%[^;];%f;%f;%f;%d;"
#define t1_pattern_stor_agregated "%s;%f;%f;%f;%d;\n"

// ===============================================================
// FUNCTIONS
// ===============================================================
char* getComparField( char* data2Store, char* graphMode)
{
	char comparField[1024];

	// t1 => 1 data -> min, max, avr (+counter to calculate next average)
	if( strcmp( graphMode, "t3") == 0
	 || strcmp( graphMode, "p3") == 0
	){
	    char    station[256];
      char    date[256];
      char    hour[256];
	    sscanf( data2Store, "%[^;];%[^T]T%[^:]:", station, date, hour);
	    sprintf( comparField, "%s%s%s", station, hour, date);
	} else
	if( strcmp( graphMode, "mSecondary")  == 0
	){
	    char    dataField1[256];
      char    dataField2[256];
	    sscanf( data2Store, "%[^;];%[^;];",         dataField1, dataField2);
	    sprintf( comparField, "%s", dataField2);
	} else
	if( strcmp( graphMode, "h")  == 0
	){
	    char    dataField1[256];
      char    dataField2[256];
      char    dataField3[256];
	    sscanf( data2Store, "%[^;];%[^;];%[^;];",         dataField1, dataField2, dataField3);
	    sprintf( comparField, "%s", dataField3);
	} else
	{
	    char comparField[256];
	    sscanf( data2Store, "%[^;];", comparField);
	}

	return strdup( comparField);
}


char* agregateData( char* data2Store, char* graphMode)
{
	char newData2Store[1024];

	// t1 => 1 data -> min, max, avr (+counter to calculate next average)
	if( strcmp( graphMode, "t1") == 0
	 || strcmp( graphMode, "t2") == 0
	 || strcmp( graphMode, "p1") == 0
	 || strcmp( graphMode, "p2") == 0
	){
	    char      comparField[256];
	    float       dataField;
	    sscanf(     data2Store, "%[^;];%f;",         comparField, &dataField);
//	    char  comparField[256], unusedField1[256], unusedField2[256];
//	    float   dataField;
//	    sscanf( data2Store, "%[^;];%[^;];%[^;];%f;", comparField, unusedField1, unusedField2, &dataField);
	    sprintf( newData2Store, "%s;%f;%f;%f;1;\n", comparField,  dataField, dataField, dataField);
	} else
	// w => 2 data -> 2 avr (+2 counters to calculate next average)
	if( strcmp( graphMode, "w") == 0
	){
	    char      comparField[256], additionalData[256];
	    float       dataField1, dataField2;
	    sscanf(     data2Store, "%[^;];%f;%f;%[^\n]",         comparField, &dataField1, &dataField2, additionalData);
	    sprintf( newData2Store, "%s;%f;%f;%s;1;1;\n", comparField,  dataField1, dataField2, additionalData);
	} else
	// w => 2 data -> 2 avr (+2 counters to calculate next average)
	if( strcmp( graphMode, "m") == 0
	){
	    char      comparField[256], additionalData[256];
	    int         dataField;
	    sscanf(     data2Store, "%[^;];%d;%[^\n]",         comparField, &dataField, additionalData);
	    sprintf( newData2Store, "%s;%d;%s\n", comparField, dataField, additionalData);
	} else
	{
	    strcpy( newData2Store, data2Store);
	}

	return strdup( newData2Store);
}

char* agregateAndMergeData( char* storedData, char* data2Store, char* graphMode)
{
	// t1 => 1 data -> min, max, avr (+counter to calculate next average)
	if( strcmp( graphMode, "t1") == 0
	 || strcmp( graphMode, "t2") == 0
	 || strcmp( graphMode, "p1") == 0
	 || strcmp( graphMode, "p2") == 0
	){
	    char newData2Store[ 2 * strlen( storedData)];

	    char  comparField[256];
	    float   dataField;
	    sscanf( data2Store, "%[^;];%f;", comparField, &dataField);
	    float oldMin, oldMax, oldAvr;
	    int   oldCount;
	    sscanf( storedData,    "%[^;];%f;%f;%f;%d;", comparField,     &oldMin,                 &oldMax,            &oldAvr, &oldCount);
	    oldAvr =( oldAvr * oldCount + dataField) / ++oldCount;
	    sprintf(   newData2Store, "%s;%f;%f;%f;%d;\n", comparField, min( oldMin, dataField), max( oldMax, dataField), oldAvr,  oldCount);
	    return strdup( newData2Store);
	} else
	// w => 2 data -> 2 avr (+2 counters to calculate next average)
	if( strcmp( graphMode, "w") == 0
	){
	    char newData2Store[ 2 * strlen( storedData)];

	    char  comparField[256], additionalData[256];
	    float   dataField1, dataField2;
	    sscanf( data2Store, "%[^;];%f;%f;%[^\n]", comparField, &dataField1, &dataField2, additionalData);
	    float oldAvr1, oldAvr2;
	    int   oldCount1, oldCount2;
	    sscanf( storedData,    "%[^;];%f;%f;%[^;];%d;%d;", comparField,     &oldAvr1,                 &oldAvr2,            additionalData, &oldCount1, &oldCount2);
	    oldAvr1 =( oldAvr1 * oldCount1 + dataField1) / ++oldCount1;
	    oldAvr2 =( oldAvr2 * oldCount2 + dataField2) / ++oldCount2;
	    sprintf(   newData2Store, "%s;%f;%f;%s;%d;%d;\n", comparField, oldAvr1, oldAvr2, additionalData, oldCount1, oldCount2);
	    return strdup( newData2Store);
	} else
	// w => 2 data -> 2 avr (+2 counters to calculate next average)
	if( strcmp( graphMode, "m") == 0
	){
	    char newData2Store[ 2 * strlen( storedData)];

	    char  comparField[256], additionalData[256];
	    int   dataField;
	    sscanf( data2Store, "%[^;];%d;%[^\n]", comparField, &dataField, additionalData);
	    int oldMax;
	    sscanf( storedData, "%[^;];%d;%s", comparField, &oldMax, additionalData);
	    sprintf(   newData2Store, "%s;%d;%s\n", comparField, max( oldMax, dataField), additionalData);
	    return strdup( newData2Store);
	} else
	{
	    char newData2Store[ 1 + strlen( storedData) + strlen( data2Store)];

	    strcpy( newData2Store, storedData);
	    strcat( newData2Store, data2Store);
	    return strdup( newData2Store);
	}
}

bool abrInsert(
	  TypNode* *comparisonNodes // in/out struct pointer
	, char*     data2Store
	, char*     graphMode
	, int       level
	, int*      balanceChange
	, bool      withAvlSelfBalancing
){
	if( g_isDebugModeOn) printf( "%d->", level);

  char* comparField = getComparField( data2Store, graphMode);

	// current node - null
	if( *comparisonNodes == NULL)
	{
	    *comparisonNodes = calloc( 1, sizeof( TypNode));
	}

	// current node - empty
	if( (*comparisonNodes)->comparisonValue == NULL)
	{
	    if( g_isDebugModeOn) printf( "%s\n", comparField);
	    (*comparisonNodes)->comparisonValue = strdup( comparField);
	    (*comparisonNodes)->storedData      = agregateData( data2Store, graphMode);
	    (*comparisonNodes)->balance = 0;
	    *balanceChange = 1; // alert parent of change

    	return 0;
      // STOP HERE
	}
  
	// current node - exist
	if( strcmp( comparField, (*comparisonNodes)->comparisonValue) == 0)
	{
	    if( g_isDebugModeOn) printf( "%s =========================twice: %s", comparField, (*comparisonNodes)->storedData);
	    char* newData2Store = agregateAndMergeData( (*comparisonNodes)->storedData, data2Store, graphMode);
	    freePtr( (*comparisonNodes)->storedData);
	    (*comparisonNodes)->storedData = newData2Store;
	    *balanceChange = 0; // no impact for parent

    	return 0;
      // STOP HERE
	}

  // left nodes
  int val1 = atoi( comparField);
  int val2 = atoi( (*comparisonNodes)->comparisonValue);
  bool result =( val1<val2);
  if( result == 1
   ||(( val1 + val2 == 0) && strcmp( comparField /*word*/, (*comparisonNodes)->comparisonValue) < 0)
	){
	    if( g_isDebugModeOn) printf( "l");
	    abrInsert( &(*comparisonNodes)->leftNode, data2Store, graphMode, level + 1, balanceChange, withAvlSelfBalancing);
      if(              *balanceChange != 0) { *balanceChange = -1; }      // new level added on left - forward to parent
      (*comparisonNodes)->balance += *balanceChange;
      if( (*comparisonNodes)->balance >= 0) { *balanceChange = 0; } // was positive = back to stable - no impact on parent
      if( g_isDebugModeOn) printf( "%d(%d)<<", level, (*comparisonNodes)->balance);
  }
  else
  // right nodes
  {
      if( g_isDebugModeOn) printf( "r");
	    abrInsert( &(*comparisonNodes)->rightNode, data2Store, graphMode, level + 1, balanceChange, withAvlSelfBalancing);
	    if(              *balanceChange != 0) { *balanceChange = 1; }      // new level added on right - forward to parent
	    (*comparisonNodes)->balance += *balanceChange;
	    if( (*comparisonNodes)->balance <= 0) { *balanceChange = 0; } // was negative = back to stable - no impact on parent
	    if( g_isDebugModeOn) printf( "%d(%d)<<", level, (*comparisonNodes)->balance);
	}
	// AVL self balancing
	if( withAvlSelfBalancing)
	{
	    if( (*comparisonNodes)->balance <= -2)
	    {
	        if( g_isDebugModeOn) printf( "\n");
	        avlRightBalancing( comparisonNodes);
	    }
	    else if( (*comparisonNodes)->balance >= 2)
	    {
	        if( g_isDebugModeOn) printf( "\n");
	        avlLeftBalancing( comparisonNodes);
	    }
	}

  return 0;
}



// ===============================================================
void avlLeftBalancing( TypNode* *comparisonNodes)
{
	if( (*comparisonNodes)->rightNode->balance <= -1)
	{
	    if( g_isDebugModeOn) printf( "Sub-");
	    avlRightBalancing( &(*comparisonNodes)->rightNode);
	}
	if( g_isDebugModeOn) printf( "Node left-balancing required!!! (%d)\n", (*comparisonNodes)->balance);

	TypNode* pivot                = (*comparisonNodes)->rightNode;
	(*comparisonNodes)->rightNode =              pivot->leftNode;
	pivot->leftNode               = (*comparisonNodes);
	int saveBalance               = (*comparisonNodes)->balance;
	int savePivotBalance          =              pivot->balance;
	(*comparisonNodes)->balance   = saveBalance - max( savePivotBalance, 0) - 1;
	pivot->balance                = min( saveBalance                    - 2
	                              , min( saveBalance + savePivotBalance - 2
	                                    ,               savePivotBalance - 1));
	(*comparisonNodes) = pivot;
}



// ===============================================================
void avlRightBalancing( TypNode* *comparisonNodes)
{
	if( (*comparisonNodes)->leftNode->balance >= 1)
	{
	    if( g_isDebugModeOn) printf( "Sub-");
	    avlLeftBalancing( &(*comparisonNodes)->leftNode);
	}
	if( g_isDebugModeOn) printf( "Node right-balancing required!!! (%d)\n", (*comparisonNodes)->balance);

	TypNode* pivot               = (*comparisonNodes)->leftNode;
	(*comparisonNodes)->leftNode =              pivot->rightNode;
	pivot->rightNode             = (*comparisonNodes);
	int saveBalance              = (*comparisonNodes)->balance;
	int savePivotBalance         =              pivot->balance;
	(*comparisonNodes)->balance  = saveBalance - min( savePivotBalance, 0) + 1;
	pivot->balance               = max( saveBalance                    + 2
	                             , max( saveBalance + savePivotBalance + 2
	                                   ,               savePivotBalance + 1));
	(*comparisonNodes) = pivot;
}



// ===============================================================
bool abrPrintOut(
	  TypNode* comparisonNodes
	, FILE*    outputDataFile
  , bool     withReverseOrder
	, int      level
	, bool     isLeft
	, int*     deepLeft
	, int*     deepRight
){
	if( isLeft) { *deepLeft  = max( *deepLeft,  level); }
	       else { *deepRight = max( *deepRight, level); }
	// write left Nodes
	if( withReverseOrder == false && comparisonNodes->leftNode != NULL)
	{
	    if( g_isDebugModeOn)
	    {
	        printf( "%d", level);
	        if( isLeft) { printf( " (l%d)->l", *deepLeft); }
	               else { printf( " (r%d)->l", *deepRight);}
	    }
	    abrPrintOut( comparisonNodes->leftNode, outputDataFile, withReverseOrder, level + 1, isLeft || level == 0, deepLeft, deepRight);
	    freePtr( comparisonNodes->leftNode);
	    if( g_isDebugModeOn) printf( "<l%d<<", level + 1);
	}
	// write right Nodes
	if( withReverseOrder == true && comparisonNodes->rightNode != NULL)
	{
	    if( g_isDebugModeOn)
	    {
	        printf( "%d", level);
	        if( isLeft) { printf( " (l%d)->r", *deepLeft); }
	               else { printf( " (r%d)->r", *deepRight);}
	    }
	    abrPrintOut( comparisonNodes->rightNode, outputDataFile, withReverseOrder, level + 1, isLeft && level != 0, deepLeft, deepRight);
	    freePtr( comparisonNodes->rightNode);
	    if( g_isDebugModeOn) printf( "<r%d<<", level + 1);
	}
	// then write current Node
	fprintf( outputDataFile, "%s", comparisonNodes->storedData);
	if( g_isDebugModeOn)
	{
	    printf( "%d->\n", level);
//	    printf( "%s\n", comparisonNodes->comparisonValue);
	    printf( "%s", comparisonNodes->storedData);
	    printf( "\t\t");
	}

	freePtr( comparisonNodes->comparisonValue);
	freePtr( comparisonNodes->storedData);

	// continue with right Nodes
	if( withReverseOrder == false && comparisonNodes->rightNode != NULL)
	{
	    if( g_isDebugModeOn)
	    {
	        printf( "%d", level);
	        if( isLeft) { printf( " (l%d)->r", *deepLeft); }
	               else { printf( " (r%d)->r", *deepRight);}
	    }
	    abrPrintOut( comparisonNodes->rightNode, outputDataFile, withReverseOrder, level + 1, isLeft && level != 0, deepLeft, deepRight);
	    freePtr( comparisonNodes->rightNode);
	    if( g_isDebugModeOn) printf( "<r%d<<", level + 1);
	}
	// continue with left Nodes
	if( withReverseOrder == true && comparisonNodes->leftNode != NULL)
	{
	    if( g_isDebugModeOn)
	    {
	        printf( "%d", level);
	        if( isLeft) { printf( " (l%d)->l", *deepLeft); }
	               else { printf( " (r%d)->l", *deepRight);}
	    }
	    abrPrintOut( comparisonNodes->leftNode, outputDataFile, withReverseOrder, level + 1, isLeft || level == 0, deepLeft, deepRight);
	    freePtr( comparisonNodes->leftNode);
	    if( g_isDebugModeOn) printf( "<l%d<<", level + 1);
	}

	return 0;
}



// ===============================================================


