#include <iostream>
#include <string>
#include <sys/socket.h>

#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

constexpr int PORT = 8083;

int main() {
  // create socket for local network on TCP with IP as protocol
  int serverSocket{socket(AF_INET, SOCK_STREAM, 0)};

  if (serverSocket == -1) {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  sockaddr_in serverAddress = {
      .sin_family = AF_INET, .sin_port = htons(PORT), .sin_addr = INADDR_ANY};

  if (bind(serverSocket, reinterpret_cast<sockaddr *>(&serverAddress),
           sizeof(serverAddress)) == -1) {
    std::cerr << "Error binding socket" << std::endl;
    close(serverSocket);
    return 1;
  }

  if (listen(serverSocket, SOMAXCONN) == -1) {
    std::cerr << "Error listening on socket" << std::endl;
    close(serverSocket);
    return -1;
  }

  std::cout << "Server listening on port " << PORT << std::endl;

  // accept client connections
  while (true) {
    sockaddr_in clientAddress{};
    socklen_t clientAddrSize = sizeof(clientAddress);
    int clientSocket =
        accept(serverSocket, reinterpret_cast<sockaddr *>(&clientAddress),
               &clientAddrSize);

    if (clientSocket == -1) {
      std::cerr << "Error accepting connection" << std::endl;
      continue;
    } else {
      std::cout << "Accepted connection!!" << std::endl;
      continue;
    }
  }
  return 0;
}
