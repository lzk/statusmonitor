#ifndef TRANS_H
#define TRANS_H

typedef int (* CALLBACK_Server)(void* ,char* ,int);

class Trans_Server
{
public:
    Trans_Server();
    ~Trans_Server();
    int createServer();
    int readThenWrite(CALLBACK_Server callback ,void* para);

private:
    int listen_fd;
};

class Trans_Client
{
public:
    Trans_Client();
    int writeThenRead(char* buffer ,int bufsize);
    int tryConnectToServer();
};
#endif // TRANS_H
