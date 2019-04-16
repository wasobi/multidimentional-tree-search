//
//  KDTree.h
//  CSC212_PAss2
//
//  Created by Sierra Obi on 3/30/16.
//  Copyright © 2016 Sierra Obi. All rights reserved.
//

#ifndef KDTree_h
#define KDTree_h
#include <string>

/* * * * * * * * * * * * * * * * * * * * * * *
 
 * * * * * * * * * * * * * * * * * * * * * * */
class KDNode {
private:
    double lat;
    double lon;
    std::string name;
    KDNode *left;
    KDNode *right;
    
    double distance(double _la, double _lo);
    
public:
    KDNode(double la, double lo, const std::string &desc);
    ~KDNode();
    
    friend class KDTree;
};


/* * * * * * * * * * * * * * * * * * * * * * *
 
 * * * * * * * * * * * * * * * * * * * * * * */
class KDTree {
private:
    unsigned int size, count;
    KDNode *root;
    
    void destroy(KDNode *p);
    
public:
    KDTree();
    ~KDTree();
    
    void insert(double la, double lo, const std::string &desc);
    unsigned int printNeighbors(double la, double lo, double rad, const std::string &filter);
    //void rangeQuery(KDNode *current, double rad, double la, double lo, const std::string &filter);
    void rangeQuery(KDNode *current, double rad, double la, double lo, const std::string &filter, int level);
    unsigned int getSize();
};



#endif /* KDTree_h */
