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

#include <fstream>
#include <iostream>
#include "olc_net.h"

template<typename T>
void push_str(olc::net::message<T>& msg, const std::string& s)
{
  std::cout << "trying to send: [" << s <<  "]" << std::endl;

  // PUSH A TEXT STRING OVER USING A CHAR
  int n = s.length()+1;
  char char_array[n];
  strcpy(char_array, s.c_str());
  
  // push string
  for (int i = n-1; i >= 0; i--)
    msg << char_array[i];

  // push size
  msg << n;
}

class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{
public:
	void PingServer()	
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;

		// Caution with this...
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();		

		msg << timeNow;
		Send(msg);
	}

	void MessageAll()
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;		
		Send(msg);
	}

  void SendText(const std::string& s)
  {
    // generate a message thing
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::SendText;		

    // USE BOOST SERIALIZATION
    // setup archive file to serialize to
    //std::string filename(boost::archive::tmpdir());
    std::string filename("/home/pi");
    filename += "/newfile.txt";

    // make class holding string
    Astr astr(s);

    // dump string to file
    save_astr(astr, filename.c_str());

    // convert from string to serialized string
    std::string ss = get_serial_str(astr);

    // push serial string
    push_str(msg, ss);
      
    // send it
		Send(msg);
  }
};

int main()
{
	CustomClient c;
	c.Connect("127.0.0.1", 60000);

	bool key[3] = { false, false, false };
	bool old_key[3] = { false, false, false };
  int usrpick;

  std::string mstring = "run x with 128 cores";
//  DEV ZONE
//
//  {
//    std::ofstream ofs("foo.txt");
//
//    std::string ss("hello world");
//    Astr astr(ss);
//    astr.print_str();
//
//    // save to archive
//    boost::archive::text_oarchive oa(ofs);
//
//    oa << astr;
//  }
//
//  {
//    //newscope
//    std::ifstream ifs("foo.txt");
//
//    Astr bstr;
//    boost::archive::text_iarchive ia(ifs);
//    ia >> bstr;
//    bstr.print_str();
//  
//  }
//  DEV ZONE

	bool bQuit = false;
	//bool bQuit = true;
	while (!bQuit)
	{
		//if (GetForegroundWindow() == GetConsoleWindow())
    if(1)
		{
      std::cin >> usrpick;
			//key[0] = GetAsyncKeyState('1') & 0x8000;
			//key[1] = GetAsyncKeyState('2') & 0x8000;
			//key[2] = GetAsyncKeyState('3') & 0x8000;
		}

		//if (key[0] && !old_key[0]) c.PingServer();
		//if (key[1] && !old_key[1]) c.MessageAll();
		//if (key[2] && !old_key[2]) bQuit = true;
		if (usrpick == 0) c.SendText(mstring);
		if (usrpick == 1) c.PingServer();
		if (usrpick == 2) c.MessageAll();
		if (usrpick == 3) bQuit = true;

		//for (int i = 0; i < 3; i++) old_key[i] = key[i];

		if (c.IsConnected()) // c is connected
		{
			if (!c.Incoming().empty()) // c has messages
			{
        // get the 1st message
				auto msg = c.Incoming().pop_front().msg;

				switch (msg.header.id){
				case CustomMsgTypes::ServerAccept:{
					// Server has acceppted the connection
					std::cout << "Server Accepted Connection\n";
          } break;

				case CustomMsgTypes::ServerPing:{
					// Server has responded to a ping request
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen;
					msg >> timeThen;
					std::cout << "Ping: "
                    << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
          } break;

				case CustomMsgTypes::ServerMessage:{
					// Server has responded to a message
					uint32_t clientID;
					msg >> clientID;
					std::cout << "Hello from [" << clientID << "]\n";
          } break;

				case CustomMsgTypes::SendText:{
					// Server has responded to a string message
          //
          //

          // pop string length
          int n;
          msg >> n; 

          // pop string
          char char_array[n];
          for (int i = 0; i < n; i++){
            msg >> char_array[i];
          }
          std::string smsg(char_array);

          // pop uid
          uint32_t uid;
          msg >> uid;
					std::cout << "Server says: (" << smsg << ") for uid: " << uid << "\n";

				  } break;
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			bQuit = true;
		}
	}

	return 0;
}
