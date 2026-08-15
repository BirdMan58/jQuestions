#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <ifaddrs.h>
#include <arpa/inet.h>

#define BUFFER_LEN 1024
#define PORT 8080

int server_socket;
char ip[INET_ADDRSTRLEN];

void gracefullShutdown(int num);
void printLinks();
void handleGet(char *route, int sock);
void handlePost(char *request, char *route, int sock);
void saveSurvey(char *payload);
void sendSurveyList(int sock);
int sendQuestion(char *title, int sock);

int main() {
    signal(SIGINT, gracefullShutdown);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket < 0) {
        perror("Error: Couldn't create a socket");
        return -1;
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
        perror("Error: Binding failed");
        return -1;
    }

    if(listen(server_socket, 5) < 0) {
        perror("Error: Listening failed");
        return -1;
    }

    printLinks();

    while(1) {
        struct sockaddr_in client_address;
        socklen_t client_len = sizeof(client_address);
        int client_socket = accept(server_socket, (struct sockaddr*) &client_address, &client_len);
        if(client_socket < 0) {
            perror("Error: Couldn't accept client");
            return -1;
        }

        char recv_buffer[BUFFER_LEN] = {0};
        char recv_buffer_copy[BUFFER_LEN] = {0};

        size_t recived_bytes = recv(client_socket, recv_buffer, BUFFER_LEN, 0);
        memcpy(recv_buffer_copy, recv_buffer, sizeof(recv_buffer));
        char *terminalMessage = strtok(recv_buffer_copy, "\n");
        printf("%s\n", terminalMessage);

        char *request = strtok(recv_buffer_copy, " ");

        if(strcmp(request, "GET") == 0) {
            char *token = recv_buffer + 4;
            char *route = strtok(token, " ");
            handleGet(route, client_socket);
        } else if(strcmp(request, "POST") == 0) {
            char *token = recv_buffer_copy + 5;
            char *route = strtok(token, " ");
            handlePost(recv_buffer, route, client_socket);
        }

        close(client_socket);
    }

    return 0;
}

void gracefullShutdown(int sig) {
    close(server_socket);
    write(STDOUT_FILENO, "\nServer shutdown gracefully\n", 28);
    _exit(0);
}

void printLinks(){
    struct ifaddrs *ifaddr;

    if(getifaddrs(&ifaddr) == -1) {
        printf("Failed to get links.\n");
        printf("Open http://localhost:%d to see the web page\n", PORT);
        return;
    }
    printf("Use Ctl + click to click:\n");
    printf("\thttp://localhost:%d\n", PORT);
    for(struct ifaddrs *ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if(ifa->ifa_addr == NULL) {
            continue;
        }

        if(ifa->ifa_addr->sa_family == AF_INET) {
            struct sockaddr_in *sa = (struct sockaddr_in *) ifa->ifa_addr;
            inet_ntop(AF_INET, &sa->sin_addr, ip, sizeof(ip));

            printf("\thttp://%s:%d\n", ip, PORT);
        }
    }
    printf("\n");
    freeifaddrs(ifaddr);
}

void handleGet(char *route, int sock) {

    char* filename;
    char* header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    if(strcmp(route, "/") == 0){
        filename = "./frontEnd/home.html";
    } else if(strcmp(route, "/style.css") == 0) {
        filename = "./frontEnd/style.css";
        header = "HTTP/1.1 200 OK\r\nContent-Type: text/css\r\n\r\n";
    } else if(strcmp(route, "/AddSurvey") == 0) {
        filename = "./frontEnd/addQuestions.html";
    } else if(strcmp(route, "/ViewSurvey") == 0) {
        filename = "./frontEnd/foam.html";
    }else if(strcmp(route, "/favicon.png") == 0) {
        filename = "./frontEnd/favicon.png";
        header = "HTTP/1.1 200 OK\r\nContent-Type: image/png,image\r\n\r\n";
    } else if(strcmp(route, "/Test") == 0) {
        sendSurveyList(sock);
        return;
    } else if(strcmp(route, "/getIp") == 0) {
        send(sock, header, strlen(header), 0);
        send(sock, ip, strlen(ip), 0);
        return;
    } else {
        char routeCpy[strlen(route)];
        strcpy(routeCpy, route);
        strtok(routeCpy, "/");

        if(strcmp(routeCpy, "/Question") == 0) {
            char* title = &route[strlen("/Question/")];
            if(sendQuestion(title, sock)) {
                return;
            }
        }
        if(strcmp(routeCpy, "/Share") == 0) {
            filename = "./frontEnd/foam.html";
        } else {
            filename = "./frontEnd/notFound.html";
            header = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n";
        }
    }
    
    FILE *fd = fopen(filename, "r");
    if(!fd) {
        perror("Error: Couldn't open the file");
    }
    char buf[BUFFER_LEN] = {0};
    size_t read = 0;

    send(sock, header, strlen(header), 0);

    while((read = fread(buf, sizeof(buf[0]), BUFFER_LEN, fd)) > 0) {
        send(sock, buf, read, 0);
    }

    fclose(fd);
}

