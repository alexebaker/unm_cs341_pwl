/*----------------------------------------------------*
 * Author: Alex Baker                                 *
 * UNM ID: 101372834                                  *
 * UNM Email: alexebaker@unm.edu                      *
 *----------------------------------------------------*
 * CS 341                                             *
 * Assignment 1                                       *
 * Due: 01/27/2016                                    *
 *----------------------------------------------------*
 * This program reports statistics about a text file  *
 * by giving the number of paragraphs, lines, words,  *
 * and characters per paragraph.                      *
 *----------------------------------------------------*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "pwl.h"


int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("Usage: %s \"path/to/file\"\n", argv[0]);
        return 1;
    }

    readFile(argv[1]);
    return 0;
}


/*------------------------------------------------------------------*
 * Reads statistics from a file with the given filename.            *
 * This will also print the statistics to stdout.                   *
 *                                                                  *
 * :param filename: a string with the name of the file to read from *
 *------------------------------------------------------------------*/
void readFile(char filename[])
{
    char c;
    int numLines = 0;
    int numWords = 0;
    int numChars = 0;
    int numNLs = 0;

    int totalLines = 0;
    int totalWords = 0;
    int totalChars = 0;
    int totalPars = 0;

    char absPath[10000];

    bool inWord = false;
    bool inPar = false;

    struct FileStats* fileStats = createFile();

    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open file, check spelling and permissions.\n");
        freeStats(fileStats);
        return;
    }

    while ((c = fgetc(file)) != EOF)
    {
        totalChars++;
        numChars++;
        if (c == ' ' || c == '\t')
        {
            /* whitespace is not part of a word so set to false */
            inWord = false;
        }
        else if (c == '\n')
        {
            if (inPar)
            {
                /* only count new lines in a paragraph */
                numLines++;
            }
            totalLines++;

            /* this keeps track of consecutive newlines, so increment */
            numNLs++;

            /* newline is not part of a word so set to false */
            inWord = false;
        }
        else
        {
            if (inPar)
            {
                if (numNLs >= 2)
                {
                    /* hit more than 2 newline chars in a row,
                     * so we are no longer in a paragraph */
                    inPar = false;

                    /* save off current stats */
                    addPar(fileStats, numLines-numNLs+1, numWords, numChars-numNLs+1);
                    numLines = 0;
                    numWords = 0;
                    numChars = 0;
                }
            }
            else
            {
                /* previously not in a paragraph but now are
                 * since any none whitespace or newline char can start a
                 * new paragraph, so increase count */
                totalPars++;
                inPar = true;
            }
            /* hit a none newline char so set back to zero */
            numNLs = 0;

            if (!inWord)
            {
                /* if we were not in a word (i.e. last char was whitespace)
                 * we are now in a new word so increase the count */
                numWords++;
                totalWords++;
            }
            /* every character not whitespace or a newline is treated as part of a word,
             * so set to true */
            inWord = true;
        }
    }

    if (inPar)
    {
        /* save off current stats */
        addPar(fileStats, numLines-numNLs+1, numWords, numChars-numNLs+1);
        numLines = 0;
        numWords = 0;
        numChars = 0;
    }

    realpath(filename, absPath);
    fileStats->path = absPath;
    fileStats->lines = totalLines;
    fileStats->words = totalWords;
    fileStats->chars = totalChars;
    fileStats->totalPars = totalPars;

    printStats(fileStats);
    freeStats(fileStats);
    fclose(file);
    return;
}


/*------------------------------------------------------------------*
 * Creates and initializes a new FileStats struct.                  *
 *                                                                  *
 * :return: a pointer to the newly created struct                   *
 *------------------------------------------------------------------*/
struct FileStats* createFile()
{
    struct FileStats* newStats = malloc(sizeof(struct FileStats));

    newStats->lines = 0;
    newStats->words = 0;
    newStats->chars = 0;
    newStats->pars = NULL;
    return newStats;
}


/*------------------------------------------------------------------*
 * Adds paragraph stats to an existing FileStats struct.            *
 *                                                                  *
 * :param stats: a pointer to FileStats struct to save the          *
 *               paragraph to.                                      *
 * :param lines: number of lines in the paragraph                   *
 * :param words: number of words in the paragraph                   *
 * :param chars: number of characters in the paragraph              *
 *------------------------------------------------------------------*/
void addPar(struct FileStats* fileStats, int lines, int words, int chars)
{
    struct ParStats* newPar = malloc(sizeof(struct ParStats));
    newPar->lines = lines;
    newPar->words = words;
    newPar->chars = chars;
    newPar->nextPar = NULL;

    if (fileStats->pars == NULL)
    {
        fileStats->pars = newPar;
    }
    else
    {
        struct ParStats* currentPar = fileStats->pars;
        while (currentPar->nextPar != NULL)
        {
            currentPar = currentPar->nextPar;
        }
        currentPar->nextPar = newPar;
    }
    return;
}


/*------------------------------------------------------------------*
 * Frees all of the memory allocated to a FileStats struct.         *
 *                                                                  *
 * :param stats: a pointer to the FileStats struct to free          *
 *------------------------------------------------------------------*/
void freeStats(struct FileStats* stats)
{
    freePars(stats->pars);
    free(stats);
    return;
}


/*------------------------------------------------------------------*
 * Frees all of the memory allocated to a ParStats struct and any   *
 * linked paragraphs it may have.                                   *
 *                                                                  *
 * :param stats: a pointer to the head ParStats struct              *
 *------------------------------------------------------------------*/
void freePars(struct ParStats* pars)
{
    if (pars == NULL)
    {
        return;
    }

    freePars(pars->nextPar);
    free(pars);
    return;
}


/*------------------------------------------------------------------*
 * Prints and formats data in a FileStats struct to stdout.         *
 *                                                                  *
 * :param stats: a pointer to the FileStats struct to print         *
 *------------------------------------------------------------------*/
void printStats(struct FileStats* stats)
{
    struct ParStats* currentPar = NULL;
    int counter = 0;

    printf("File       : %s\n", stats->path);
    printf("Paragraphs : %d\n\n", stats->totalPars);

    currentPar = stats->pars;
    while (currentPar != NULL)
    {
        counter++;
        printf("Paragraph #%d: ", counter);
        printf("%d  %d  %d\n", currentPar->lines, currentPar->words, currentPar->chars);
        currentPar = currentPar->nextPar;
    }
    printf("\n");
    return;
}


