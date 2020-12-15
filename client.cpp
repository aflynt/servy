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
/*
  //std::cout << "\033[1;35m [35] LIGHT BLUE \033[0m" << std::endl;
  //std::cout << "\033[1;44m [44] BG blue , FG white \033[0m" << std::endl;
  //std::cout << "\033[1;41m [41] BG red  , FG white \033[0m" << std::endl;
  //std::cout << "\033[1;37m [37] BG blk  , FG bold white \033[0m" << std::endl;
  //std::cout << "\033[1;01m [01] white bold \033[0m" << std::endl;
  //std::cout << "\033[1;02m [02] grey font \033[0m" << std::endl;
  //std::cout << "\033[1;03m [03] blk font white bg \033[0m" << std::endl;
  //std::cout << "\033[1;04m [04] white font underlined \033[0m" << std::endl;
  //std::cout << "\033[1;05m [05] white font blinking \033[0m" << std::endl;
  //std::cout << "\033[1;06m [06] white font strike thru \033[0m" << std::endl;
  //std::cout << "\033[1;07m [07] \033[0m" << std::endl;
  //std::cout << "\033[1;30m [30] dark grey font \033[0m" << std::endl;
  //std::cout << "\033[1;31m [31] red font \033[0m" << std::endl;
  //std::cout << "\033[1;33m [33] yellow font \033[0m" << std::endl;
  //std::cout << "\033[1;34m [34] purple font \033[0m" << std::endl;
*/
#include "olc_net.h"
bool bQuit = false;

template<typename T>
void push_str(olc::net::message<T>& msg, const std::string& s)
{
  std::cout << "trying to send: [" << s <<  "]" << std::endl;

  // PUSH A TEXT STRING OVER USING A CHAR
  uint16_t n = s.length()+1;
  char char_array[n];
  strcpy(char_array, s.c_str());
  
  // push string
  for (auto i = n-1; i >= 0; i--)
    msg << char_array[i];

  // push size
  msg << n;
}

class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{
public:
  CustomClient(const std::string& ip, const int port): m_ip(ip) {
	  Connect(ip, port);
    std::string user{safe_getenv("USER")};
    std::string dir {safe_getenv("PWD")};
    m_user = user;
    m_dir = dir;
  }

  void print_state()
  {
    std::cout << "IP:     " <<     m_ip << std::endl;
    std::cout << "USER:   " <<   m_user << std::endl;
    std::cout << "DIR:    " <<    m_dir << std::endl;
  }

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

  void SendRun(const run& arun)
  {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::SendRun;		

    std::string ss = get_serial_str(arun);
    push_str(msg, ss);
		Send(msg);
  }
  void SendMachine(const machine& amachine)
  {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::SendMachine;		

    std::string ss = get_serial_str(amachine);
    push_str(msg, ss);
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
    //std::string filename("/home/pi");
    //filename += "/newfile.txt";

    // make class holding string
    Astr astr(s);

    // dump string to file
    //save_astr(astr, filename.c_str());

    // convert from string to serialized string
    std::string ss = get_serial_str(astr);

    // push serial string
    push_str(msg, ss);
      
    // send it
		Send(msg);
  }
  void Qstatus()
  {
    // generate a message thing
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::Qstatus;		
		Send(msg);
  }
  void MoveRunUp(run arun)
  {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MoveRunUp;		

    std::cout << "Client MVUP with run: " << arun << std::endl;
    std::string ss = get_serial_str(arun);
    push_str(msg, ss);
		Send(msg);
  }
private:
  std::string m_ip;
  std::string m_user;
  std::string m_dir ;
};


void get_userpick(std::atomic<int>& usrpick, std::atomic<bool>& gotnews){
  int tmp;
  while (!bQuit) {
    std::cin >> tmp;
    usrpick = tmp;
    gotnews = true;
  }
}

