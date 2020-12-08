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
  run(){};
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
    std::stringstream ss;
    ss << m_user << '_'; 
    ss << std::setw(5) << std::setfill('0') << m_id;
    //m_xid = m_user+'_'+to_string(m_id);
    m_xid = ss.str();
  }

  const vector<machine>& get_machines() const {
    return machines;
  }
  void print() const {
    string mstr = get_machine_str(machines);
    std::cout << "run( " << m_xid << ", "
                         << m_id  << ", "
                         << m_simname << ", " 
                         << mstr << " )"
                         << std::endl;
  }
  void execute() const {
    //std::string ccm_home("/shared/thor/apps/starccm/15.02.009-R8/STAR-CCM+15.02.009-R8");
    //std::string ccm_exec(ccm_home+"/star/bin/starccm+");
    //std::string execstr(ccm_exec);
    //execstr += ' ' + m_powerlist;
    //execstr += " -batch -on";
    //std::string mstr = get_machine_str(machines);
    //execstr += ' ' + mstr;
    //execstr += ' ' + m_dir;
    //if(execstr.back() != '/')
    //  execstr += '/';
    //execstr += m_simname;

    //std::cout << execstr << std::endl;
    std::cout << "[START] RUN" << m_xid << "\n";

    // make subdirectory
    std::string xdir(m_dir);
    if(xdir.back() != '/')
      xdir += '/';
    xdir += m_xid;
    std::string runcmd("mkdir "+xdir);
    std::system(runcmd.c_str());

    // call executable
    //std::string xcs("cntdwn 5 > ");
    std::string xcs("echo it works > ");
    xcs += xdir + "/res.dat";
    std::system(xcs.c_str());

    std::cout << "[END] RUN" << m_xid << "\n";
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
    ar & m_xid;
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
    os << "run( " << obj.m_xid << ", "
                  << obj.m_id  << ", "
                  << obj.m_simname << ", " 
                  << mstr << " )";
    return os;
  }
    

private:
  int    m_id;
  string m_simname;
  string m_user;
  string m_dir ;
  string m_podkey;
  string m_powerlist;
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
