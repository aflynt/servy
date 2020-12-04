#ifndef _RUN
#define _RUN

//#include <iostream>
//#include <cstdlib>
//#include <cstring>
//#include <cctype>

#include "net_common.h"
#include "machine.h"


using namespace std;
class run{

public:
  run(int id, const string& simname, vector<machine>mlist):
    m_id(id),
    m_simname(simname),
    machines(mlist)
  {
    std::string user{safe_getenv("USER")};
    std::string dir {safe_getenv("PWD")};
    std::string podkey {safe_getenv("PODKEY")};
    std::string powerlist {safe_getenv("POWERLIST")};
    m_user = user;
    m_dir = dir;
    m_podkey = podkey;
    m_powerlist = powerlist;
  }

  void print(){
    string mstr = get_machine_str(machines);
    std::cout << "run( " << m_id << ", "
                         << m_simname << ", " 
                         << mstr << " )"
                         << std::endl;
  }
  template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & m_id;
      ar & m_simname;
      ar & m_user;
      ar & m_dir ;
      ar & m_podkey;
      ar & m_powerlist;
      ar & machines;
    }
  friend class boost::serialization::access;

private:
  int    m_id;
  string m_simname;
  string m_user;
  string m_dir ;
  string m_podkey;
  string m_powerlist;
  vector<machine> machines;
};

#endif
