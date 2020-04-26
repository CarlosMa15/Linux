#include "csapp.h"
#include "dictionary.h"
#include "more_string.h"

static void doit(void *connfd_p);
static dictionary_t *read_requesthdrs(rio_t *rp);
static void read_postquery(rio_t *rp, dictionary_t *headers, dictionary_t *d);
static void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg);
static void print_stringdictionary(dictionary_t *d);
static void serve_request(int fd, dictionary_t *query, char *data);
static int dictionarySize(dictionary_t *dic);
static int peopleNumber = 0;
static int placesNumber = 0;
static char *myPort;
static pthread_mutex_t lock;
static char *bad_header(size_t len, const char *content_type);
static int badRequest(dictionary_t * query, char *data);
dictionary_t *peopleDictionary;
dictionary_t *placesDictionary;

int main(int argc, char **argv) 
{

  if(pthread_mutex_init(&lock,NULL)) {
    exit(1);
  }

  peopleDictionary = make_dictionary(COMPARE_CASE_SENS, dictionary_free);
  placesDictionary = make_dictionary(COMPARE_CASE_SENS, dictionary_free);
  int listenfd, connfd;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;

  /* Check command line args */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  myPort = argv[1];
  listenfd = Open_listenfd(argv[1]);

  exit_on_error(0);

  /* Also, don't stop on broken connections: */
  Signal(SIGPIPE, SIG_IGN);

  while (1) {
    clientlen = sizeof(clientaddr);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    if (connfd >= 0) {
      Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                  port, MAXLINE, 0);
      printf("Accepted connection from (%s, %s)\n", hostname, port);

      pthread_t th;
      int *connfd_p = malloc(sizeof(int));
      *connfd_p = connfd;
      Pthread_create(&th, NULL, doit, connfd_p);
      Pthread_detach(th);
    }
  }
}

void doit(void *connfd_p) 
{
  int fd = *(int *)connfd_p;
  char buf[MAXLINE], *method, *uri, *version;
  rio_t rio;
  dictionary_t *headers, *query;

  free(connfd_p);

  /* Read request line and headers */
  Rio_readinitb(&rio, fd);
  if (Rio_readlineb(&rio, buf, MAXLINE) <= 0)
    return;
  printf("%s", buf);
  
  if (!parse_request_line(buf, &method, &uri, &version)) {
    clienterror(fd, method, "400", "Bad Request",
                "Redpin did not recognize the request");
  } else {
    if (strcasecmp(version, "HTTP/1.0")
        && strcasecmp(version, "HTTP/1.1")) {
      clienterror(fd, version, "501", "Not Implemented",
                  "Redpin does not implement that version");
    } else if (strcasecmp(method, "GET")
               && strcasecmp(method, "POST")) {
      clienterror(fd, method, "501", "Not Implemented",
                  "Redpin does not implement that method");
    } else {
      headers = read_requesthdrs(&rio);

      /* Parse all query arguments into a dictionary */
      query = make_dictionary(COMPARE_CASE_SENS, free);
      parse_uriquery(uri, query);
      if (!strcasecmp(method, "POST"))
        read_postquery(&rio, headers, query);

      pthread_mutex_lock(&lock);
      serve_request(fd, query,uri);
      pthread_mutex_unlock(&lock);

      /* Clean up */
      dictionary_free(query);
      dictionary_free(headers);
    }

    /* Clean up status line */
    free(method);
    free(uri);
    free(version);
  }
  Close(fd);
}

dictionary_t *read_requesthdrs(rio_t *rp) 
{
  char buf[MAXLINE];
  dictionary_t *d = make_dictionary(COMPARE_CASE_INSENS, free);

  Rio_readlineb(rp, buf, MAXLINE);
  printf("%s", buf);
  while(strcmp(buf, "\r\n")) {
    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    parse_header_line(buf, d);
  }
  
  return d;
}

