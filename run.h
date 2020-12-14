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
  run(){
    std::string env_user{safe_getenv("USER")};
    m_user = env_user;
    m_id = 0;
    ctor_helper();
  };
  run(const string& user, int id): m_user(user), m_id(id) {
    ctor_helper();
  };
  run(string user,
      int id,
      string dir,
      string sim_name,
      string run_cmd,
      vector<machine>mlist
      ):
    m_user(user),
    m_id(id),
    m_dir(dir),
    m_sim_name(sim_name),
    m_run_cmd(run_cmd),
    machines(mlist)
  {
    ctor_helper();
  }

  const vector<machine>& get_machines() const {
    return machines;
  }
  void print() const {
    string mstr = get_machine_str(machines);
    std::cout << "run( " << m_user << ", "
                         << m_id   << ", "
                         << m_dir  << ", "
                         << m_sim_name << ", " 
                         << m_run_cmd.substr(0,10)  << ", " 
                         << mstr << " )"
                         << std::endl;
  }
  void execute() const {

    //std::cout << execstr << std::endl;
    std::cout << "[EXE RUN] " << m_xid << "\n";

    // make subdirectory
    std::string xdir(m_dir);
    if(xdir.back() != '/')
      xdir += '/';
    xdir += m_xid;
    std::string runcmd("mkdir "+xdir);
    std::system(runcmd.c_str());

    // call executable
    //std::string xcs("cntdn 60 ");
    std::string xcs{m_run_cmd};

    xcs += " > " + xdir + "/res.dat";
    std::system(xcs.c_str());

    std::cout << "[END RUN] " << m_xid << "\n";
  }
  template<class Archive>
  void serialize(Archive& ar, const unsigned int version)
  {
    ar & m_id;
    ar & m_user;
    ar & m_dir ;
    ar & m_sim_name;
    ar & m_run_cmd;
    ar & m_xid;
    ar & machines;
  }
  friend class boost::serialization::access;

  friend inline bool operator==(const run& lhs, const run& rhs);
  friend inline bool operator!=(const run& lhs, const run& rhs);
  friend inline bool operator< (const run& lhs, const run& rhs);
  friend inline bool operator> (const run& lhs, const run& rhs);
  friend inline bool operator<=(const run& lhs, const run& rhs);
  friend inline bool operator>=(const run& lhs, const run& rhs);
  friend std::ostream& operator<<(std::ostream& os, const run& obj) {
    string mstr = get_machine_str(obj.machines);
    os << "run( " << obj.m_user << ", "
                  << obj.m_id   << ", "
                  << obj.m_dir  << ", "
                  << obj.m_sim_name << ", " 
                  << obj.m_run_cmd.substr(0,10)  << ", " 
                  << mstr << " )";
    return os;
  }
private:
  void ctor_helper(){
    std::stringstream ss;
    ss << m_user << '_'; 
    ss << std::setw(5) << std::setfill('0') << m_id;
    m_xid = ss.str();
  }
  int    m_id;
  string m_user;
  string m_dir ;
  string m_sim_name;
  string m_run_cmd;
  string m_xid; // extended id;
  vector<machine> machines;
};

inline bool operator==(const run& lhs, const run& rhs){ return (lhs.m_xid).compare(rhs.m_xid) == 0; }
inline bool operator!=(const run& lhs, const run& rhs){ return (lhs.m_xid).compare(rhs.m_xid) != 0; }
inline bool operator< (const run& lhs, const run& rhs){ return (lhs.m_xid).compare(rhs.m_xid) <  0; }
inline bool operator> (const run& lhs, const run& rhs){ return (lhs.m_xid).compare(rhs.m_xid) >  0; }
inline bool operator<=(const run& lhs, const run& rhs){ return (lhs.m_xid).compare(rhs.m_xid) <= 0; }
inline bool operator>=(const run& lhs, const run& rhs){ return (lhs.m_xid).compare(rhs.m_xid) >= 0; }

#endif
