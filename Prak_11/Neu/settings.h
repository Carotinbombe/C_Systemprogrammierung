#define BUFFERSIZE 1024
#define SOCKET_PATH "/tmp/mysocket"
#define DATA_PATH 1
#define DATA_SUCC 2
#define DATA_FAIL -1
#define DATA_SEND 3
#define QUEUE_VALUE 5

struct SocketMessage{
    int msg_typ;
    char msg_text[BUFFERSIZE];
    ssize_t msg_size;
};