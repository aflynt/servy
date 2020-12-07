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

#include <iostream>
#include "olc_net.h"


template<typename T>
std::string pop_str(olc::net::message<T>& msg)
{

  // pop size
  uint16_t n;
  msg >> n;

  char char_array[n];
  
  // pop sstring
  for (auto i = 0; i < n; i++){
    msg >> char_array[i];
  }

  // put char[] into string 
  std::string sstr(char_array);

  std::cout << "trying to unpack: [" << sstr <<  "]" << std::endl;
  return sstr;
}

class CustomServer : public olc::net::server_interface<CustomMsgTypes>
{
public:
	CustomServer(uint16_t nPort) : olc::net::server_interface<CustomMsgTypes>(nPort)
	{

	}

protected:
	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		return true;
	}

	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	{
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	// Called when a message arrives
	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg)
	{
		switch (msg.header.id){
		case CustomMsgTypes::ServerPing:{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";

			// Simply bounce message back to client
			client->Send(msg);
		  } break;

		case CustomMsgTypes::MessageAll:{
			std::cout << "[" << client->GetID() << "]: Message All\n";

			// Construct a new message and send it to all clients
			olc::net::message<CustomMsgTypes> new_msg;
			new_msg.header.id = CustomMsgTypes::ServerMessage;
			new_msg << client->GetID();
			MessageAllClients(new_msg, client);
		  } break;

    case CustomMsgTypes::SendRun:{
			 std::cout << "[" << client->GetID() << "]: Send Run\n";
       std::string ss = pop_str(msg);
       // replace archive 17 with archive 16
       std::string str2 ("archive 17");
       std::size_t found = ss.find(str2);
       if (found !=std::string::npos)
         ss.replace(ss.find(str2),str2.length(),"archive 16");

       // covert from stringstream to class
       run arun;
       fwrite_serial(ss); // write the serialized string
       fread_T(arun);
       arun.print();
       
      }break;
    case CustomMsgTypes::SendMachine:{
			 std::cout << "[" << client->GetID() << "]: Send Machine\n";
       std::string ss = pop_str(msg);
       // replace archive 17 with archive 16
       std::string str2 ("archive 17");
       std::size_t found = ss.find(str2);
       if (found !=std::string::npos)
         ss.replace(ss.find(str2),str2.length(),"archive 16");

       // covert from stringstream to machine class
       machine amachine;
       fwrite_serial(ss); // write the serialized string
       fread_T(amachine);
       amachine.print();
       
      }break;

		case CustomMsgTypes::SendText:{
			 std::cout << "[" << client->GetID() << "]: Send Text\n";
       
       // new pop str method
       // get serialized string
       std::string ss = pop_str(msg);

       // replace archive 17 with archive 16
       std::string str2 ("archive 17");
       std::size_t found = ss.find(str2);
       if (found !=std::string::npos)
         ss.replace(ss.find(str2),str2.length(),"archive 16");

       // covert from stringstream to astr class
       Astr astr;
       //std::stringstream str_strm(ss);
       fwrite_astr(ss);
       fread_astr(astr);
       astr.print_str();

       // reply back
       /////////////////////////////////////////////
			 olc::net::message<CustomMsgTypes> new_msg;
			 new_msg.header.id = CustomMsgTypes::SendText;

       // push uid
       uint32_t uid = client->GetID();
       new_msg << uid;

       std::string outstr = "[APPROVED]";
       int n = outstr.length()+1;
       char char_array[n];
       strcpy(char_array, outstr.c_str());
       
       // push string
       for (int i = n-1; i >= 0; i--)
         new_msg << char_array[i];

       // push string length
       new_msg << n;

       // send back modified string
			 client->Send(new_msg);
		  } break;
		}
	}
};

void myswapper(vector<int>& v, const int i1, const int i2){
  iter_swap(v.begin() + i1, v.begin() + i2);
}

int main()
{
	CustomServer server(60000); 
	server.Start();

  //std::vector<int> v{10,20,15,40};
  vqueue<run> vq;

  machine rm1("c21",10);
  machine rm2("c22",10);
  machine rm3("c23",10);
  vector<machine> rmv;
  rmv.push_back(rm1);

  run r1(101, "foo1.sim", rmv);
  rmv.push_back(rm2);
  run r2(102, "foo1.sim", rmv);
  rmv.push_back(rm3);
  run r3(103, "foo1.sim", rmv);

  machine cm1("c21",30);
  machine cm2("c22",30);
  machine cm3("c23",30);
  vector<machine> cmv;
  cmv.push_back(cm1);
  cmv.push_back(cm2);
  cmv.push_back(cm3);

  cluster c(cmv);

  if (c.can_alloc(r1)) {
    c.alloc(r1);
    auto cm = c.get_machines();
    string rname = "R1: ";
    std::cout << "Alloc " << rname << std::endl;
    for (machine& m : cm) {
      m.print();
      //std::cout << m << std::endl;
    }
  }
  if (c.can_alloc(r2)) {
    c.alloc(r2);
    auto cm = c.get_machines();
    string rname = "R2: ";
    std::cout << "Alloc " << rname << std::endl;
    for (machine& m : cm) {
      m.print();
    }
  }
  c.free(r2);
  string rname = "R2: ";
  std::cout << "FREE " << rname << std::endl;
  auto cm = c.get_machines();
  for (machine& m : cm) {
    m.print();
  }
  if (c.can_alloc(r3)) {
    c.alloc(r3);
    auto cm = c.get_machines();
    string rname = "R3: ";
    std::cout << "Alloc " << rname << std::endl;
    for (machine& m : cm) {
      m.print();
    }
  }
  //std::cout << "Can alloc for run#1: " << canRun1 << std::endl;
  //std::cout << "Can alloc for run#2: " << canRun2 << std::endl;
  //std::cout << "Can alloc for run#3: " << canRun3 << std::endl;



	while (1)
	{
		server.Update(-1, true);
	}
	
	return 0;
}
