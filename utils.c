// ===============================================================
// UTILS_C
// ===============================================================
#include "headers/utils.h"


// ===============================================================
// FUNCTIONS
// * anyGetch
// * anyScanf
// * asciiTab
// * cleanExit - defined at main level
// * clearScreen
// * freePtr
// * generateIntId
// * isNumber
// * ltrim
// * rtrim
// *  trim
// * purgeBuffer
// * qsort - included in stdlib.h - sort memory consecutive elements
// * qsortCompInt
// * qsortCompStruct
// * swapInt
// * toLowerCase
// * toUpperCase
// * varEnv
// * verifyPassword
// * vigenereCrypt
// * vigenereDecrypt
// * waitUntilKeyPressed
// ===============================================================
int anyGetch()
{
	#if defined(__linux__) || defined(__CYGWIN__)
	    system( "/bin/stty raw");
	    int key;
	    system( "/bin/stty -echo");
	    key = getc( stdin);
	    system( "/bin/stty echo");
	    system( "/bin/stty cooked");
	#else
/*	    time_t startTime = time( NULL);
	    time_t  currTime = time( NULL);
	    while( !kbhit()) // CTRL-C is not working
	    {
	        if( difftime( currTime, startTime) >= 60.0 // 60 seconds wait
	        ){
	            return 3; // CTRL-C
	        }
	        currTime = time( NULL);
	    } */
	    int key = _getch();
	#endif

	return key;
}

// Fonction pour échanger deux nombres
void swap(char *x, char *y) {
	char t = *x; *x = *y; *y = t;
}

// Fonction pour inverser le contenu du buffer
char* reverse(char *buffer, int i, int j)
{
	while (i < j) {
	    swap(&buffer[i++], &buffer[j--]);
	}
	
	return buffer;
}

// Fonction itérative pour implémenter la fonction itoa en C
char* itoa(int value, char* buffer, int base)
{
	// entrée invalide
	if (base < 2 || base > 32) {
	    return buffer;
	}

	// considère la valeur absolue du nombre
	int n = abs(value);

	int i = 0;
	while (n)
	{
	    int r = n % base;

	    if (r >= 10) {
	        buffer[i++] = 65 + (r - 10);
	    }
	    else {
	        buffer[i++] = 48 + r;
	    }

	    n = n / base;
	}

	// si le nombre est 0
	if (i == 0) {
	    buffer[i++] = '0';
	}

	// Si la base est 10 et la valeur est négative, la string résultante
	// est précédé d'un signe moins (-)
	// Avec toute autre base, la valeur est toujours considérée comme non signée
	if (value < 0 && base == 10) {
	    buffer[i++] = '-';
	}

	buffer[i] = '\0'; // string de fin nulle

	// inverse la string et la renvoie
	return reverse(buffer, 0, i - 1);
}

