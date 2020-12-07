#ifndef _CLUSTER
#define _CLUSTER
#include "net_common.h"
#include "machine.h"
#include "run.h"

class cluster {
public:
  cluster(){};
  cluster(const vector<machine>& mlist): machines(mlist) {}

  bool can_alloc(const run& r) const;
  void alloc(const run& r);
  void free(const run& r);
  const vector<machine>& get_machines() const {
    return machines;
  }

private:
  vector<machine> machines;
};

bool cluster::can_alloc(const run& r) const {

  bool canRun = true;
  // loop over all machines in run
  for (auto& rm : r.get_machines()){
    auto rname = rm.get_name();
    auto rsize = rm.get_size();
    // check against cluster machine
    for (auto& cm : machines) {
      auto cname = cm.get_name();
      auto cfree = cm.get_free();
      if (rname == cname) { // found requested machine
        //check if size is <= free;
        if (rsize > cfree){
          //std::cout << "Failed for m,size: " << rname << ", " << rsize << std::endl;
          canRun = false;
        }
        //else {
        //  std::cout << "OK : " << rname 
        //            << " size " << rsize
        //            << " <= " << cfree << std::endl;
        //}
      }
    }
  }
  return canRun;
};
void cluster::alloc(const run& r) {
  // allocate run's machines onto cluster machines
 
  for (auto& rm : r.get_machines()){ // for all run machines
    auto rname = rm.get_name();
    auto rsize = rm.get_size();
    for (auto& cm : machines) {  // for all cluster machines
      auto cname = cm.get_name();
      if (rname == cname) { // found requested machine
        cm.alloc(rsize);    // reserve run's size
      }
    }
  }
}

#endif
