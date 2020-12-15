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
    // build the machine cluster
    auto vm = fget_machines("cluster_machines.dat");
    for(auto m : vm){
      mcluster.add_machine(m);
      m.print();
    }

    // point thread to handle_runs()
    runthread = std::thread([this]() { handle_runs(); });


	}
  void handle_runs() {
    while(canHandle) {
      std::unique_lock<mutex> locker(mu);
      cond.wait(locker, [this](){ return !vq.empty() && 
                   mcluster.can_alloc(vq.front()); }); //wake when ready
      run arun = vq.pop_front();
      mcluster.alloc(arun);

      std::cout << "STARTING RUN: " << arun << std::endl;
      std::cout << print_qstatus();

      locker.unlock();
      std::thread trun(&CustomServer::execute_run, this, arun);
      trun.detach();
    }
  }
  void execute_run(run arun){
      arun.execute();

      // free machine resources
      std::unique_lock<mutex> locker(mu);

      mcluster.free(arun);

      cond.notify_one();
      locker.unlock();
  }

  std::string print_qstatus(){
    std::stringstream ss;

    ss << "== Q STATUS ==\n";
    ss << vq.print_str();

    ss << std::endl;
    ss << "== CLUSTER STATUS ==\n";
    auto qms = mcluster.get_machines();
    for(auto m : qms){
      ss << m.print_str();
    }
    return ss.str();
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

      // finally have a real run
      //arun.print();
      
      // push it into my vq<runs> vq
      std::unique_lock<mutex> locker(mu);
      vq.add_item(arun);

      std::cout << print_qstatus();

      locker.unlock();
      cond.notify_one();
    }break;
    case CustomMsgTypes::MoveRunUp:{
			std::cout << "[" << client->GetID() << "]: Move Run Up\n";
      std::string ss = pop_str(msg);
      // replace archive 17 with archive 16
      std::string str2 ("archive 17");
      std::size_t found = ss.find(str2);
      if (found !=std::string::npos)
        ss.replace(ss.find(str2),str2.length(),"archive 16");

      // covert from stringstream to class
      run arun;
      std::unique_lock<mutex> locker(mu);
      fwrite_serial(ss); // write the serialized string to file
      fread_T(arun);     // read serialized type from file

      // finally have a real run
      std::cout << "LOOKING TO MVUP RUN: " << arun;
      //arun.print();
      
      // move the reference run up
      vq.mvup(arun);

      std::cout << print_qstatus();

      locker.unlock();
      cond.notify_one();
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

      //// push uid
      //uint32_t uid = client->GetID();
      //new_msg << uid;

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
		case CustomMsgTypes::Qstatus:{
			std::cout << "[" << client->GetID() << "]: Qstatus\n";
      
      // reply back
      /////////////////////////////////////////////
			olc::net::message<CustomMsgTypes> new_msg;
			new_msg.header.id = CustomMsgTypes::SendText;

      // out message string
      std::unique_lock<mutex> locker(mu);
      std::string outstr = print_qstatus();
      locker.unlock();

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
private:
  std::mutex mu;
  vqueue<run> vq;
  std::condition_variable cond;
  cluster mcluster;
  std::thread runthread;
  bool canHandle = true;
};

int main()
{
	CustomServer server(60000); 
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}
	
	return 0;
}

/*
  //std::vector<int> v{10,20,15,40};
  //vqueue<run> vq;

  //machine rm1("c21",10);
  //machine rm2("c22",10);
  //machine rm3("c23",10);
  //vector<machine> rmv;
  //rmv.push_back(rm1);

  //run r1(101, "foo1.sim", rmv);
  //rmv.push_back(rm2);
  //run r2(102, "foo1.sim", rmv);
  //rmv.push_back(rm3);
  //run r3(103, "foo1.sim", rmv);

  //machine cm1("c21",30);
  //machine cm2("c22",30);
  //machine cm3("c23",30);
  //vector<machine> cmv;
  //cmv.push_back(cm1);
  //cmv.push_back(cm2);
  //cmv.push_back(cm3);

  //cluster c(cmv);

  //if (c.can_alloc(r1)) {
  //  c.alloc(r1);
  //  auto cm = c.get_machines();
  //  string rname = "R1: ";
  //  std::cout << "Alloc " << rname << std::endl;
  //  for (machine& m : cm) {
  //    m.print();
  //    //std::cout << m << std::endl;
  //  }
  //  std::thread t([&](){r1.execute();});
  //  t.detach();
  //}
  //if (c.can_alloc(r2)) {
  //  c.alloc(r2);
  //  auto cm = c.get_machines();
  //  string rname = "R2: ";
  //  std::cout << "Alloc " << rname << std::endl;
  //  for (machine& m : cm) {
  //    m.print();
  //  }
  //  std::thread t([&](){r2.execute();});
  //  t.detach();
  //}
  //c.free(r2);
  //string rname = "R2: ";
  //std::cout << "FREE " << rname << std::endl;
  //auto cm = c.get_machines();
  //for (machine& m : cm) {
  //  m.print();
  //}
  //if (c.can_alloc(r3)) {
  //  c.alloc(r3);
  //  auto cm = c.get_machines();
  //  string rname = "R3: ";
  //  std::cout << "Alloc " << rname << std::endl;
  //  for (machine& m : cm) {
  //    m.print();
  //  }
  //}
  ////std::cout << "Can alloc for run#1: " << canRun1 << std::endl;
  ////std::cout << "Can alloc for run#2: " << canRun2 << std::endl;
  ////std::cout << "Can alloc for run#3: " << canRun3 << std::endl;
*/

