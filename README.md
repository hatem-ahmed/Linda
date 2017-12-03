# Linda
Shared Memory Implementation over a Distributed System

Project Objective
=================
        - Project is implemented in C
        - LINDA Implementation supports High Availability (Primary/Standby)
        - Implement LINDA Shared Memory over Distributed System #N Nodes
        - User will have the below interface to Read/Write TUPLEs in memory
        - Distributed System nodes are based on Linux CENTOS7
        - SOCKET Programming is Used betwwn nodes for communication
        - Process forking is used to have Client-Server communication
        - Port Numbers Used are random and outside the assigned range of 0-1023
        - MD5 HASH Algorithm is used for node selection
        - HASH table is generated according to number of nodes and updated if a node is added or removed
        - Database File is created to keep track of Primary/Standby Nodes
        - Temporary Files are creeated to store user tuples


Below Diagram is High Level Sequence of Implementation
-----------------------------------------------------
1-Pre-Requisite Configuration from user
    - User Defines the needed nodes in LINDA System
   i.e.  add (host_1, 129.210.16.85, xxxxx)(host_2, 129.210.16.88, xxxxx)(host_3, 129.210.16.90, xxxxx)  //port numbers are expected to be 5 digits


2- User will Choose any node to start LINDA from       (i.e. [linuxMachine ~]$ P1 host2)
3- Program will inform user of current machine IP/Port (i.e. 129.210.16.81 at port number: xxxxx)
4- User will enter tuple as one below Operations
        a- out (Insert tuple)
        b- rd  (Read tuple)
        c- in  (remove tuple)

                              +------+
                              | USER |
                              +------+
                                 ||
                                _||_
                                \  /
                                 \/
                        +----------------+
                        |  HASH Function |      "HASH is based on MD5 to choose Primary and Backup Nodes"
                        +----------------+
                                 ||
                                _||_
                                \  /
                                 \/
                     +------------------------+
                     |  File Store Function   |         "Incase of new tuple to the node, file is created in /tmp"
                     +------------------------+
                                 ||
                                _||_
                                \  /
                                 \/
                     +------------------------+
                     |  Node Communication    |         "Primary Standby Nodes exchange tuples based on network communication"
                     +------------------------+         "Network Communication is Based on Socket Programming between server and Client"



              +--------+    +--------+    +--------+                                     +--------+
              | NODE 1 |    | NODE 2 |    | NODE 3 |       SOCKET-Programming ===>       | NODE N |
              +--------+    +--------+    +--------+                                     +--------+

