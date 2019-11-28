#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <string>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <fstream>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>




using namespace std;
ssize_t
sock_fd_read(int sock, void *buf, ssize_t bufsize, int *fd)
{
    ssize_t     size;

    if (fd) {
        struct msghdr   msg;
        struct iovec    iov;
        union {
            struct cmsghdr  cmsghdr;
            char        control[CMSG_SPACE(sizeof (int))];
        } cmsgu;
        struct cmsghdr  *cmsg;

        iov.iov_base = buf;
        iov.iov_len = bufsize;

        msg.msg_name = NULL;
        msg.msg_namelen = 0;
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);
        size = recvmsg (sock, &msg, 0);
        if (size < 0) {
            perror ("recvmsg");
            exit(1);
        }
        cmsg = CMSG_FIRSTHDR(&msg);
        if (cmsg && cmsg->cmsg_len == CMSG_LEN(sizeof(int))) {
            if (cmsg->cmsg_level != SOL_SOCKET) {
                fprintf (stderr, "invalid cmsg_level %d\n",
                     cmsg->cmsg_level);
                exit(1);
            }
            if (cmsg->cmsg_type != SCM_RIGHTS) {
                fprintf (stderr, "invalid cmsg_type %d\n",
                     cmsg->cmsg_type);
                exit(1);
            }

            *fd = *((int *) CMSG_DATA(cmsg));
            printf ("received fd %d\n", *fd);
        } else
            *fd = -1;
    } else {
        size = read (sock, buf, bufsize);
        if (size < 0) {
            perror("read");
            exit(1);
        }
    }
    return size;
}
ssize_t
sock_fd_write(int sock, void *buf, ssize_t buflen, int fd)
{
    ssize_t     size;
    struct msghdr   msg;
    struct iovec    iov;
    union {
        struct cmsghdr  cmsghdr;
        char        control[CMSG_SPACE(sizeof (int))];
    } cmsgu;
    struct cmsghdr  *cmsg;

    iov.iov_base = buf;
    iov.iov_len = buflen;

    msg.msg_name = NULL;
    msg.msg_namelen = 0;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (fd != -1) {
        msg.msg_control = cmsgu.control;
        msg.msg_controllen = sizeof(cmsgu.control);

        cmsg = CMSG_FIRSTHDR(&msg);
        cmsg->cmsg_len = CMSG_LEN(sizeof (int));
        cmsg->cmsg_level = SOL_SOCKET;
        cmsg->cmsg_type = SCM_RIGHTS;

        printf ("passing fd %d\n", fd);
        *((int *) CMSG_DATA(cmsg)) = fd;
    } else {
        msg.msg_control = NULL;
        msg.msg_controllen = 0;
        printf ("not passing fd\n");
    }

    size = sendmsg(sock, &msg, 0);

    if (size < 0)
        perror ("sendmsg");
    return size;
}
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

typedef std::vector<std::string> stringvec;
void adjust_folders(stringvec& folders)
{
  stringvec temp;
  for(int i=2;i<folders.size();i++)
  {
    temp.push_back(folders[i]);
  }
  folders = temp;
}

int is_regular_file(const char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}
void read_directory(const std::string& name, stringvec& v)
{
    DIR* dirp = opendir(name.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
      if(dp->d_name[0] != '.')
        v.push_back(dp->d_name);
    }
    closedir(dirp);
}



