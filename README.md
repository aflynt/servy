# servy
asio server

# BASIC CALLS
$ ./clientel
   Initiates interactive server session
   further commands:
   * 0: SendText(message), send a text message
   * 1: PingServer(), ping the server
   * 2: MessageAll(), message all connected clients
   * 3: quit program

$ ./clientel -r
$ ./clientel -run
   Passes a run to server

   * runs have the "RUN_COMMAND" executed with the specified machines reserved on the cluster

# RUN CONSTRUCTOR
# run ( user, id, dir, sim_name, run_cmd, machines );

# COMMAND LINE FLAGS and equivalent ENVIRONMENT VARIABLES
-u    USER_NAME="$USER"
-id   RUN_ID=1
-i    RUN_ID=1
-c    RUN_COMMAND="$CCM_EXEC -batch -power -podkey $PODKEY -on $MACHINES $DIR/$SIM_NAME"
-on   MACHINES="thor:2,c22:40,c23:40"
-m    MACHINES="thor:2,c22:40,c23:40"
-d    DIR="`pwd`"
-f    SIM_NAME="foo.sim"


# connect to server with IP set via environment variable in .bashrc:
* export BOOST_IP=10.4.228.10

# ENVIRONMENT VARIABLES CHECKED IF NOT PASSED ON COMMAND LINE
* USER (automatically defined)
* PWD  (automatically defined)
* RUN_ID
* RUN_COMMAND
* MACHINES
* SIM_NAME

# UNUSED ENVIRONMENT VARIABLES
* export DIR=$DIR
* export PODKEY=$podkey
* export POWERLIST="-power -podkey $PODKEY -mpi platform -cpubind -licpath 1999@flex.cd-adapco.com"
