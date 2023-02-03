// ===============================================================
// MAIN_C
// ===============================================================
#include "headers/utils.h"

#include "headers/sortNodes.h"
//#include "headers/menus.h"
//#include "headers/users.h"


// ===============================================================
// MANAGE EXIT with taking care to free all pointers - they need to be globals
// ===============================================================
TypNode* gComparisonNodes = NULL;
TypNode* gComparisonNodes2 = NULL;
bool g_isDebugModeOn = false;

void cleanExit()
{
	freePtr( gComparisonNodes);

//	if( g_isDebugModeOn) clearScreen();
	if( g_isDebugModeOn) printf("\nMerci de votre visite, au plaisir de vous revoir, bonne journee !!!\n");
	if( g_isDebugModeOn) waitUntilKeyPressed( .silentMode = 1);

	exit(0);
}

// ===============================================================
// MAIN
// ===============================================================
int main( int argc, char** argv, char** env
){
	char* algoForOrder       = strdup( "avl");
	char* graphMode          = "";
	char*  inputDataFileName = NULL;
	char* outputDataFileBase = NULL;
	bool withReverseOrder    = false;

	static const char* GNUPLOT_DATA_EXT=".gpdat";

	int opt;
	while( (opt = getopt( argc, argv, "a:f:g:o:cdr")) != -1)
	{
	    switch( opt)
	    {
	        case 'c':
	            clearScreen();
	            break;
	        case 'd':
	            g_isDebugModeOn = true;
	            printf( "debug mode is on\n");
	            break;
	        case 'a':
	            algoForOrder = strdup( optarg);
	            /*if( g_isDebugModeOn)*/ printf( "order algorythm: %s\n", optarg);
	            break;
	        case 'f':
	            inputDataFileName = strdup( optarg);
	            /*if( g_isDebugModeOn)*/ printf( "input file: %s\n", optarg);
	            break;
	        case 'g':
	            graphMode = strdup( optarg);
	            /*if( g_isDebugModeOn)*/ printf( "graph mode: %s\n", optarg);
	            break;
	        case 'o':
	            outputDataFileBase = strdup( optarg);
	            /*if( g_isDebugModeOn)*/ printf( "output file: %s\n", optarg);
	            break;
	        case 'r':
	            withReverseOrder = true;
	            /*if( g_isDebugModeOn)*/ printf( "reverse order\n");
	            break;
	        default:
	            printf( "unkown option: '-%d'\n", opt);
	            break;
	    }
	}

	if( inputDataFileName == NULL)
	{
	    printf( "missing input file\n");
	    exit(1);
	}
	if( outputDataFileBase == NULL)
	{
	    printf( "missing output file base name\n");
	    exit(1);
	}

	// =============================================

	if( g_isDebugModeOn) printf( "Process sorted data !!!\n");

	FILE* inputDataFile = fopen( inputDataFileName, "r");
	if( inputDataFile == NULL
	){
	    printf( "La meteo est mauvaise aujourd'hui, revenez demain. (%s)", inputDataFileName);
	    waitUntilKeyPressed( .silentMode = 1);
	    exit(1);
	}

	char* line = calloc( 256, sizeof( char));
	int lineRead = 0;
	int balance = 0;
	while( fgets( line, 256, inputDataFile))
	{
//	    if( ++lineRead == 1) { continue; }
      if( strstr( line, ";;") != NULL) { continue; }
      if( strlen( line) > 1 && line[ strlen( line) - 2] == ';') { continue; }
	    abrInsert(
	        /* comparisonNodes      */  &gComparisonNodes
	        /* data2Store           */,  line
	        /* graphMode            */,  graphMode
	        /* level                */,  0
	        /* balanceChange        */, &balance
	        /* withAvlSelfBalancing */,  0 == strcmp( "avl", algoForOrder)
	        );
	}
	freePtr( line);
	fclose( inputDataFile);

	int deepLeft  = 0;
	int deepRight = 0;

    
  
    
  
  // BIG HACK : would be better to do rotation in AVL ... but very complex
  // this solution below could take a lot of time as it build tree twice
  if( strcmp( graphMode, "m") == 0
  ){
      char secondaryGraphMode[256];
      strcpy( secondaryGraphMode, graphMode);
      strcat( secondaryGraphMode, "Secondary");

      FILE* intermediateDataOutputFile = fopen( "./generated/tempIntermediate.cdata", "w");
	    if( intermediateDataOutputFile == NULL
	    ){
	        printf( "La meteo est très mauvaise aujourd'hui, revenez demain. (./generated/tempIntermediate.cdata)");
	        if( g_isDebugModeOn) waitUntilKeyPressed( .silentMode = 1);
	        exit(1);
	    }
	    abrPrintOut( gComparisonNodes, intermediateDataOutputFile, false, 0, false, &deepLeft, &deepRight);
	    fclose( intermediateDataOutputFile);

	    inputDataFile = fopen( "./generated/tempIntermediate.cdata", "r");
	    if( inputDataFile == NULL
	    ){
	        printf( "La meteo est mauvaise aujourd'hui, revenez demain. (%s)", inputDataFileName);
	        waitUntilKeyPressed( .silentMode = 1);
	        exit(1);
	    }

	    line = calloc( 256, sizeof( char));
	    lineRead = 0;
	    balance = 0;
	    while( fgets( line, 256, inputDataFile))
	    {
          if( strstr( line, ";;") != NULL) { continue; }
    	    abrInsert(
	        /* comparisonNodes      */  &gComparisonNodes2
	        /* data2Store           */,  line
	        /* graphMode            */,  secondaryGraphMode
	        /* level                */,  0
	        /* balanceChange        */, &balance
	        /* withAvlSelfBalancing */,  0 == strcmp( "avl", algoForOrder)
	        );
	    }
	    freePtr( line);
	    fclose( intermediateDataOutputFile);
      gComparisonNodes = gComparisonNodes2;
  }
  // BIG HACK : would be better to do rotation in AVL ... but very complex






  char sortedDataOutputFilename[256] = "";
	strcpy( sortedDataOutputFilename, outputDataFileBase);
	strcat( sortedDataOutputFilename, GNUPLOT_DATA_EXT);
	FILE* sortedDataOutputFile = fopen( sortedDataOutputFilename, "w");
	if( sortedDataOutputFile == NULL
	){
	    printf( "La meteo est très mauvaise aujourd'hui, revenez demain. (%s)", sortedDataOutputFilename);
	    if( g_isDebugModeOn) waitUntilKeyPressed( .silentMode = 1);
	    exit(1);
	}

	deepLeft  = 0;
	deepRight = 0;
	// NOTE: memory is freed while printing gComparisonNodes
	abrPrintOut( gComparisonNodes, sortedDataOutputFile, withReverseOrder, 0, false, &deepLeft, &deepRight);
	if( g_isDebugModeOn) printf("\n\ndeepLeft: %d <?> %d :deepRight\n", deepLeft, deepRight);


	fclose( sortedDataOutputFile);

	freePtr( algoForOrder);
	freePtr( inputDataFileName);
	freePtr( outputDataFileBase);

// ===============================================================
// TESTS
// ===============================================================
	if( g_isDebugModeOn) waitUntilKeyPressed();
// ===============================================================

	cleanExit();
}


// ===============================================================