void handlePost(char *request, char *route, int sock) {
    int request_len = strlen(request);
    char *json = NULL;
    printf("Length of the request is: %d, and the route is %s\n", request_len, route);

    for(int i = 0; i < request_len-4; i++) {
        if(request[i] == '\r' && request[i+1] == '\n' && request[i+2] == '\r' && request[i+3] == '\n') {
            json = &request[i+4];
            break;
        }
    }

    if(json == NULL) {
        return;
    }

    if(strcmp(route, "/Test") == 0) {
        saveSurvey(json);
    }
    char *data = "HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\nSurvey successfully added to the server.";
    send(sock, data, strlen(data), 0);

}

void saveSurvey(char *payload) {
    char title[200];

    int offset = strlen("[{\"title\":\"");
	
	int i;
	for(i = 0; payload[offset] != '\"'; i++) {
		title[i] = payload[offset++];
    }
    title[i] = '\0';

    FILE *fd = fopen("./data/index.txt", "a");
    if(fd == NULL) {
        perror("Error: can't create the fd.\n");
    }

    fprintf(fd, "\"%s\"\n", title);
    fclose(fd);

    char file_path[strlen("./data/questions/") + strlen(title) + strlen(".json") + 1];
    snprintf(file_path, sizeof(file_path), "./data/questions/%s.json", title);

    fd = fopen(file_path, "w");

    if(fd == NULL) {
        perror("Error: can't create the fd.\n");
    }
    printf("Payload: %s\n", payload);

    size_t count = fwrite(payload, sizeof(char), strlen(payload), fd);

    fclose(fd);
    printf("File saved in %s(%d)\n", file_path, count);
}

void sendSurveyList(int sock) {
    char *file_path = "./data/index.txt";
    char *header = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n[";

    FILE *fd = fopen(file_path, "r");

    if(fd == NULL) {
        perror("Error: Can't open the file.\n");
    }

    char buffer[BUFFER_LEN] = {0};
    size_t read;
    send(sock, header, strlen(header), 0);

    while((read = fread(buffer, sizeof(buffer[0]), BUFFER_LEN, fd)) > 0) {
        int i;
        for(i = 0; i < read; i++) {
            if(buffer[i] == '\n') {
                buffer[i] = ',';
            }
        }
        if(feof(fd)) {
            buffer[i-1] = ']';
        }

        send(sock, buffer, read, 0);
    }

    fclose(fd);

}

int sendQuestion(char *raw_title, int sock) {
    char *header = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n";
    char title[strlen(raw_title)];

    int i = 0;
    for(int j = 0; raw_title[j] != '\0'; j++) {
        if(raw_title[j] == '%') {
            title[i] = ' ';
            j += 2;
        } else {
            title[i] = raw_title[j];
        }
        i++;
    }
    title[i] = '\0';
    
    char file_name[strlen("./data/questions/") + strlen(title) + strlen(".json") + 1];
    snprintf(file_name, sizeof(file_name), "./data/questions/%s.json", title);
    
    FILE *fd = fopen(file_name, "r");
    if(fd == NULL) {
        printf("Requested an non-existing page!\n");
        return 0;
    }

    send(sock, header, strlen(header), 0);

    size_t read;
    char buffer[BUFFER_LEN];
    while((read = fread(buffer, sizeof(buffer[0]), BUFFER_LEN, fd)) > 0) {
        send(sock, buffer, read, 0);
    }

    fclose(fd);

    return 1;
}