void read_postquery(rio_t *rp, dictionary_t *headers, dictionary_t *dest)
{
  char *len_str, *type, *buffer;
  int len;
  
  len_str = dictionary_get(headers, "Content-Length");
  len = (len_str ? atoi(len_str) : 0);

  type = dictionary_get(headers, "Content-Type");
  
  buffer = malloc(len+1);
  Rio_readnb(rp, buffer, len);
  buffer[len] = 0;

  if (!strcasecmp(type, "application/x-www-form-urlencoded")) {
    parse_query(buffer, dest);
  }

  free(buffer);
}

static char *ok_header(size_t len, const char *content_type) {
  char *len_str, *header;
  
  header = append_strings("HTTP/1.0 200 OK\r\n",
                          "Server: Redpin Web Server\r\n",
                          "Connection: close\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n",
                          "Content-type: ", content_type, "\r\n\r\n",
                          NULL);
  free(len_str);

  return header;
}

void clienterror(int fd, char *cause, char *errnum, 
     char *shortmsg, char *longmsg) 
{
  size_t len;
  char *header, *body, *len_str;

  body = append_strings("<html><title>Redpin Error</title>",
                        "<body bgcolor=""ffffff"">\r\n",
                        errnum, " ", shortmsg,
                        "<p>", longmsg, ": ", cause,
                        "<hr><em>Redpin Server</em>\r\n",
                        NULL);
  len = strlen(body);

  /* Print the HTTP response */
  header = append_strings("HTTP/1.0 ", errnum, " ", shortmsg, "\r\n",
                          "Content-type: text/html; charset=utf-8\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n\r\n",
                          NULL);
  free(len_str);
  
  Rio_writen(fd, header, strlen(header));
  Rio_writen(fd, body, len);

  free(header);
  free(body);
}

static void print_stringdictionary(dictionary_t *d)
{
  int i;
  const char **keys;

  keys = dictionary_keys(d);
  
  printf("%s\n", "dictionary!!!");
  for (i = 0; keys[i] != NULL; i++) {
    printf("%s=%s\n",
           keys[i],
           (const char *)dictionary_get(d, keys[i]));
  }
  printf("\n");

  free(keys);
}

static int dictionarySize(dictionary_t *dic) {
  int i, counter = 0;
  const char **keys;
  keys = dictionary_keys(dic);
  for (i = 0; keys[i] != NULL; i++) {
    counter++;
  }
  free(keys);
  return counter;
}

static char *bad_header(size_t len, const char *content_type) {
  char *len_str, *header;
  
  header = append_strings("HTTP/1.0 400 Bad request\r\n",
                          "Server: Redpin Web Server\r\n",
                          "Connection: close\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n",
                          "Content-type: ", content_type, "\r\n\r\n",
                          NULL);
  free(len_str);

  return header;
}

static int badRequest(dictionary_t * query, char *data) {

  if(starts_with("/bad",data)) return 1;
  if(starts_with("/pin",data) && dictionary_count(query) < 2) return 1;
  if(starts_with("/unpin",data) && dictionary_count(query) < 2) return 1;
  if(starts_with("/copy",data) && dictionary_count(query) < 4) return 1;
  return 0;
}

