
//
// Implementation of a HashTable that stores void *
//
#include "HashTableVoid.h"
#include <stdio.h>

// Obtain the hash code of a key
int HashTableVoid::hash(const char * key)
{
  // Add implementation here
  int h = 0;
  const char * p = key;
  while (*p) {
    h += *p;
    p++;
  }
  return h % TableSize;
}

// Constructor for hash table. Initializes hash table
HashTableVoid::HashTableVoid()
{
  // Add implementation here
  _buckets =  (HashTableVoidEntry **) malloc(TableSize*sizeof(HashTableVoidEntry*));
  for ( int i = 0; i < TableSize; i++) {
    _buckets[i] = NULL;
  }
}

// Add a record to the hash table. Returns true if key already exists.
// Substitute content if key already exists.
bool HashTableVoid::insertItem( const char * key, void * data)
{
  // Add implementation here
  int h = hash(key);
  HashTableVoidEntry * e = _buckets[h];

  while(e != NULL) {
    if (!strcmp(e->_key, key)) {
      e->_data = data;
      return true;
    }
    e = e->_next;
  }

  e = new HashTableVoidEntry;
  e->_key = strdup(key);
  e->_data = data;
  _buckets[h] = e;
  return false;
}

// Find a key in the dictionary and place in "data" the corresponding record
// Returns false if key is does not exist
bool HashTableVoid::find( const char * key, void ** data)
{
  // Add implementation here
  int h = hash(key);
  HashTableVoidEntry * e = _buckets[h];
  while (e!=NULL) {
    if (!strcmp(e->_key, key)) {
    // Entry found
    *data = e->_data;
    return true;
    }
    e = e->_next;
  }
  return false;
}

// Removes an element in the hash table. Return false if key does not exist.
bool HashTableVoid::removeElement(const char * key)
{
  // Add implementation here
  int h = hash(key);
  HashTableVoidEntry * e = _buckets[h];
  HashTableVoidEntry * prev = NULL;
  while (e!=NULL) {
    if (!strcmp(e->_key, key)) {
    // Entry found
      if (prev != NULL) {
        prev->_next = e->_next;
      } else {
        _buckets[h] = e->_next;
      }
      // free(e->_key);
      delete e;
      return true;
    }
    prev = e;
    e = e->_next;
  }
  return false;
}

// Creates an iterator object for this hash table
HashTableVoidIterator::HashTableVoidIterator(HashTableVoid * hashTable)
{
  // Add implementation here
  _currentBucket = 0;
  _hashTable = hashTable;
  _currentEntry = hashTable->_buckets[_currentBucket];
}

// Returns true if there is a next element. Stores data value in data.
bool HashTableVoidIterator::next(const char * & key, void * & data)
{
  // printf("----------------------\n");

  while (_currentEntry == NULL) {
    if (_currentBucket < _hashTable->TableSize) {
      _currentBucket++;
      _currentEntry = _hashTable->_buckets[_currentBucket];
    }
    // _currentEntry = _currentEntry->_next;
    // if(_currentEntry->_next != NULL)
    //   break;
  }
  if (_currentEntry != NULL) {
    if (_currentBucket == _hashTable->TableSize)
      return false;
    key = _currentEntry->_key;
    data = _currentEntry->_data;
    if (_currentEntry->_next != NULL) {
      _currentEntry = _currentEntry->_next;
      return true;
    }
    if (_currentBucket < _hashTable->TableSize) {
      _currentBucket++;
      _currentEntry = _hashTable->_buckets[_currentBucket];
    }
    return true;
  }
  
  
  return false;
}

