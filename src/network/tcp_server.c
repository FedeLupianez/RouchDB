#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <common/functions.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <sys/types.h>

#define PORT 9090
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

typedef enum {
    LOGIN,
    INSERT,
    SELECT,
    DELETE,
    UPDATE,
    RESPONSE,
    ERROR

} Command;
typedef struct {
    uint8_t size;
    Command comm;
} Package;

typedef struct {
    uint32_t fd;
    char buffer[1024];
    uint32_t buffer_len;
} Client;

int set_non_blocking(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);

    if (flags == -1)
        return -1;

    return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

int main()
{
    int server_fd;
    struct sockaddr_in addr;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        logging("ERROR", "Error creating server");
        exit(1);
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        logging("ERROR", "error bindind");
        exit(1);
    }

    if (listen(server_fd, SOMAXCONN) < 0) {
        logging("ERROR", "Error listening");
        exit(1);
    }

    set_non_blocking(server_fd);

    int epoll_fd = epoll_create1(0);

    if (epoll_fd == -1) {
        logging("ERROR", "Error creating epoll1");
        exit(1);
    }

    struct epoll_event event;

    event.events = EPOLLIN;
    event.data.fd = server_fd;
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

    logging("SETUP", "Server Listening");
    struct epoll_event events[MAX_EVENTS];

    while (1) {
        int n = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (n == -1) {
            logging("ERROR", "error with epoll_wait");
            break;
        }

        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;
            if (fd == server_fd) {
                while (1) {
                    int client_fd = accept(server_fd, NULL, NULL);
                    if (client_fd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        logging("ERROR", "Error reading client");
                        break;
                    }

                    set_non_blocking(client_fd);
                    struct epoll_event client_event;
                    client_event.events = EPOLLIN;
                    client_event.data.fd = client_fd;

                    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &client_event);
                    logging("CONNECTION", "client connected");
                }
            } else if (events[i].events & EPOLLIN) {
                char buffer[BUFFER_SIZE];
                while (1) {
                    int count = recv(fd, buffer, sizeof(buffer), 0);

                    if (count > 0) {
                        // process data
                    }

                    if (count == 0) {
                        logging("CLOSE", "Client closed");
                        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
                        close(fd);
                    }

                    if (count < 0) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        logging("ERROR", "Error receibing data");
                        close(fd);
                        break;
                    }

                    send(fd, buffer, count, 0);
                }
            }
        }
    }

    close(server_fd);
    return 0;
}
