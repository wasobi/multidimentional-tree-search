//
//  KDTree.cpp
//  CSC212_PAss2
//
//  Created by Sierra Obi, Zachary Taylor, and Justin Szarkowicz
//
#include "KDTree.h"
#include <math.h>
#include <iostream>

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Initializes the data members for a given node.
 * * * * * * * * * * * * * * * * * * * * * * * * * */
KDNode::KDNode(double la, double lo, const std::string &desc) {
    left = NULL;
    right = NULL;
    name = desc;
    lat = la;
    lon = lo;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Default deconstructor for the KDNode class
 * * * * * * * * * * * * * * * * * * * * * * * * * */
KDNode::~KDNode() {
}


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Returns distance between given latitude/longitude
 and latitude/longitude of current node.
 * * * * * * * * * * * * * * * * * * * * * * * * * */
double KDNode::distance(double _la, double _lo) {
    double param = M_PI / 180.0; // required for conversion from degrees to radians
    double rad = 3956.0;  // radius of earth in miles
    double d_lat = (_la - lat) * param;
    double d_lon = (_lo - lon) * param;
    double dist = sin(d_lat/2) * sin(d_lat/2) + cos(lat*param) * cos(_la*param) * sin(d_lon/2) * sin(d_lon/2);
    dist = 2.0 * atan2(sqrt(dist), sqrt(1-dist));
    return rad * dist;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Initializes the size to 0 for a KDTree as well
 as the root to NULL. This defines an empty tree.
 A tree will have a size of 0 and a root pointer
 that points to NULL until insertions are made
 into the tree. The count is used to keep track of
 the total number of nodes that are within range
 of the center.
 * * * * * * * * * * * * * * * * * * * * * * * * * */
KDTree::KDTree() {
    size = 0;
    count = 0;
    root = NULL;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Deconstructor for the KDTree class
 * * * * * * * * * * * * * * * * * * * * * * * * * */
KDTree::~KDTree() {
    destroy(root);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Uses a postorder traversal to delete each node in
 a KDTree
 * * * * * * * * * * * * * * * * * * * * * * * * * */
void KDTree::destroy(KDNode *p) {
    if (p) {             //for p not equal to NULL
        destroy(p->left); //visit left child
        destroy(p->right); //visit right child
        delete p;         //delete node
    }
    
}

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Traverses through the tree until a leaf node
 is reached. The comparisons are made based on
 a 2D data structure where K = 2. In this case
 we are comparing the latitude and longitude
 for a given node. The even levels will compare
 latitude and the odd levels will compare the
 longitude. Insertions are made at a leaf.
 -------
 The double pointer current points to either the
 left or right pointer of a node.
 * * * * * * * * * * * * * * * * * * * * * * * * */
void KDTree::insert(double la, double lo, const std::string &desc) {
    
    // Creates a double pointer to keep track of the location that insertions need to be made
    KDNode **current = &root;
    
    // If current points to null (empty tree) create the root node
    if (*current == NULL){
        root = new KDNode(la,lo,desc);
        // Increase the size of the tree by 1
        size++;
        return;
    }
    
    // Initializes the level to zero when you start at the root
    int level = 0;
    
    // if the double pointer is not pointing to NULL then the tree will be traversed until a leaf is reached
    while (*current) {
        
        // compares the x (latitude) if the level is even
        if (level % 2 == 0){
            
            // if the value is less than or equal to la, it will be inserted as the left child
            if (fabs(la) < fabs(((*current)->lat)) || fabs(la) == fabs(((*current)->lat))){
                current = &((*current)->left);
            }
            
            // otherwise the node will be inserted by a right child
            else {
                current = &((*current)->right);
            }
            
           // Changes the level to an even number for the next iteration
            level = 1;
        }
        
        // compares the y (longitude) if the level is odd
        else {
            
            // if the value is less than or equal to lo, current is updated to point to the left pointer
            if (fabs(lo) < fabs(((*current)->lon)) || fabs(lo) == fabs(((*current)->lon))){
                current = &((*current)->left);
            }
            
            // otherwise the current will be updated to the right pointer
            else {
                current = &((*current)->right);
            }
            
            // Changes the level to an even number for the next iteration
            level = 0;
        }
    }
    
    // Create a new node at the left or right pointer of the leaf node
    *current = new KDNode(la,lo,desc);
    
    // Increase the size
    size++;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Traverses through the tree until a leaf node
 is reached. The comparisons are made based on
 a 2D data structure where K = 2. In this case
 we are comparing the latitude and longitude
 for a given node. The even levels will compare
 latitude and the odd levels will compare the
 longitude. Insertions are made at a leaf.
 -------
 The double pointer current points to either the
 left or right pointer of a node.
 * * * * * * * * * * * * * * * * * * * * * * * * * * */
 void KDTree::rangeQuery(KDNode *current, double rad, double la, double lo, const std::string &filter, int level){
 
     // Comparing based on latitude, when rangeQuery is called next it will compare longitude (level = 1)
     if (level == 0) {
     
         // Check if the node is a leaf
         if(current == NULL)return;
         // CONDITION 1: Latitude at line x = la is far right of radius, traverse the left subtree
         else if ((la + rad) < current->lat ) {
             rangeQuery(current->left, rad, la, lo, filter, 1);
         }
         // CONDITION 2: Latitude at line x = la is far left of radius, traverse the right subtree
         else if (current && (la - rad) > current->lat){
             rangeQuery(current->right, rad, la, lo, filter, 1);
         }
         // CONDITION 3: Latitude at line x = la intersects the radius, traverse through both subtrees
         else {
             // Compares distance from the current node to the center to the radius and the description to
             // verify if a point is within the range of the radius
             if (current->distance(la,lo) < rad && current->name.find(filter) != std::string::npos) {
                 std::cout << "\t[\"" << current->name << "\", " << current->lat << ", " << current->lon << "],\n";
                 // Increment the counter if a point is wihtin the range
                 count ++;
             }
             rangeQuery(current->left, rad, la, lo, filter, 1);
             rangeQuery(current->right, rad, la, lo, filter, 1);
         }
     }
     
     // Comparing based on longitude, when rangeQuery is called next it will compare latitude (level = 0)
     else {
     
         // Check if the node is a leaf
         if(current == NULL)return;
         // CONDITION 1: Longitude at line y = lo is far right of radius, traverse the left subtree
         else if (current && (lo + rad) < current->lon){
             rangeQuery(current->left, rad, la, lo, filter, 0);
         }
         // CONDITION 2: Longitude at line y = lo is far left of radius, traverse the right subtree
         else if (current && (lo - rad) > current->lon){
             rangeQuery(current->right, rad, la, lo, filter, 0);
         }
         // CONDITION 3: Longitude at line y = lo intersects the radius, traverse both subtrees
         else {
             // Compares distance from the current node to the center to the radius and the description to
             // verify if a point is within the range of the radius
             if (current->distance(la,lo) < rad && current->name.find(filter) != std::string::npos) {
                 std::cout << "\t[\"" << current->name << "\", " << current->lat << ", " << current->lon << "],\n";
                 // Increment the counter if a point is wihtin the range
                 count ++;
             }
             rangeQuery(current->left, rad, la, lo, filter, 0);
             rangeQuery(current->right, rad, la, lo, filter, 0);
         }
     }
     return;
 }

/* * * * * * * * * * * * * * * * * * * * * * * * * *
 Traverses through the tree until a leaf node
 is reached. The comparisons are made based on
 a 2D data structure where K = 2. In this case
 we are comparing the latitude and longitude
 for a given node. The even levels will compare
 latitude and the odd levels will compare the
 longitude. Insertions are made at a leaf.
 -------
 The double pointer current points to either the
 left or right pointer of a node.
 * * * * * * * * * * * * * * * * * * * * * * * * * */
unsigned int KDTree::printNeighbors(double la, double lo, double rad, const std::string &filter) {
    
    std::cout << "var markers = [\n";
    std::cout << "\t[\"" << "CENTER" << "\", " << la << ", " << lo << "],\n";
    
    // Range search using the root of the tree with the level starting at 0 (even,compares latitude first)
    rangeQuery(root, rad, la, lo, filter,0);
    
    std::cout << "];\n";
    
    // Returns the total number of points that were within the search radius
    return count;
}


/* * * * * * * * * * * * * * * * * * * * * * *
  Returns the size of the tree
 * * * * * * * * * * * * * * * * * * * * * * */
unsigned int KDTree::getSize() {
    return size;
}
