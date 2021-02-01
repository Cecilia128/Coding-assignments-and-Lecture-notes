/*
 * File: bigint.cpp
 * ----------------
 * This file implements the bigint.h interface.
 */

#include <cctype>
#include <string>
#include "bigint.h"
#include "error.h"
#include "strlib.h"

/*
 * Implementation notes: BigInt constructor
 * ----------------------------------------
 * Creates a new BigInt from an int or a string of decimal digits.
 */

BigInt::BigInt(std::string str)
{
    Cell* digit = new Cell;
    digit->Digit = charToInteger(str[str.size()-1]);
    digit->leadingDigits = NULL;
    start = digit;
    Cell* cp = start;
    for (int i=1; i<str.size(); i++) {
        Cell* digit = new Cell;
        digit->Digit = charToInteger(str[str.size()-i-1]);
        digit->leadingDigits = NULL;
        cp->leadingDigits = digit;
        cp = cp->leadingDigits;
    }

}

BigInt::BigInt(int k)
{
    int reminder;
    //use % to get the digit in each position
    reminder = k%10;
    k = k/10;
    Cell* digit = new Cell;
    digit->Digit = reminder;
    digit->leadingDigits = NULL;
    start = digit;
    Cell* cp = start;
    while (k>0) {
        reminder = k%10;
        k = k/10;
        Cell* digit = new Cell;
        digit->Digit = reminder;
        digit->leadingDigits = NULL;
        cp->leadingDigits = digit;
        cp = cp->leadingDigits;
    }

}

/*
 * Implementation notes: BigInt destructor
 * ---------------------------------------
 * Frees the memory used by a BigInt when it goes out of scope.
 */

BigInt::~BigInt()
{
    Cell* cp = start;
    Cell* next = cp->leadingDigits;
    while (next != NULL) {
        delete cp;
        cp = next;
        next = cp->leadingDigits;
    }
    delete cp;
}

/*
 * Implementation notes: toString
 * ------------------------------
 * Converts a BigInt object to the corresponding string.
 */

std::string BigInt::toString() const
{
    std::string str;
    Cell* cp = start;
    while (cp!=NULL) {
        //insert each digit to the beginning of str
        str.insert(0,integerToString(cp->Digit));
        cp = cp->leadingDigits;
    }
    return str;
}

BigInt BigInt::operator+(const BigInt & b2) const
{
    std::string str;
    int remainder;
    int round=0;
    int n1;
    int n2;
    Cell* cp1 = start;
    Cell* cp2 = b2.start;
    while (cp1 != NULL || cp2 != NULL) {
        // if the length of b2 is longer than this
        if (cp1 == NULL) {
            n1 = 0;
            n2 = cp2->Digit;
            cp2 = cp2->leadingDigits;
        }
        // if the length of b2 is longer than this
        else if (cp2 ==NULL) {
            n2 = 0;
            n1 = cp1->Digit;
            cp1 = cp1->leadingDigits;
        }
        else {
            n1 = cp1->Digit;
            n2 = cp2->Digit;
            cp1 = cp1->leadingDigits;
            cp2 = cp2->leadingDigits;
        }

        remainder = (n1+n2)%10;
        // if round+remainder >= 10, add 1 in higher position
        if (round+remainder < 10) {
            str.insert(0,integerToString(round+remainder));
            round = (n1+n2)/10;
        }
        else {
            str.insert(0,integerToString((round+remainder)%10));
            round = 1;
        }
    }
    if (round != 0) str.insert(0,integerToString(round));
    return BigInt(str);
}

BigInt BigInt::operator*(const BigInt & b2) const
{

    int remainder;
    int count=0;
    int n1;
    int n2;
    BigInt result("0");
    Cell* cp1 = start;
    // use each digit of this to times b2
    while (cp1 != NULL) {
        std::string str;
        int round=0;
        n1 = cp1->Digit;
        cp1 = cp1->leadingDigits;
        Cell* cp2 = b2.start;
        while (cp2 != NULL) {

            n2 = cp2->Digit;
            cp2 = cp2->leadingDigits;
            remainder = (n1*n2)%10;
            // if round+remainder >= 10, add one more in higher position
            if (round+remainder <10) {
                str.insert(0,integerToString(round+remainder));
                round = (n1*n2)/10;
            }
            else {
                str.insert(0,integerToString((round+remainder)%10));
                round = (n1*n2)/10 + 1;
            }

        }
        if (round != 0) str.insert(0,integerToString(round));
        // when the digit is in ith position in this, the result should times 10^(i-1)
        for (int i=0; i<count; i++) {
            str.append(integerToString(0));
        }
        count++;
        result = result + BigInt(str);

    }
    return result;
}

BigInt & BigInt::operator=(const BigInt & b2) {
    // free the memory used before and assign b2 to this
    if (this != & b2) {
        Cell* cp = start;
        Cell* next = cp->leadingDigits;
        while (next != NULL) {
            delete cp;
            cp = next;
            next = cp->leadingDigits;
        }
        delete cp;
        deepCopy(b2);
    }
    return *this;
}

void BigInt::deepCopy(const BigInt& b2){
    // assign b2 to this
    start = b2.start;

    Cell* cp2 = b2.start;
    Cell* digit = new Cell;
    digit->Digit = cp2->Digit;
    digit->leadingDigits = NULL;
    start = digit;
    Cell* cp1 = start;
    cp2 = cp2->leadingDigits;
    while (cp2 != NULL) {

        Cell* digit = new Cell;
        digit->Digit = cp2->Digit;
        digit->leadingDigits = NULL;
        cp1->leadingDigits = digit;
        cp1 = cp1->leadingDigits;
        cp2 = cp2->leadingDigits;
    }



}
BigInt fact(int n)
{
   BigInt result("1");
   for (int i = 1; i <= n; i++)
   {
      BigInt mul(i);

      result = result * mul;
   }
   return result;
}
