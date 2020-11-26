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
#include <fstream>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include <boost/archive/tmpdir.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/string.hpp>

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
};


class Astr {
public:
  Astr() { s= "foo";}
  Astr(std::string& instr): s(instr) {}
  Astr(const std::string& instr): s(instr) {}
  void print_str()
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


#endif
