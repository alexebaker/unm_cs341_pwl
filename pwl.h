/*----------------------------------------------------*
 * Author: Alex Baker                                 *
 * UNM ID: 101372834                                  *
 * UNM Email: alexebaker@unm.edu                      *
 *----------------------------------------------------*
 * CS 341                                             *
 * Assignment 1                                       *
 * Due: 01/27/2016                                    *
 *----------------------------------------------------*
 * This is the header file for pwl.c which contains   *
 * all struct definitions and function definitions.   *
 *----------------------------------------------------*/


#ifndef PWL_H
#define PWL_H


#ifndef _BSD_SOURCE
#define _BSD_SOURCE
#endif


/* data struct similar to a linked list for holding stats for different paragraphs  */
struct ParStats
{
    int lines;
    int words;
    int chars;
    struct ParStats* nextPar;
};


/* struct for holding statistics for an entire file and its paragraphs */
struct FileStats
{
    char* path;
    int lines;
    int words;
    int chars;
    int totalPars;
    struct ParStats* pars;
};


/*------------------------------------------------------------------*
 * Reads statistics from a file with the given filename.            *
 * This will also print the statistics to stdout.                   *
 *                                                                  *
 * :param filename: a string with the name of the file to read from *
 *------------------------------------------------------------------*/
void readFile(char filename[]);


/*------------------------------------------------------------------*
 * Creates and initializes a new FileStats struct.                  *
 *                                                                  *
 * :return: a pointer to the newly created struct                   *
 *------------------------------------------------------------------*/
struct FileStats* createFile();


/*------------------------------------------------------------------*
 * Adds paragraph stats to an existing FileStats struct.            *
 *                                                                  *
 * :param stats: a pointer to FileStats struct to save the          *
 *               paragraph to.                                      *
 * :param lines: number of lines in the paragraph                   *
 * :param words: number of words in the paragraph                   *
 * :param chars: number of characters in the paragraph              *
 *------------------------------------------------------------------*/
void addPar(struct FileStats* stats, int lines, int words, int chars);


/*------------------------------------------------------------------*
 * Frees all of the memory allocated to a FileStats struct.         *
 *                                                                  *
 * :param stats: a pointer to the FileStats struct to free          *
 *------------------------------------------------------------------*/
void freeStats(struct FileStats* stats);


/*------------------------------------------------------------------*
 * Frees all of the memory allocated to a ParStats struct and any   *
 * linked paragraphs it may have.                                   *
 *                                                                  *
 * :param stats: a pointer to the head ParStats struct              *
 *------------------------------------------------------------------*/
void freePars(struct ParStats* pars);


/*------------------------------------------------------------------*
 * Prints and formats data in a FileStats struct to stdout.         *
 *                                                                  *
 * :param stats: a pointer to the FileStats struct to print         *
 *------------------------------------------------------------------*/
void printStats(struct FileStats* stats);


#endif
