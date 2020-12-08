#ifndef _VQ
#define _VQ
#include "olc_net.h"

using namespace std;

template <typename T>
class vqueue {
public:
  vqueue(){}
  vqueue(const vector<T>& inq): m_q(inq) {}

  void print() const {
    for (const auto&i : m_q){
      std::cout << i ;
      std::cout << "\n";
      //i.print();
    }
    std::cout << std::endl;
  }
  std::string print_str() const {
    std::stringstream ss;
    for (const auto&i : m_q){
      ss << i ;
      ss << " ";
      //ss << i << " ";
      //i.print();
      //ss << i.print() << " ";
    }
    ss << '\n';
    return ss.str();
  }
  const std::vector<T>& get_q() const{
    return m_q;
  }
  void mvup(const T& x){
    auto result = std::find(std::begin(m_q), std::end(m_q), x);

    if (result != std::end(m_q)) {
      if (result != std::begin(m_q)){
        iter_swap(result-1, result);
      }
    }
  }
  void mvdn(const T& x){
    auto result = std::find(std::begin(m_q), std::end(m_q), x);

    if (result < std::end(m_q)-1) {
      std::cout << "moving down\n";
      iter_swap(result+1, result);
    }
  }
  void add_item(const T& x) {
    m_q.push_back(x);
  }
  void rm_item(const T& x){
    auto result = std::find(std::begin(m_q), std::end(m_q), x);

    if (result != std::end(m_q)) { // found it
      m_q.erase(result);
    }
  }
  const T& front() {
    return m_q.front();
  }
  T pop_front() {
    T tmp = m_q[0]; 
    m_q.erase(m_q.begin());
    return tmp;
  }
  bool empty() const {
    return m_q.empty();
  }

private:
  std::vector<T> m_q;
};


#endif
