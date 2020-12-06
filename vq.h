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
    for (auto&i : m_q){
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }
  std::string print_str() const {
    std::stringstream ss;
    for (auto&i : m_q){
      ss << i << " ";
    }
    ss << '\n';
    return ss.str();
  }
  const std::vector<T>& get_q(){
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

private:
  std::vector<T> m_q;
};


#endif