// ===============================================================
int anyScanf( int strMaxLen, char* strOutput, ... // call must be terminated by a NULL item
){
    va_list              ptrStdArgItems;
    va_start(            ptrStdArgItems, strOutput); // pointer start just after strOutput
	char  escapeChar   = ptrStdArgItems == NULL ? '\n'  : (char)  va_arg( ptrStdArgItems, int);
	char* defaultValue = ptrStdArgItems == NULL ? NULL  : (char*) va_arg( ptrStdArgItems, char*);
	char* message      = ptrStdArgItems == NULL ? NULL  : (char*) va_arg( ptrStdArgItems, char*);
	bool  pwdSecure    = ptrStdArgItems == NULL ? false : (bool)  va_arg( ptrStdArgItems, int);
    va_end(              ptrStdArgItems); // free origin pointer

	escapeChar = escapeChar == '\n' ? 13 : escapeChar;

	char*   scanPrompt = calloc( 3, sizeof( char));
	assert( scanPrompt != NULL);
	strcpy( scanPrompt, "> ");

	if( NULL != message
	 && strlen( message) > 0
	){
	    scanPrompt = realloc( scanPrompt, strlen( message) + 1);
	    assert( scanPrompt != NULL);
	    strcpy( scanPrompt, message);
	    if( NULL != defaultValue
	     && strlen( defaultValue) > 0
	    ){
	        scanPrompt = realloc( scanPrompt, strlen( message) + strlen( defaultValue) + 1 + 4); // +4 for parenthesis " (defaultValue)"
	        assert( scanPrompt != NULL);
	        scanPrompt[ strcspn( scanPrompt, ":")] = 0;
	        strcat( scanPrompt, " (");
	        strcat( scanPrompt, defaultValue);
	        strcat( scanPrompt, "):");
	    }
	    strcat( scanPrompt, " ");
	}

	int counterLen = strlen( itoa( strMaxLen, strOutput, 10))
	               + 3 // 3 more spaces more for parenthesis " (strMaxLen)"
	               + 4 // 4 more spaces more for parenthesis and escapeChar " (.)"
	               ;

	strcpy( strOutput, "");
	int   strOutputLen = 0;
	int   scanGetChar;
	// need to alloc once to be reallocated in loop below
	char*   strSecureOutput = calloc( 1, sizeof( char));
	assert( strSecureOutput != NULL );

	do
	{
	    strSecureOutput = realloc( strSecureOutput, strOutputLen + 1);
	    assert( strSecureOutput != NULL);
	    strcpy( strSecureOutput, strOutput);
	    if( strOutputLen > 0
	     && pwdSecure
	    ){
	        memset( strSecureOutput, '*', strOutputLen);
	    }

	    if( NULL == defaultValue
	     || strlen( defaultValue) == 0
	    ){
	        // first rewritting to display counter of characters left
	        //                             what is escape character
	        //                         and a final space for carriage return
	        //
	        printf( "\r%s%s", scanPrompt, strSecureOutput);
	        // counter of characters left
	        printf( " (%d)", (int)( strMaxLen - strOutputLen));
	        // escape character
	        if( escapeChar > 32
	        ){
	            printf( " (%c)", escapeChar);
	        }
	        // final space for carriage return
	        printf( " ");
	    }
	    // second rewritting to put cursor just a end of entered characters and before counter
	    printf( "\r%s%s", scanPrompt, strSecureOutput);

	    // read stdin character
	    // and start checks with CTRL-C
	    if(( scanGetChar = anyGetch()) == 3
	    ){
	        printf( "\r%*s", (int)( strlen( scanPrompt) + strOutputLen + counterLen), " ");
	        if( strOutputLen == 0
	        ){
	            return -1;
	        } else
	        {
	            strcpy( strOutput, "");
	        }
	    } else
	    if( scanGetChar == 8 // carriage return
	    ){
	        if( strOutputLen > 0
	        ){
	            strOutput[ strOutputLen - 1] = 0;
	        }
	    } else
	    if( scanGetChar == 13 // new line
	     && escapeChar  != 13 // not escapeChar
	    ){
	        strOutput[ strOutputLen + 1] = 0;
	        strOutput[ strOutputLen] = '\\'; // for good use of carriage return, avoid to add real new line - replaced with '\'
	    } else
	    if( scanGetChar != escapeChar
	     && scanGetChar >= 32 // alphanumeric
	    ){
	        strOutput[ strOutputLen + 1] = 0;
	        strOutput[ strOutputLen] = scanGetChar;
	        if( pwdSecure
	        ){
	            printf( "*");
	        } else
	        {
	            printf( "%c", scanGetChar);
	        }
	    }
 	    strOutputLen = strlen( strOutput);
	}
	while( scanGetChar != escapeChar
	    && strOutputLen < strMaxLen // not max length reached
	);

	// set default value if nothing entered
	if( strlen( strOutput) == 0
	 && NULL != defaultValue
	 && strlen( defaultValue) > 0
	){
	    strcpy( strOutput, defaultValue);
	}

	// final rewritting to remove default value & hide counter
	strcpy( strSecureOutput, strOutput);
	if( strOutputLen > 0
	 && pwdSecure
	){
	    memset( strSecureOutput, '*', strOutputLen);
	}

	printf( "\r%s %s %*s",( message != NULL && strlen( message) > 0) ? message : ">"
	                     , strSecureOutput
	                     , counterLen, " "
	                     );

	// free pointers
	freePtr( scanPrompt);
	freePtr( strSecureOutput);

	// put real new lines
	if( escapeChar != 13 // new line is not escapeChar
	){
	    int newLinePosition;
	    while( strOutputLen >( newLinePosition = strcspn( strOutput, "\\"))
	    ){
	        strOutput[ newLinePosition] = '\n';
	    }
	}

	return strlen( strOutput);
}


// ===============================================================
void asciiTab()
{
	printf( "\nASCII TABLE\n-----------");
	for( int compteur = 32; compteur <= 256; compteur++
	){
	    printf( "\n%c - %d - %x", compteur, compteur, compteur);
	}
}


// ===============================================================
void clearScreen()
{
#if !defined(__linux__) && !defined(__CYGWIN__)
	system("cls");
#else
	system("clear");
#endif
}


// ===============================================================
void freePtr( void* pointer
){
	if( pointer && pointer != NULL
	){
	    free( pointer);
	    pointer = NULL;
	}
}


// ===============================================================
int generateIntId( int mustDifferFrom)
{
#ifndef SRAND_INIT
#define SRAND_INIT
	srand( time( NULL));   // Initialization, should only be called once.
#endif
	int intGen;
	while( 10000 >( intGen = rand()) // generate a pseudo-random integer between 10000 and RAND_MAX
	    ||( mustDifferFrom != 0
		 && mustDifferFrom == intGen
	      ));
	return intGen;
}


// ===============================================================
bool isNumber( char* strNumber
){
	for( int letterId = 0; strNumber[ letterId] != '\0'; letterId++
	){
	    if( isdigit( strNumber[ letterId]) == 0
	    ){
	        return false;
	    }
	}
	return true;
}


// ===============================================================
char* ltrim( char* str)
{
    while( isspace( *str)) str++;
    return str;
}

