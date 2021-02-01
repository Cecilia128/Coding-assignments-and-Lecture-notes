/*
 * File: buffer.cpp
 * --------------------------------------------------
 * This file implements the EditorBuffer class using a
 * doubly linked list to represent the buffer.
 */

#include <iostream>
#include "buffer.h"

/*
 * Implementation notes: EditorBuffer constructor
 * ----------------------------------------------
 * This function initializes an empty editor buffer, represented
 * as a doubly linked list.
 */

EditorBuffer::EditorBuffer()
{
   start = cursor = new Cell;
   start->next = start;
   start->prev = start;
}

/*
 * Implementation notes: EditorBuffer destructor
 * ---------------------------------------------
 * The destructor must delete every cell in the buffer.  Note
 * that the loop structure is not exactly the standard idiom for
 * processing every cell within a linked list, because it is not
 * legal to delete a cell and later look at its next field.
 */

EditorBuffer::~EditorBuffer()
{
    Cell* cp = start->next;
    while (cp != start) {
        delete cp->prev;
        cp = cp->next;
    }
    delete cp->prev;
}

/*
 * Implementation notes: cursor movement
 * -------------------------------------
 * In a doubly linked list, each of these operations runs in
 * constant time.
 */

void EditorBuffer::moveCursorForward()
{
   if (cursor->next != start)
   {
      cursor = cursor->next;
   }
}

void EditorBuffer::moveCursorBackward()
{
   if (cursor != start)
   {
      cursor = cursor->prev;
   }
}

void EditorBuffer::moveCursorToStart()
{
   cursor = start;
}

void EditorBuffer::moveCursorToEnd()
{
   cursor = start->prev;
}

/*
 * Implementation notes: insertCharacter, deleteCharacter
 * ------------------------------------------------------
 * This code is much like that used for the traditional linked
 * list except that more pointers need to be updated.
 */

void EditorBuffer::insertCharacter(char ch)
{
    Cell* cp = new Cell;
    cp->ch = ch;
    cp->next = cursor->next;
    cp->prev = cursor;
    cursor->next->prev = cp;
    cursor->next = cp;
    cursor = cp;
}

void EditorBuffer::deleteCharacter()
{
    if (cursor->next != start) {
        Cell* old = cursor->next;
        cursor->next = old->next;
        old->next->prev = cursor;
        delete old;
    }
}

/*
 * Implementation notes: getText and getCursor
 * -------------------------------------------
 * The getText method uses the standard linked-list pattern to loop
 * through the cells in the linked list.  The getCursor method counts
 * the characters in the list until it reaches the cursor.
 */

std::string EditorBuffer::getText() const
{
    //loop through the cells in the linked list and append char in each cell to text
    Cell* cp = start;
    std::string text;
    while (cp->next != start) {
        text.append(&cp->ch);
        cp = cp->next;
    }
    text.append(&cp->ch);
    return text;
}

int EditorBuffer::getCursor() const
{
    // loop through the cells in the linked list and count the number of cell
    int count = 0;
    Cell* cp = start;
    while (cp != cursor) {
        count ++;
        cp = cp->next;
    }
    return count;
}


/*
 * Function: printHelpText
 * Usage: printHelpText();
 * -----------------------
 * Displays a message showing the legal commands.
 */

void printHelpText()
{
   std::cout << "Editor commands:" << std::endl;
   std::cout << "  Iabc  Inserts the characters abc at the cursor position" << std::endl;
   std::cout << "  F     Moves the cursor forward one character" << std::endl;
   std::cout << "  B     Moves the cursor backward one character" << std::endl;
   std::cout << "  D     Deletes the character after the cursor" << std::endl;
   std::cout << "  J     Jumps to the beginning of the buffer" << std::endl;
   std::cout << "  E     Jumps to the end of the buffer" << std::endl;
   std::cout << "  H     Prints this message" << std::endl;
   std::cout << "  Q     Exits from the editor program" << std::endl;
}

/*
 * Function: displayBuffer
 * Usage: displayBuffer(buffer);
 * -----------------------------
 * Displays the state of the buffer including the position of the cursor.
 */

void displayBuffer(EditorBuffer & buffer)
{
   std::string str = buffer.getText();
   for (int i = 0; i<str.length(); i++)
   {
      std::cout << " " << str[i];
   }
   std::cout << std::endl;
   std::cout << std::string(2 * buffer.getCursor(), ' ') << "^" << std::endl;
}


/*
 * Function: executeCommand
 * Usage: executeCommand(buffer, line);
 * ------------------------------------
 * Executes the command specified by line on the editor buffer.
 */

void executeCommand(EditorBuffer & buffer, std::string line)
{
   switch (toupper(line[0]))
   {
    case 'I': for (int i=1; i<line.length(); i++)
              {
                 buffer.insertCharacter(line[i]);
              }
              displayBuffer(buffer);
              break;
    case 'D': buffer.deleteCharacter(); displayBuffer(buffer); break;
    case 'F': buffer.moveCursorForward(); displayBuffer(buffer); break;
    case 'B': buffer.moveCursorBackward(); displayBuffer(buffer); break;
    case 'J': buffer.moveCursorToStart(); displayBuffer(buffer); break;
    case 'E': buffer.moveCursorToEnd(); displayBuffer(buffer); break;
    case 'H': printHelpText(); break;
    case 'Q': std::exit(0);
    default:  std::cout << "Illegal command" << std::endl; break;
   }
}


