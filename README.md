# Linda
Shared Memory Implementation over a Distributed System

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

