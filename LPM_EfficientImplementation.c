/* file : LPM_EfficientImplementation.c */
/* authors : Vrincianu Andrei - Darius (a.vrincianu@student.rug.nl) and Vitalii Sikorski (v.sikorski@student.rug.nl) */
/* date : March 11 2024 */
/* version: 1.0 */

/* Description:
  This program accepts n addresses and their respective numbers and stores them in a trie, after which it also accepts m address which are then routed to the longest
  matching prefix using the previously mentioned trie
*/

#include <stdlib.h>
#include <stdio.h>

typedef struct bitTrie {
  int isLeaf;
  //routingNumber is the number taken alongside the address in the input
  int routingNumber;
  struct bitTrie *childLeft, *childRight;
} bitTrie;

// Creates an empty trie
bitTrie* makeTrie (void) {
  bitTrie* node = (bitTrie*)malloc(sizeof(bitTrie));
  // Checking if memory allocation was successful
  if (node == NULL) {
    printf("malloc failure");
    exit(EXIT_FAILURE);
  }
  if (node) {
    node->childLeft = NULL;
    node->childRight = NULL;
    node->isLeaf = 0;
    node->routingNumber = -1;
  }
  return node;
}

// Releases the allocated memory
void freeTrie (bitTrie* node) {
  if (node == NULL) {
    return;
  }
  freeTrie(node->childLeft);
  freeTrie(node->childRight);
  free(node);
}

// Transforms the IP address into a bit array
void makeBitsInt(int p1, int p2, int p3, int p4, int* bitArray) {
  int x = 0;
  int binaryImp = 7;
  // Starting from the least significant bit, the bit value is the remainder of division to 2
  for (int j = binaryImp; j >= x; j--) {
    bitArray[j] = p1 % 2;
    p1 /= 2;
  }
  // Adjusting the indexes
  x += 8;
  binaryImp += 8;
  for (int j = binaryImp; j >= x; j--) {
    bitArray[j] = p2 % 2;
    p2 /= 2;
  }
  x += 8;
  binaryImp += 8;
  for (int j = binaryImp; j >= x; j--) {
    bitArray[j] = p3 % 2;
    p3 /= 2;
  }
  x += 8;
  binaryImp += 8;
  for (int j = binaryImp; j >= x; j--) {
    bitArray[j] = p4 % 2;
    p4 /= 2;
  }
}

// Inserts the first n-bits of an address into the trie, where n is the the mask of said address, and sets the value of the last bit to the number relevant to the address
// taken from the input
void insertInTrie(bitTrie *root, int* bitArray, int maskNumber, int routeNumber) {
  int level;
  int index;
  int mask = maskNumber;
  int counter = 0;
  bitTrie *newtrie = root;
  for (level = 0; level < mask; level++) {
    index = bitArray[level];
    if (index) {
      if (newtrie->childRight == NULL) {
        newtrie->childRight = makeTrie();
      }
      newtrie = newtrie->childRight;
    } else {
      if (newtrie->childLeft == NULL) {
        newtrie->childLeft = makeTrie();
      }
      newtrie = newtrie->childLeft;
    }
    
  }
  newtrie->routingNumber = routeNumber;
  newtrie->isLeaf = 1;
}

// This searches the trie for the best match for the given address by returning the number relevant to the address with the last mask encountered,
int searchInTrie(bitTrie *trie, int* bitArray) {
  bitTrie *newtrie = trie;
  int level = 0;
  int index;
  int lastMask = -1;
  //The number length is fixed as 32 due to the fact that an address has exactly 32 bits
  int length = 32;
  for (level = 0; level < length; level++) {
    index = bitArray[level];
    // Upon discovering a value different from -1, the fact that a relevant value has been discovered is flagged
    if (newtrie->routingNumber != -1) {
      lastMask = newtrie->routingNumber;
    }
    if (index) {
      newtrie = newtrie->childRight;
    } else {
      newtrie = newtrie->childLeft;
    }
    if (newtrie == NULL) {
      break;
    }
  }
  return lastMask;
}

int main(int argc, char* argv[]) {
  int n;
  scanf("%d", &n);
  int p1, p2, p3, p4, mask, routingNumber;
  int bitArray[32];
  // Creating the root of the bit trie
  bitTrie* root = makeTrie();
  while (n) {
    // Getting the user input
    scanf("%d.%d.%d.%d/%d %d", &p1, &p2, &p3, &p4, &mask, &routingNumber);
    // Transforming the input address into an array of bits
    makeBitsInt(p1, p2, p3, p4, bitArray);
    // Adding the array of bits to the trie
    insertInTrie(root, bitArray, mask, routingNumber);
    n--;
  }
  int m;
  scanf("%d", &m);
  while (m) {
    scanf("%d.%d.%d.%d", &p1, &p2, &p3, &p4);
    makeBitsInt(p1, p2, p3, p4, bitArray);
    // Outputting the interface index of the matching network
    printf("%d\n", searchInTrie(root, bitArray));
    m--;
  }
  // Freeing up the memory
  freeTrie(root);
  return 0;
}
