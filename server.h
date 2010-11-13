#ifndef HTTP2RTSP_SERVER_H
#define HTTP2RTSP_SERVER_H


#include "parse_video.h"


#define QUEUESIZE 400

enum mediastates
{
  IDLE,
  GETSENT,
  RECVTCP,
  STREAM
};

enum clientstates
{
  NOCLIENT,
  CLICONNECTED,
  SDPSENT,
  SETUPSENT,
  STREAMING
};

enum timeouttypes
{
  FRAME,
  CHECKMEDIASTATE,
  STARTSTREAMING
};

typedef struct client
{
  int state;
  int rtspfd;
  int session;
  int cseq;
  int videofds[2];
  int audiofds[2];
} Client;


typedef struct timeoutevent
{
  struct timeoutevent *next, *prev;
  struct timeval time;
  int type;
  Frame *frame;
} TimeoutEvent;


typedef struct queue
{
  TimeoutEvent *first, *last;
  int size;
} Queue;

void init_client(Client *client);

void *fill_queue(void *init);

int start_server(const char *url, const char *rtspport);

/* Inserts the given TimeoutEvent to the give queue. Retains the order
 * of the queue, smallest time first. */
void push_event(TimeoutEvent *event, Queue *queue);

/* Pulls the first item from the given queue. If the queue is empty, returns NULL */
TimeoutEvent *pull_event(Queue *queue);


void set_timeval(TimeoutEvent *event, struct timeval base);


void push_timeout(Queue *queue, int time_ms, int type);


/* Returns positive is 'second' is bigger, negative if 'first' is bigger,
 * and 0 if they are both the same size. */
int timecmp(struct timeval first, struct timeval second);


/* Calculates the difference between two time instants, (second - first) */
struct timeval caclulate_delta(struct timeval *first, struct timeval *second);


#endif

