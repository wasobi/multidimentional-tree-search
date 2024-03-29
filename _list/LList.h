//
//  LList.h
//  CSC212_PAss2
//
//  Created by Sierra Obi, Zachary Taylor, and Justin Szarkowicz
//

#ifndef LList_h
#define LList_h
#include <string>


/* * * * * * * * * * * * * * * * * * * * * * *
 
 
 
 * * * * * * * * * * * * * * * * * * * * * * */
class LLNode {
private:
    double lat;        // latitude
    double lon;        // longitude
    std::string name;  // description
    LLNode *next;      // pointer to the next element
    
    // calculate the distance in miles between this object and the location
    // given by _la and _lo
    double distance(double _la, double _lo);
    
public:
    // constructor / destructor
    LLNode(double la, double lo, const std::string &desc);
    ~LLNode();
    
    friend class LinkedList;
};


/* * * * * * * * * * * * * * * * * * * * * * *
 
 
 
 * * * * * * * * * * * * * * * * * * * * * * */
class LinkedList {
private:
    unsigned int size;  // number of nodes in the list
    LLNode *head;       // pointer to the head of the list
    
    // method called by the destructor (deletes all modes)
    void destroy(LLNode *p);
    
public:
    // constructor / destructor
    LinkedList();
    ~LinkedList();
    
    // insert a new node at the beginning of the list
    void insert(double la, double lo, const std::string &desc);
    // print all the nodes under a distance 'rad' from 'la,lo' and where
    // filter is a non-empty substring of their description
    unsigned int printNeighbors(double la, double lo, double rad, const std::string &filter);
    // return the number of nodes in the list
    unsigned int getSize();
};

#endif /* LList_h */