int calculate_folder_num(string path , stringvec& folders_files)
{
  stringvec v;
  int folder_num = 0;
  read_directory(path, v);
  for(int i=0;i<v.size();i++)
  {
    //if(is_regular_file((path + "/" + v[i]).c_str()) == 0)
    //{
      folder_num++;
      folders_files.push_back(v[i]);
    //}

  }
  return folder_num;
}
void search_for_fine(string car_id, string file_path, int p)
{
  string line;
  int fines_sum = 0;
  ifstream fine_file;
  //cout<<"File_Path="<<file_path<<endl;
  fine_file.open(file_path);
  while(!fine_file.eof())
  {
    getline(fine_file ,line );
    string part1;
    bool id_checked = false;
    bool second_delimiter = false;
    string fine;
    for(int j=0;j<line.length();j++)
    {
      if(id_checked == false)
      {
        if(line [j] == ' ')
          id_checked = true;
        else
          part1 += line[j];
      }
      else
      {
        if( part1 != car_id)
          break;
        if(line[j] == ' ')
        {
          second_delimiter = true;
          continue;
        }
        if(second_delimiter == true)
        {
          fine += line[j];
        }

      }
    }
    if(id_checked && second_delimiter)
    {
      fines_sum+=(atoi(fine.c_str()));
      //cout << "***************************fine=" << fine<<endl;
    }
    //cout <<"FILEINFO: " <<line << endl;
  }
  fine_file.close();
  //cout<<"^^^^^^^^^^^^^^^^^^^^^^^^Fines_Sum_In_File= "<< fines_sum << endl;
  write(p, to_string(fines_sum).c_str(), 1000);
}
void inquiry (string car_id, string folder_path, int pz)
{
  pid_t pid;
  //int p[2];
  stringvec folders_files;
  int folder_file_num = calculate_folder_num(folder_path , folders_files) ;
  int p [folder_file_num][2];
  //cout << "Folder Num: "<<folder_num<<endl;
  //adjust_folders(folders);

  for(int i=0;i<folder_file_num;i++)
  {
    pipe(p[i]);
    //cout << folders_files[i] << endl;
    pid = fork();
    if(pid < 0)
    {
      fprintf(stderr, "Fork Failed");
      return;
    }
    if(pid == 0)//Child Process
    {
      if(is_regular_file((folder_path + "/" + folders_files[i]).c_str()) == 0)//Folder
      {
        inquiry(car_id , folder_path + "/" + folders_files[i] , p[i][1]);
      }
      else//File
       {
         search_for_fine(car_id , folder_path+ "/" + folders_files[i], p[i][1]);
       }
      exit(0);

    }
    else
    {
      //wait(NULL);

      //cout << "Child has completed" << endl;
    }
  }

   //string data ;
   int final_sum = 0;
  for(int k=0;k<folder_file_num;k++)
  {
    //cout<<"WAIitng"<<endl;
    wait(NULL);
  }

  //sleep(1);
  for(int k=0;k<folder_file_num;k++)
  {
    char buf[100];
    memset(buf, '\0', 100);
    //cout<<"pk="<<p[k][0]<<"...."<<p[k][1]<<endl;
    read(p[k][0] ,buf , 100);

    //cout<<"buf="<<buf << endl;
    //data += buf;
    final_sum += atoi(buf);
    //cout<<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ : "<<final_sum<<endl;

  }
  //cout << "Data:" <<data << endl;
  //cout<<"MY LAST SUM="<<final_sum<<endl;
  write(pz , to_string(final_sum).c_str() , 100 );
  //cout<<"going back to rest from INQUIRY"<<endl;

}
void named_pipe(int* sv)
{
  string myfifo = "/tmp/pjct";
  //int fd = open(myfifo.c_str(), O_RDONLY);
  while(true)
  {


    //mkfifo(myfifo.c_str(), 0666);
    int fd = open(myfifo.c_str(), O_RDONLY);
    char buf[100];
    memset(buf, '\0', 100);
    //cout <<"WE are in named pipe"<<endl;

    read(fd , buf , 100);
    //cout << "NAMEDPIPE = " << buf << endl;
    if(string(buf) == "EXIT") {
      exit(0);
    }
    int t = 0;
    char temp[100];
    memset(temp, '\0', 100);
    sock_fd_read(sv[0] , temp , 1 , &t);
    send(t , buf , 100 , 0);
    cout<<"PACKET is:"<<buf<<endl;
    cout<<"PACKET SENT!"<<endl;

    close (fd);

  }

}
void intermediate(string path , int* p , string buff)
{
  //cout<<"We are in intermeditae"<<endl;
  char plate[100];
  memset(plate, '\0', 100);

  read(p[0], plate , 100 );
  //sleep(2);
  string s_plate = plate;
  //cout<<"PLATE="<<s_plate<<endl;

  string myfifo = "/tmp/pjct";
  //cout <<"Going to INQ0"<<endl;
  int fd = open(myfifo.c_str(), O_WRONLY);
  //int fd = open("/temp/university", O_WRONLY);
  //cout <<"Going to INQ1"<<endl;
  int pz[2];
  //cout <<"Going to INQ2"<<endl;
  pipe(pz);
  //cout <<"Going to INQ3"<<endl;
  inquiry(s_plate,"./Test" , pz[1]);
  //inquiry(buff,"./Test" , pz[1]);
  char buf[100];
  memset(buf, '\0', 100);
  read(pz[0] , buf , 100);
  //cout <<"INTER=" << buf << endl;
  write(fd , buf , 1000);
  close(fd);
}
int main()
{
  int sv[2];
    int pid;

    if (socketpair(AF_LOCAL, SOCK_STREAM, 0, sv) < 0) {
        perror("socketpair");
        exit(1);
    }

  string myfifo = "/tmp/pjct";
  mkfifo(myfifo.c_str(), 0666);
  cout<<"P2 Created"<<endl;
  if(!fork())
    named_pipe(sv);

    //*********************Server Start
    fd_set master;    // master file descriptor list
      fd_set read_fds;  // temp file descriptor list for select()
      int fdmax;        // maximum file descriptor number

      int listener;     // listening socket descriptor
      int newfd;        // newly accept()ed socket descriptor
      struct sockaddr_storage remoteaddr; // client address
      socklen_t addrlen;

      char buf[256];    // buffer for client data
      int nbytes;

      char remoteIP[INET6_ADDRSTRLEN];

      int yes=1;        // for setsockopt() SO_REUSEADDR, below
      int i, j, rv;

      struct addrinfo hints, *ai, *p;

      FD_ZERO(&master);    // clear the master and temp sets
      FD_ZERO(&read_fds);

      // get us a socket and bind it
      memset(&hints, 0, sizeof hints);
      hints.ai_family = AF_UNSPEC;
      hints.ai_socktype = SOCK_STREAM;
      hints.ai_flags = AI_PASSIVE;
      if ((rv = getaddrinfo(NULL, "9090", &hints, &ai)) != 0) {
          fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
          exit(1);
      }

      for(p = ai; p != NULL; p = p->ai_next) {
          listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
          if (listener < 0) {
              continue;
          }

          // lose the pesky "address already in use" error message
          setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

          if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
              close(listener);
              continue;
          }

          break;
      }

      // if we got here, it means we didn't get bound
      if (p == NULL) {
          fprintf(stderr, "selectserver: failed to bind\n");
          exit(2);
      }

      freeaddrinfo(ai); // all done with this

      // listen
      if (listen(listener, 10) == -1) {
          perror("listen");
          exit(3);
      }

      // add the listener to the master set
      FD_SET(listener, &master);
      FD_SET(0, &master);

      // keep track of the biggest file descriptor
      fdmax = listener; // so far, it's this one

      // main loop
      for(;;) {
          read_fds = master; // copy it
          if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
              perror("select");
              exit(4);
          }

          // run through the existing connections looking for data to read
          for(i = 0; i <= fdmax; i++) {
              if (FD_ISSET(i, &read_fds)) { // we got one!!
                  if(i == 0) {
                    string input;
                    cin >> input;

                    if(input == "quit") {
                      //cout << "tamom!" << endl;
                      int fd = open("/tmp/pjct", O_WRONLY);
                      write(fd, "EXIT", 5);
                      close(fd);
                      exit(0);
                    }
                    else
                      continue;

                    // wait(NULL);
                    exit(0);
                  }
                  else if (i == listener) {
                      // handle new connections
                      addrlen = sizeof remoteaddr;
                      newfd = accept(listener,
                          (struct sockaddr *)&remoteaddr,
                          &addrlen);

                      if (newfd == -1) {
                          perror("accept");
                      } else {
                          FD_SET(newfd, &master); // add to master set
                          if (newfd > fdmax) {    // keep track of the max
                              fdmax = newfd;
                          }
                          printf("selectserver: new connection from %s on "
                              "socket %d\n",
                              inet_ntop(remoteaddr.ss_family,
                                  get_in_addr((struct sockaddr*)&remoteaddr),
                                  remoteIP, INET6_ADDRSTRLEN),
                              newfd);
                      }
                  } else {
                      // handle data from a client
                      if ((nbytes = recv(i, buf, sizeof buf, 0)) <= 0) {
                          // got error or connection closed by client
                          if (nbytes == 0) {
                              // connection closed
                              printf("selectserver: socket %d hung up\n", i);
                          } else {
                              perror("recv");
                          }
                          close(i); // bye!
                          FD_CLR(i, &master); // remove from master set
                      } else {
                          // we got some data from a client
                          cout<<"Plate Received: "<<buf <<endl;
                          int pipe_for_p1[2];
                          pipe(pipe_for_p1);
                          write(pipe_for_p1[1] , buf , 100);
                          if(!fork()) {
                            cout<<"P1 created"<<endl;

                            char temp[100];
                            memset(temp , '\0' , 100);
                            sock_fd_write(sv[1],temp,12,i);
                            string buff = buf;
                            //cout<<"GOING to interm"<<endl;
                            intermediate("." , pipe_for_p1 , buff);
                            exit(0);
                          }
                      }
                  } // END handle data from client
              } // END got new incoming connection
          } // END looping through file descriptors
      } // END for(;;)--and you thought it would never end!


  //string myfifo = "/tmp/pjct";
  //cout <<"1"<<endl;
  // int fd = open(myfifo.c_str(), O_WRONLY);
  //
  // int pz[2];
  // pipe(pz);
  // inquiry("1234","./Test" , pz[1]);
  // char buf[100];
  // memset(buf, '\0', 100);
  // read(pz[0] , buf , 100);
  // write(fd , buf , 100);
  // close(fd);
  //named_pipe();
  //cout<<"lastlastlastlastlast: "<<buf<<endl;
}
