#ifndef _HTTPD_H___
#define _HTTPD_H___

#include <string.h>
#include <stdio.h>

// FUNCTIONS TO CONTROL THE HTTP SERVER
void serve_forever(const char *PORT);

// external variables to handle Client request.
extern char *method; // will obtain "GET" or "POST"
extern char *uri;    // will obtain "/index.html" and things before '?'
extern char *qs;    // will obtain the query parameters such as "a=1&b=2" and things after '?'
extern char *prot; // will obtain the protocol name, such as "HTTP/1.1".

extern char *payload;     // Holds the request body for POST requests.
extern int payload_size; // Stores the size of the POST request payload.

// Function to fetch a specific request header value by name.
char *request_header(const char* name); 

// Function to parse HTTP request data.
void analyze_http(char* buf,int rcvd);

// Function to define custom routes - to be implemented by the user.
void route();


// Macros that define a simple routing mechanism in C.
#define ROUTE_START()       if (0) { // is simply a trick to begin a chain of } else if (...) { … }.

// Defines an else-if condition that checks if `method` and `uri` match the arguments
#define ROUTE(METHOD,URI)   } else if (strcmp(URI, uri) == 0 && strcmp(METHOD, method) == 0) {

// a GET macro shortcut that for a nother macro shortcut to a ROUTE func. calls the ROUTE macro which is a macro to a function as well, it calls ROUTE func with a "GET" as first parameter, and URI that the client sent as second parameter.("GET", URI).
#define ROUTE_GET(URI)      ROUTE("GET", URI)

// a GET macro shortcut that for a nother macro shortcut to a ROUTE func. calls the ROUTE macro which is a macro to a function as well, it calls ROUTE func with a "POST" as first parameter, and URI that the client sent as second parameter.("POST", URI).
#define ROUTE_POST(URI)     ROUTE("POST", URI)

// Ends the chain of else-if routes. If no routes matched, respond with 500.
#define ROUTE_END()         } else printf(\
                                "HTTP/1.1 500 Not Handled\r\n\r\n" \
                                "The server has no handler to the request.\r\n" \
                            );

#endif




