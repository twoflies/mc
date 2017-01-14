#ifndef NODE_H_
#define NODE_H_

// Node

class Node {
 public:
  Node();
  virtual ~Node();
  bool isOnline();
 
 private:
  bool online_;
};

#endif // NODE_H_