int main(int argc, char ** argv)
{
  bool sendrun = false;
  int id = -1;
  std::string user    = "BAD";
  std::string dir     = "BAD";
  std::string simfile = "BAD";
  std::string run_cmd = "BAD";
  std::string machstr = "BAD";
  if (argc > 1 ) { // parse if args are passed
    if (strcmp("-r",argv[1]) == 0 ||
        strcmp("-run",argv[1]) == 0 ){
      sendrun = true;
      for (int i = 2; i < argc; i++)
      {
        if (strcmp("-u",argv[i]) == 0) { // user
          std::string tmpstr{argv[++i]};
          user = tmpstr;
        }
        if (strcmp("-d",argv[i]) == 0) { // directory
          std::string tmpstr{argv[++i]};
          dir = tmpstr;
        }
        if (strcmp("-i",argv[i]) == 0 || // id
            strcmp("-id",argv[i]) == 0) {
          std::string tmpstr{argv[++i]};
          id = std::stoi(tmpstr);
        }
        if (strcmp("-f",argv[i]) == 0) { // file
          std::string tmpstr{argv[++i]};
          simfile = tmpstr;
        }
        if (strcmp("-c",argv[i]) == 0) { // run command 
          std::string tmpstr{argv[++i]};
          run_cmd = tmpstr;
        }
        if (strcmp("-m",argv[i]) == 0 || // machines
            strcmp("-on",argv[i]) == 0) {
          std::string tmpstr{argv[++i]};
          machstr = tmpstr;
        }
      }
    } // DONE PARSE RUN
    // CHECK Environment vars for args not passed
    if (user    == "BAD") user    = safe_getenv("USER");
    if (dir     == "BAD") dir     = safe_getenv("PWD");
    if (simfile == "BAD") simfile = safe_getenv("SIM_NAME");
    if (run_cmd == "BAD") run_cmd = safe_getenv("RUN_COMMAND");
    if (machstr == "BAD") machstr = safe_getenv("MACHINES");
    if (id == -1){
      std::string tmpstr = safe_getenv("RUN_ID");
      if (tmpstr != "BAD")
        id = std::stoi(safe_getenv("RUN_ID"));
      else {
        std::cout << "ERROR: no args to -id and no RUN_ID env var\n";
        return 0; // bail out
      }
    }
    std::cout << "AFTER PARSING:" << std::endl;
    std::cout << "user: "    << user << std::endl;
    std::cout << "id: "      << id << std::endl;
    std::cout << "dir: "     << dir << std::endl;
    std::cout << "simfile: " << simfile << std::endl;
    std::cout << "run_cmd: " << run_cmd << std::endl;
    std::cout << "machstr: " << machstr << std::endl;
  } // DONE PARSE ARGS



  std::string ip{safe_getenv("BOOST_IP")};
	CustomClient c(ip, 60000);
  c.print_state();


  std::string mstring = "run x with 128 cores";


  //std::atomic<int> usrpick;
  std::atomic<bool> gotnews {false};
  std::atomic<int> usrpick  {-99};
  std::thread t1(get_userpick, std::ref(usrpick),std::ref(gotnews));
	while (!bQuit)
	{

		if (sendrun) 
    {
      auto ivm = get_machines(machstr);

      std::cout << "\033[1;32mINPUT MACHINES:\033[0m" << std::endl;
      for(auto&m : ivm){
        m.print();
      }

      run r1{user, id, dir, simfile, run_cmd, ivm};

      c.SendRun(r1);
      sendrun = false;
    }
    if(gotnews){
		  if (usrpick == 1) c.SendText(mstring);
		  if (usrpick == 2) c.PingServer();
		  if (usrpick == 3) c.MessageAll();
		  if (usrpick == 4) c.Qstatus();
		  if (usrpick == 5) {
        run rr{user,5};
        c.MoveRunUp(rr);
      }
		  if (usrpick == 0){
			  bQuit = true;
        t1.join();
        break;
      }
      gotnews = false;
    }

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

          // pop string length
          int n;
          msg >> n; 

          // pop string
          char char_array[n];
          for (int i = 0; i < n; i++){
            msg >> char_array[i];
          }
          std::string smsg(char_array);

          //// pop uid
          //uint32_t uid;
          //msg >> uid;
					std::cout << smsg << "\n";

				  } break;
				}
			}
		}
		else
		{
			std::cout << "Server Down\n";
			bQuit = true;
      t1.join();
		}
	}

	return 0;
}

