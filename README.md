
# DNS Lookup Project

This project implements a DNS lookup system where a client requests website data from a local server. If the domain and its IP address are not in the local server cache, the local server queries the root server, top-level domain server, and authoritative server to obtain the IP address information. Finally, the local server returns the IP address to the client.

## Prerequisites

Before running this project, ensure that you have the following:

- C compiler (GCC or any C compiler)
- Development environment with necessary libraries (e.g., `stdio.h`, `stdlib.h`, `sys/socket.h`, `netdb.h`, etc.)

## Project Structure

The project consists of several components:

- `client.c`: Implements the client side to send DNS lookup requests to the local server.
- `local_server.c`: Acts as the local server, receiving client requests, querying other servers, and responding back to the client.
- `root_server.c`: Represents the root server that receives queries from the local server and forwards them to the top-level domain server.
- `top_level_domain_server.c`: Handles queries from the root server and forwards them to the authoritative server.
- `authoritative_server.c`: Receives queries from the top-level domain server and provides the IP address information.

## Running the Project

To run the project, follow these steps:

1. Compile each component separately using the C compiler.
2. Start the authoritative server first, followed by the top-level domain server, root server, and finally the local server.
3. Run the client program and provide the domain name as input.
4. The client will send the request to the local server, which will process the DNS lookup and return the IP address to the client.

**Note:** Make sure to run the servers in the order mentioned above, as each server depends on the previous one.

## Example Usage

Here's an example usage of the DNS lookup project:

1. Start the authoritative server: `./authoritative_server`
2. Start the top-level domain server: `./top_level_domain_server`
3. Start the root server: `./root_server`
4. Start the local server: `./local_server`
5. Run the client program: `./client`
6. Enter the domain name for which you want to perform a DNS lookup.
7. The client will display the IP address returned by the local server.

## Additional Notes

- The project uses hardcoded domain-IP mappings in the servers for demonstration purposes. In a real-world scenario, these mappings would be obtained from DNS records or a database.
- The project uses UDP sockets for communication between the client and servers. Ensure that the necessary ports are not blocked by firewalls or other security measures.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

- The project is inspired by the DNS lookup process and aims to provide a simplified implementation.
- Thanks to the authors and contributors of the libraries and resources used in this project.

