// ===============================================================
// SORT_NODES_H
// ===============================================================
#ifndef SORT_NODES_H_INCLUDED
#define SORT_NODES_H_INCLUDED

#include "utils.h"


// ===============================================================
// TYPE_DEF
// ===============================================================
typedef struct NodeTemplate // for recursive usage
{
	char*  comparisonValue;
	char*        nodevalue;
	char*      storedData;
	struct NodeTemplate*  leftNode;
	struct NodeTemplate* rightNode;
	int    balance;
}
TypNode;


// ===============================================================
// STRUCTS FUNCTIONS
// ===============================================================


// ===============================================================
// CONSTANTS
// ===============================================================
__attribute__((unused)) static const char* FILE_BOOKS        = "ressources/database/books.txt";
__attribute__((unused)) static const char* FILE_BOOKS_TEMP   = "ressources/database/booksTemp.txt";
__attribute__((unused)) static const char* FILE_AUTHORS      = "ressources/database/authors.txt";
__attribute__((unused)) static const char* FILE_AUTHORS_TEMP = "ressources/database/authorsTemp.txt";


// ===============================================================
// FUNCTIONS
// - abrInsert
// - avlLeftBalancing
// - avlRightBalancing
// - abrPrintOut
// ===============================================================
bool abrInsert( TypNode**, char*, char*, int, int*, bool);
void avlLeftBalancing(  TypNode* *comparisonNodes);
void avlRightBalancing( TypNode* *comparisonNodes);
bool abrPrintOut( TypNode*, FILE*, bool, int, bool, int*, int*);


#endif // SORTNODES_H_INCLUDED
