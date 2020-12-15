/*
	MMO Client/Server Framework using ASIO
	"Happy Birthday Mrs Javidx9!" - javidx9

	Videos:
	Part #1: https://youtu.be/2hNdkYInj4g
	Part #2: https://youtu.be/UbjxGvrDrbw

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2020 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020

*/

#ifndef _NET_COMMON
#define _NET_COMMON

#include <memory>
#include <thread>
#include <mutex>
#include <deque>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#define ASIO_STANDALONE
#endif

#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

#include <boost/asio/streambuf.hpp>
#include <bits/stdc++.h>

enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
  SendText,
  SendMachine,
  SendRun,
  Qstatus,
  MoveRunUp,
  MoveRunDown,
  RemoveRun
};


class Astr {
public:
  Astr() { s= "foo";}
  Astr(std::string& instr): s(instr) {}
  Astr(const std::string& instr): s(instr) {}
  void print_str() const
  {
    std::cout << s << std::endl;
  }
  template<class Archive>
    void serialize(Archive& ar, const unsigned int version)
    {
      ar & s;
    }
  friend class boost::serialization::access;
  friend std::ostream & operator<<(std::ostream & os, const Astr& astr);
private:
  std::string s;
};
BOOST_CLASS_VERSION(Astr, 1)

std::ostream & operator<<(std::ostream & os, const Astr& astr)
{
  return os << astr.s;
}

void save_astr(const Astr& astr, const char* filename)
{
  std::ofstream ofs(filename, std::ofstream::out);
  boost::archive::text_oarchive oa(ofs);
  oa << astr;
}

void restore_astr(Astr& astr, const char* filename)
{
  std::ifstream ifs(filename, std::ifstream::in);
  boost::archive::text_iarchive ia(ifs);

  ia >> astr;
}

//void chg2astr(Astr& astr, std::stringstream& ss)
//
//
void fwrite_serial(const std::string& ss)
{
  // write string to file
  std::ofstream ofs("foobar.txt", std::ofstream::out);
  ofs << ss;
  ofs.close();
}
//
void fwrite_astr(const std::string& ss)
{
  // write string to file
  std::ofstream ofs("foobar.txt", std::ofstream::out);
  ofs << ss;
  ofs.close();
}

template <typename T>
void fread_T(T& astr)
{
  // read back in
  std::ifstream ifs("foobar.txt", std::ifstream::in);
  boost::archive::text_iarchive ia(ifs);

  ia >> astr;
}

void fread_astr(Astr& astr)
{
  // read back in
  std::ifstream ifs("foobar.txt", std::ifstream::in);
  boost::archive::text_iarchive ia(ifs);

  ia >> astr;
}

template <typename T>
std::string get_serial_str(const T& x)
{
  std::stringstream ss;
  boost::archive::text_oarchive oa(ss);
  oa << x;
  return ss.str();
}

//trim from end (in place)
static inline void rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

std::string safe_getenv(const char * var){
  std::string rstr{"BAD"};
  if (const char* env_p = std::getenv(var)){
      std::string estr(env_p);
      rstr = estr;
  }
  return rstr;
}


std::vector<std::string> split(const std::string& s, char delim) {
  std::vector<std::string> result;
  std::stringstream ss (s);
  std::string item;

    while (getline(ss,item, delim)) {
      result.push_back(item);
    }
    return result;
}




#endif
