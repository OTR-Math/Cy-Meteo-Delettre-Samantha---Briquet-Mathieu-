// ===============================================================
// UTILS_H
// ===============================================================
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#if !defined(__linux__) && !defined(__CYGWIN__)
#include <conio.h>
#endif

#include <assert.h>
#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>


// ===============================================================
// TYPE_DEF
// ===============================================================
typedef enum
{
	low = 0,
	medium,
	high,
	strong,
}
PwdStengh;

// usage: void myFunction( funcVoid1Int f) { ... f( 1) ... }
typedef void (*funcVoid1Int)( int);
typedef int  (*funcInt2Int)( int, int);

// ===============================================================
// CONSTANTS
__attribute__((unused)) static const char* PATH_RESS = "ressources/";
__attribute__((unused)) static const char* PATH_DB   = "ressources/database/";
__attribute__((unused)) static const char* PATH_HEAD = "headers/";
__attribute__((unused)) static const char* PATH_SRC  = "src/";


// ===============================================================
// GLOBAL VARIABLES
extern bool g_isDebugModeOn;



// ===============================================================
// FUNCTIONS
// * any_getch
// * anyScanf
// * asciiTab
// * cleanExit - defined at main level
// * clearScreen
// * freePerso
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
/*! \brief anyGetch - instantly capture of keypressed
 *
 * MultiOS version of getch
 * Allow to capture just pressed key without waiting for <RETURN>
 * Usage on "...continue" waiting for key pressed
 * or on dynamic display according to each character entered in a filter field
 */
int  anyGetch();


/*! \brief anyScanf - rework of scanf and fgets on stdin
 *
 * With usage of anyGetch above
 * Manage length limit
 * Manage interruption CTRL-C to clean or exit
 * call it with array definition: anyScanf( strMaxLen
 *                                        , (char*[]){ strOutput
 *                                                   , escapeChar (as String)
 *                                                   , defaultValue
 *                                                   , message}
 *                                        , argsLen)
 * return -1 in case of CTRL-C
 * else return lenght of captured string
 */
int anyScanf( int strMaxLen, char* strOutput, ...); // must be terminated by a NULL item


/*! \brief asciiTab - give ASCII char, id, hexa values (>=32)
 */
void asciiTab();


/*! \brief cleanExit - global exit function /!\ defined at main level /!\
 *
 * Needed when multiple scanf are used, fulled stdin buffer may make scanf to not wait for user entering words
 */
void cleanExit();


/*! \brief clearScreen - depending of os
 */
void clearScreen();


/*! \brief freePtr - all steps to free memory safe
 */
void freePtr( void* pointer);


/*! \brief generatIntId - generate random integer
 */
int generateIntId( int);


/*! \brief isNumber - is string with all digit characters
 */
bool isNumber( char*);


/*! \brief trim, ltrim, rtrim - remove spaces at extremities of string
 */
char* ltrim( char* str);
char* rtrim( char* str);
char* trim(  char* str);


/*! \brief purgeBuffer - free content of stdin buffer
 *
 * Needed when multiple scanf are used, fulled stdin buffer may make scanf to not wait for user entering words
 */
void  purgeBuffer();


/*!
 * qsort( &( tab[0]), tabSize, sizeof( tabItem), qsortCompStruct);
 */
int qsortCompInt(    const void*, const void*);
int qsortCompStruct( const void*, const void*);


/*! \brief swapInt -
 */
void swapInt( int* a, int* b);


/*! \brief toLowerCase, toUpperCase -
 */
char* toLowerCase( const char*);
char* toUpperCase( const char*);


/*! \brief toLowerCase, toUpperCase -
 */
void varEnv( char**);


/*! \brief verifyPassword - check if password is strong or not
 *
 * Password has to pass all CRITERIA defined below
 */
bool verifyPassword( char*);
static const int PWD_CRITERIA_LENGTH = 8;
static const int PWD_CRITERIA_UPPER  = 1;
static const int PWD_CRITERIA_LOWER  = 1;
static const int PWD_CRITERIA_DIGIT  = 1;
static const int PWD_CRITERIA_SYMBOL = 1;


/*! \brief vigenere(De)Crypt - (de)cyphering string
 *
 * These crypting algorythms replace standard ASCII alphabet
 * using alphabets starting with characters of a password
 * replacing 1rst character of a string                   ( relative to ASCII space '32' )
 *                by associated character of new alphabet ( relative to 1rst character of password )
 * 2d (ASCII-letter 45) => 45th (ALPHA-2d)
 * 3rd etc...
 */
void  vigenereCrypt(   char*, char*);
void  vigenereDecrypt( char*, char*);


/*! \brief waitUntilKeyPressed( .silentMode = 1 ) - wait Until Key Pressed
 */
struct wukpOption { int silentMode; char* message; };
#define waitUntilKeyPressed(...) waitUntilKeyPressed( (struct wukpOption) { .silentMode = 0, .message = "", __VA_ARGS__ })
/* use parentheses to avoid macro subst */
void (waitUntilKeyPressed)( struct wukpOption option);


// ===============================================================



#endif // UTILS_H_INCLUDED