char* rtrim( char* str)
{
    char* back = str + strlen( str);
    while( isspace( *--back));
    *( back + 1) = '\0';
    return str;
}

char* trim( char* str)
{
    return rtrim( ltrim( str));
}


// ===============================================================
void purgeBuffer()
{
	int bufferChar;
	while(( bufferChar = getchar()) != EOF && bufferChar != '\n');
}


// ===============================================================
// qsort( &( tab[0]), tabSize, sizeof( tabItem), qsortCompStruct);
// ===============================================================
int qsortCompInt( const void* item1, const void* item2
){
	// cast input void* values
    int* pItem1  = (int*)item1;
    int* pItem2  = (int*)item2;
    // return comparison result
    return *pItem1 - *pItem2;
}
// ===============================================================
struct Exemple
{
	char toCompare[50];
};
int qsortCompStruct( const void* item1, const void* item2
){
	// cast input void* values
    struct Exemple* pItem1  = (struct Exemple*)item1;
    struct Exemple* pItem2  = (struct Exemple*)item2;
    // return comparison result
    return strcmp( pItem1->toCompare, pItem2->toCompare);
}


// ===============================================================
void swapPtr( int* a, int* b
){
//	*a ^= *b ^= *a ^= *b;
	*a = *a ^ *b;
	*b = *a ^ *b;
	*a = *a ^ *b;
}


// ===============================================================
char* toLowerCase( const char* src
){
	char*   dest  = calloc( 1, strlen( src) + 1);
	assert( dest != NULL);
    char* result  = dest;
    while( 0  !=(  *dest++ = tolower( *src++)));
    return result;
}
char* toUpperCase( const char* src
){
	char*   dest  = calloc( 1, strlen( src) + 1);
	assert( dest != NULL);
    char* result  = dest;
    while( 0  !=(  *dest++ = toupper( *src++)));
    return result;
}


// ===============================================================
void varEnv( char** env
){
	for( int itemNum = 0; env[itemNum] != NULL; itemNum++
	){
	    printf( "\n%s", env[itemNum]);
	}
}


// ===============================================================
bool verifyPassword( char* pwd)
{
	size_t length = strlen( pwd);

	if( length < PWD_CRITERIA_LENGTH
	){
	    return false;
	}

	size_t  upper_count = 0;
	size_t  lower_count = 0;
	size_t  digit_count = 0;
	size_t symbol_count = 0;

	for( int i = 0; i < length; i++
	){
	    if(      isupper( pwd[ i])) {  upper_count++; }
	    else if( islower( pwd[ i])) {  lower_count++; }
	    else if( isdigit( pwd[ i])) {  digit_count++; }
	    else if( ispunct( pwd[ i])) { symbol_count++; }
	    if(  upper_count >= PWD_CRITERIA_UPPER
	     &&  lower_count >= PWD_CRITERIA_LOWER
	     &&  digit_count >= PWD_CRITERIA_DIGIT
	     && symbol_count >= PWD_CRITERIA_SYMBOL
	    ){
	        return true;
	    }
	}
	return false;
}


// ===============================================================
void vigenereCrypt( char* phrase, char* key
){
	char* cypherKey =( key != NULL ? key : "CYTECHLIBRARY");

	for( int iChar = 0; iChar < strlen( phrase); iChar++
	){
		phrase[ iChar] = (( phrase[ iChar] - 32                                           // alphabet from ASCII 32 "space"
	                                       + cypherKey[ (int) iChar % strlen( cypherKey)] // switch to alphabet starting at key letter (0 to 12)
	                      ) - 32                   // minimal 32 "space" char
	                        +( 122 + 1 - 32)       // avoid negative value => push to next range (reduced by modulo below)
	                     ) % ( 122 + 1 - 32) + 32; // loop between 32 to 122
	}
}

void vigenereDecrypt( char* cypheredPhrase, char* key
){
	char* cypherKey =( key != NULL ? key : "CYTECHLIBRARY");

	for( int iChar = 0; iChar < strlen( cypheredPhrase); iChar++
	){
		cypheredPhrase[ iChar] =(( cypheredPhrase[ iChar] - cypherKey[ (int) iChar % strlen( cypherKey)] // alphabet from key letter (0 to 12)
	                                                      + 32                                           // back to alphabet from ASCII 32 "space"
	                             ) - 32                   // minimal 32 "space" char
	                               +( 122 + 1 - 32)       // avoid negative value => push to next range (reduced by modulo below)
	                            ) % ( 122 + 1 - 32) + 32; // loop between 32 to 122
	}
}


// ===============================================================
/* use parentheses to avoid macro subst */
void (waitUntilKeyPressed)( struct wukpOption option
){
	if( option.silentMode == 0
	){
	    if( strlen( option.message) > 0
	    ){
	        printf( "%s", option.message);
	    } else
	    {
	        printf( "\ncontinue...");
	    }
	}
	if( anyGetch() == 3 // 3 === CTRL-C
	 && option.silentMode != 2
	){
	    cleanExit();
	};
}


// ===============================================================