static void serve_request(int fd, dictionary_t *query, char *data)
{
  int i,j;
  size_t len;
  char *body, *header;

  printf("Data: %s\n", data);
  print_stringdictionary(query);

  if(badRequest(query,data)) {
    printf("%s\n", "/bad");
    body = strdup("\n");

    len = strlen(body);

  /* Send response headers to client */
    header = bad_header(len, "text/html; charset=utf-8");
    Rio_writen(fd, header, strlen(header));
    printf("Response headers:\n");
    printf("%s", header);

    free(header);

    /* Send response body to client */
    Rio_writen(fd, body, len);

    free(body);
    return;
  }

  if(starts_with("/counts",data)) { 
    char number1[20];
    char number2[20];
    sprintf(number1, "%d", peopleNumber);
    sprintf(number2, "%d", placesNumber);
    char *temp = append_strings(number1,"\n",number2,"\n");
    body = strdup(temp);
    free(temp);

  } else if(starts_with("/reset",data)) { 
    peopleNumber = 0;
    placesNumber = 0;

    const char **keys;
    keys = dictionary_keys(peopleDictionary);
    printf("%s\n", "remove people dictionary!!!");
    for (i = 0; keys[i] != NULL; i++) {
      dictionary_remove(peopleDictionary, keys[i]);
    }
    printf("\n");
    free(keys);

    const char **keys1;
    keys1 = dictionary_keys(placesDictionary);
    printf("%s\n", "remove places dictionary!!!");
    for (i = 0; keys1[i] != NULL; i++) {
      dictionary_remove(placesDictionary, keys1[i]);
    }
    printf("\n");
    free(keys1);
    
    char number1[20];
    char number2[20];
    sprintf(number1, "%d", peopleNumber);
    sprintf(number2, "%d", placesNumber);
    char *temp = append_strings(number1,"\n",number2,"\n");
    body = strdup(temp);
    free(temp);

  } else if(starts_with("/people",data)) { 

  int size = dictionary_count(peopleDictionary);
  if(size == 0 ) {
    printf("%s\n", "No People");
    body = strdup("");
  } else {
    if(dictionarySize(query)) {
      const char **keys = dictionary_keys(query);
      if(dictionary_has_key(placesDictionary, (const char *)dictionary_get(query, keys[0]))) {
        const char **peopleElements = dictionary_keys(dictionary_get(placesDictionary,
          (const char *)dictionary_get(query, keys[0])));

        char * answer[dictionarySize(dictionary_get(placesDictionary,
          (const char *)dictionary_get(query, keys[0]))) + 1];
        for (i = 0; peopleElements[i] != NULL; i++) {
          answer[i] = peopleElements[i];
        }
        free(peopleElements);
        answer[i] = NULL;
        printf("%s\n", "place exist");
        char * temp = join_strings(answer, '\n');
        body = strdup(temp);
        free(temp);
      } else {
        printf("%s\n", "place does not exist");
        body = strdup("");
      }
      free(keys);
    } else {
      const char **keys1;
        keys1 = dictionary_keys(peopleDictionary);
        char *answer[size+1];
        for (i = 0; keys1[i] != NULL; i++) {
          answer[i] = keys1[i];
        }
        answer[i] = NULL;
        char *temp = join_strings(answer,'\n');
        body = strdup(temp);
        free(temp);
        free(keys1);
    }
  }

  } else if(starts_with("/places",data)) { 
    
  int size = dictionarySize(placesDictionary);
  if(size == 0 ) {
    printf("%s\n", "No places");
    body = strdup("");
  } else {
    if(dictionarySize(query)) {
      const char **keys = dictionary_keys(query);
      if(dictionary_has_key(peopleDictionary, (const char *)dictionary_get(query, keys[0]))) {
        const char **placesElements = dictionary_keys(dictionary_get(peopleDictionary,
          (const char *)dictionary_get(query, keys[0])));

        char * answer[dictionarySize(dictionary_get(peopleDictionary,
          (const char *)dictionary_get(query, keys[0]))) + 1];
        for (i = 0; placesElements[i] != NULL; i++) {
          answer[i] = placesElements[i];
        }
        free(placesElements);
        answer[i] = NULL;
        printf("%s\n", "people exist");
        char *temp1 = join_strings(answer, '\n');
        printf("ANSWER: %s\n", temp1);
        body = strdup(temp1);
        free(temp1);
      } else {
        printf("%s\n", "people does not exist");
        body = strdup("");
      }
      free(keys);
    } else {
        const char **keys;
        keys = dictionary_keys(placesDictionary);
        char *answer[size+1];
        for (i = 0; keys[i] != NULL; i++) {
          answer[i] = keys[i];
        }
        answer[i] = NULL;
        char * temp = join_strings(answer,'\n');
        printf("ANSWER: %s\n", temp);
        body = strdup(temp);
        free(temp);
        free(keys);
    }
  }

  } else if(starts_with("/pin",data)) { 

    //Updates people
  const char **peopleElement = split_string((const char *)dictionary_get(query, "people"), '\n');
  for (j = 0; peopleElement[j] != NULL; j++) {
      printf("People: %s\n", peopleElement[j]);

      if(!dictionary_has_key(peopleDictionary, peopleElement[j])) {
        printf("%s\n", "New People added!!!");
        dictionary_set(peopleDictionary, peopleElement[j], make_dictionary(COMPARE_CASE_SENS, free));
      }
      printf("%s\n", "Updated Person!!!");
      const char **placeElement = split_string((const char *)dictionary_get(query, "places"), '\n');
      for (i = 0; placeElement[i] != NULL; i++) {
        printf("Place: %s\n", placeElement[i]);

        if(dictionary_has_key(dictionary_get(peopleDictionary, peopleElement[j]), placeElement[i])) {
          printf("%s\n", "Already Exist!!!");
        } else {
          printf("%s\n", "Place added to person!!!");
          dictionary_set(dictionary_get(peopleDictionary, peopleElement[j]), placeElement[i], NULL);
        }
      }

      for (i = 0; placeElement[i] != NULL; i++) {
        free(placeElement[i]);
      }
      free(placeElement);
    }

    for (j = 0; peopleElement[j] != NULL; j++) {
      free(peopleElement[j]);
    }
    free(peopleElement);

    // Updates places
    const char **placesElement = split_string((const char *)dictionary_get(query, "places"), '\n');
    for (j = 0; placesElement[j] != NULL; j++) {
      printf("Place: %s\n", placesElement[j]);

      if(!dictionary_has_key(placesDictionary, placesElement[j])) {
        printf("%s\n", "New Place added!!!");
        dictionary_set(placesDictionary, placesElement[j], make_dictionary(COMPARE_CASE_SENS, free));
      }
      printf("%s\n", "Updated Place!!!");
      const char **peoplesElement = split_string((const char *)dictionary_get(query, "people"), '\n');
      for (i = 0; peoplesElement[i] != NULL; i++) {
        printf("People: %s\n", peoplesElement[i]);

        if(dictionary_has_key(dictionary_get(placesDictionary, placesElement[j]), peoplesElement[i])) {
          printf("%s\n", "Already Exist!!!");
        } else {
          printf("%s\n", "People added to place!!!");
          dictionary_set(dictionary_get(placesDictionary, placesElement[j]), peoplesElement[i], NULL);
        }
      }

      for (i = 0; peoplesElement[i] != NULL; i++) {
        free(peoplesElement[i]);
      }
      free(peoplesElement);
    }

    for (j = 0; placesElement[j] != NULL; j++) {
      free(placesElement[j]);
    }
    free(placesElement);

    peopleNumber = dictionarySize(peopleDictionary);
    placesNumber = dictionarySize(placesDictionary);
    char number1[20];
    char number2[20];
    sprintf(number1, "%d", peopleNumber);
    sprintf(number2, "%d", placesNumber);
    char *temp = append_strings(number1,"\n",number2,"\n");
    body = strdup(temp);
    free(temp);

  } else if(starts_with("/unpin",data)) { 

    const char **peopleElement = split_string((const char *)dictionary_get(query, "people"), '\n');
    for (j = 0; peopleElement[j] != NULL; j++) {
        printf("People: %s\n", peopleElement[j]);

        if(!dictionary_has_key(peopleDictionary, peopleElement[j])) {
          printf("%s\n", "Person does not exist to remove!!!");
        } else {
          printf("%s\n", "Updated Person!!!");
          const char **placeElement = split_string((const char *)dictionary_get(query, "places"), '\n');
          for (i = 0; placeElement[i] != NULL; i++) {
            printf("Place: %s %d\n", placeElement[i], i);

            // dictionary_get(peopleDictionary, peopleElement[j]);
            // printf("%s\n", "SECOND CHECKER");

            if(dictionary_has_key(peopleDictionary, peopleElement[j]) && dictionary_has_key(dictionary_get(peopleDictionary, peopleElement[j]), placeElement[i])) {
              printf("%s\n", "Person remove!!!");
              printf("%s\n", "CHECKER");
              //print_stringdictionary(query);
              dictionary_remove(dictionary_get(peopleDictionary, peopleElement[j]), placeElement[i]);
              if(!dictionarySize(dictionary_get(peopleDictionary, peopleElement[j]))) {
                dictionary_remove(peopleDictionary,peopleElement[j]);
              }
            } else {
              printf("%s\n", "Place does not exist in person!!!");
            }
            free(placeElement[i]);
          }
          free(placeElement);
        }
        free(peopleElement[j]);
      }
      free(peopleElement);

      printf("%s\n", "SECOND PART CHECKER");

      const char **placesElement = split_string((const char *)dictionary_get(query, "places"), '\n');
      for (j = 0; placesElement[j] != NULL; j++) {
        printf("Place: %s\n", placesElement[j]);

        if(!dictionary_has_key(placesDictionary, placesElement[j])) {
          printf("%s\n", "Place does not exist to remove!!!");
        } else {
          printf("%s\n", "Updated Place!!!");
          const char **peoplesElement = split_string((const char *)dictionary_get(query, "people"), '\n');
          for (i = 0; peoplesElement[i] != NULL; i++) {
            printf("People: %s\n", peoplesElement[i]);

            if(dictionary_has_key(placesDictionary, placesElement[j]) && dictionary_has_key(dictionary_get(placesDictionary, placesElement[j]), peoplesElement[i])) {
              printf("%s\n", "Already Exist!!!");
              free(dictionary_get(dictionary_get(placesDictionary, placesElement[j]), peoplesElement[i]));//Check this
              dictionary_remove(dictionary_get(placesDictionary, placesElement[j]), peoplesElement[i]);
              if(!dictionarySize(dictionary_get(placesDictionary, placesElement[j]))) {
                dictionary_remove(placesDictionary,placesElement[j]);
              }
            } else {
              printf("%s\n", "People does not exist in person!!!");
            }
            free(peoplesElement[i]);
          }
          free(peoplesElement);
        }
        free(placesElement[j]);
      }
      free(placesElement);

      peopleNumber = dictionarySize(peopleDictionary);
      placesNumber = dictionarySize(placesDictionary);
      char number1[20];
      char number2[20];
      sprintf(number1, "%d", peopleNumber);
      sprintf(number2, "%d", placesNumber);
      char *temp = append_strings(number1,"\n",number2,"\n");
      body = strdup(temp);
      free(temp);

  } else if(starts_with("/copy",data)) { 

    print_stringdictionary(query);

    if(dictionary_has_key(query, "person")) {
      printf("COPY PERSON\n");
      char *as = dictionary_get(query,"as");
      printf("As: %s\n", as);
      char *port = dictionary_get(query,"port");
      printf("Port: %s\n", port);
      char *person = dictionary_get(query,"person");
      printf("Person: %s\n", person);
      char *host = dictionary_get(query,"host");
      printf("Host: %s\n", host);

      char *result = append_strings("GET /places?person=", person," HTTP/1.1\r\n\r\n", NULL);
      int fdServer = Open_clientfd(host, port);
      int lengnth = strlen(result);
      Write(fdServer, result, lengnth);
      free(result);
      char serverBuffer[MAXLINE];
      rio_t rio;
      Rio_readinitb(&rio,fdServer);
      int elementChecker = 0;
      int index = 0;
      char *placesCopy[100000];

      pthread_mutex_unlock(&lock);
      while(Rio_readlineb(&rio, serverBuffer, MAXLINE)) {
        printf("buffer: \"%s\"\n", serverBuffer);

        if(elementChecker) {
          printf("%s\n", "Do Work");
          char **parts = split_string(serverBuffer,'\n');
          placesCopy[index++] = parts[0];

          for(i = 1;parts[i] != NULL;i++) {
            free(parts[i]);
          }
          free(parts);

        }

        if(strlen(serverBuffer) == 2) elementChecker = 1;
      }
      pthread_mutex_lock(&lock);
      placesCopy[index++] = NULL;
      close(fdServer);

      if(!dictionary_has_key(peopleDictionary, as))
        dictionary_set(peopleDictionary ,as, make_dictionary(COMPARE_CASE_SENS, free));
      for (i = 0;placesCopy[i] != NULL;i++) {
        printf("PLACE COPY: \"%s\"\n", placesCopy[i]);
        if(strlen(placesCopy[i]))  {
          dictionary_set(dictionary_get(peopleDictionary, as), placesCopy[i], NULL);
          if(!dictionary_has_key(placesDictionary ,placesCopy[i])) {
            dictionary_set(placesDictionary, placesCopy[i], make_dictionary(COMPARE_CASE_SENS, free));
          }
          dictionary_set(dictionary_get(placesDictionary, placesCopy[i]), as, NULL);
        }
        free(placesCopy[i]);
      }
      printf("\n");

      peopleNumber = dictionarySize(peopleDictionary);
      placesNumber = dictionarySize(placesDictionary);
      char number1[20];
      char number2[20];
      sprintf(number1, "%d", peopleNumber);
      sprintf(number2, "%d", placesNumber);
      char *temp = append_strings(number1,"\n",number2,"\n");
      body = strdup(temp);
      free(temp);

  } else { 

    printf("COPY PLACE\n");
    char *as = dictionary_get(query,"as");
    printf("As: %s\n", as);
    char *port = dictionary_get(query,"port");
    printf("Port: %s\n", port);
    char *place = dictionary_get(query,"place");
    printf("Person: %s\n", place);
    char *host = dictionary_get(query,"host");
    printf("Host: %s\n", host);

    char *result = append_strings("GET /people?place=", place," HTTP/1.1\r\n\r\n", NULL);
    int fdServer = Open_clientfd(host, port);
    int lengnth = strlen(result);
    Write(fdServer, result, lengnth);
    free(result);
    char serverBuffer[MAXLINE];
    rio_t rio;
    Rio_readinitb(&rio,fdServer);
    int elementChecker = 0;
    int index = 0;
    char *peopleCopy[100000];

    pthread_mutex_unlock(&lock);
    while(Rio_readlineb(&rio, serverBuffer, MAXLINE)) {
      printf("buffer: \"%s\"\n", serverBuffer);
      if(elementChecker) {
        printf("%s\n", "Do Work");
        char **parts = split_string(serverBuffer,'\n');
        peopleCopy[index++] = parts[0];

        for(i = 1;parts[i] != NULL;i++) {
            free(parts[i]);
        }
        free(parts);
      }

      if(strlen(serverBuffer) == 2) elementChecker = 1;
    }
    pthread_mutex_lock(&lock);
    peopleCopy[index++] = NULL;
    close(fdServer);

    if(!dictionary_has_key(placesDictionary, as))
      dictionary_set(placesDictionary ,as, make_dictionary(COMPARE_CASE_SENS, free));
    for (i = 0;peopleCopy[i] != NULL;i++) {

      if(strlen(peopleCopy[i]))  {
        dictionary_set(dictionary_get(placesDictionary, as), peopleCopy[i], NULL);
        if(!dictionary_has_key(peopleDictionary ,peopleCopy[i])) {
          dictionary_set(peopleDictionary, peopleCopy[i], make_dictionary(COMPARE_CASE_SENS, free));
        }
        dictionary_set(dictionary_get(peopleDictionary, peopleCopy[i]), as, NULL);
      }
      free(peopleCopy[i]);
    }

    printf("\n");

      peopleNumber = dictionarySize(peopleDictionary);
      placesNumber = dictionarySize(placesDictionary);
      char number1[20];
      char number2[20];
      sprintf(number1, "%d", peopleNumber);
      sprintf(number2, "%d", placesNumber);
      char *temp = append_strings(number1,"\n",number2,"\n");
      body = strdup(temp);
      free(temp);
  }

  } else { 
    body = strdup("FAIL\n");
  }

  len = strlen(body);

  /* Send response headers to client */
  header = ok_header(len, "text/html; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  printf("Response headers:\n");
  printf("%s", header);

  free(header);

  /* Send response body to client */
  Rio_writen(fd, body, len);

  free(body);

  printf("%s\n", "Done CHECKER");
}