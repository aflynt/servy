#ifndef _MACHINE
#define _MACHINE
#include "net_common.h"
//#include <iostream>

using namespace std;
class machine{

public:
  machine(){};
  machine(const string& name, const int size):
    m_name(name),
    m_size(size),
    m_free(size),
    m_res(0){}

  const string& get_name() const { return m_name;}
  int get_size() const { return m_size;}
  int get_res()  const { return m_res;}
  int get_free() const { return m_size-m_res;}

  void print(){
    std::cout << m_name <<
      " srf: " << setw(2) << m_size   <<
      " | " << setw(2) << m_res    <<
      " | " << setw(2) << m_free   << std::endl;
  }

  int alloc(const int n){
    if ( n <= m_free ){
      //SZ:  4 -> |0|1|2|3|
      //RES: 1 -> |x|.|.|.|
      //FREE:3 -> | |x|x|x|
      // can allocate
      m_res  += n;
      m_free -= n;
      return n;
    }
    else{
      return -1;
    }
  }
  int free(const int n){
    //SZ:  4 -> |0|1|2|3|
    //RES: 1 -> |x| | | |
    //FREE:3 -> | |x|x|x|
    if (n <= m_res){
      m_res  -= n;
      m_free += n;
    }
    return m_free;
  }

  template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & m_name;
      ar & m_size;
      ar & m_res;
      ar & m_free;
    }
  friend class boost::serialization::access;
  //friend std::ostream& operator<<(std::ostream& os, const machine& m);
  
private:
  string m_name;
  int m_size;
  int m_res;
  int m_free;
};
//std::ostream& operator<<(std::ostream& os, const machine& m)
//{
//  return os << m;
//}


//std::string get_serial_machine(const machine& m)
//{
//  std::stringstream ss;
//  boost::archive::text_oarchive oa(ss);
//  oa << m;
//  return ss.str();
//}

// split "thor:60" with delim : into str and int
//vector<machine> get_machines( vector<string>& vs) {
std::vector<machine> get_machines( const std::string& mstring) {
    
    // first split on ','
    auto vs = split(mstring, ','); // now have split machines vector of strings

    std::vector<machine> result;

    // split each string into str,int pairs
    for (auto& s : vs){
      for (char& ch : s) //replace each punct char with space
        switch(ch) {
          case ':':
            ch = ' ';
        }

      std::stringstream ss (s); // make istream ss reading from string s
      std::string name;
      int numcores;
      ss >> name;
      ss >> numcores;
      result.push_back(machine{name,numcores});
    }
    return result;
}

std::string get_machine_str (const std::vector<machine>& machines){
    string mstr = "";
    bool first = true;
    for (auto&m : machines) {
      string mname = m.get_name();
      string ssize = std::to_string(m.get_size());
      if (first){
        mstr += mname + ":" + ssize;
        first = false;
      }
      else
        mstr += "," + mname + ":" + ssize;
    }
    return mstr;
}
#endif
