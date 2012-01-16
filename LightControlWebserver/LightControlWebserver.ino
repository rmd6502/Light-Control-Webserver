/*
 * Web Server
 *
 * (Based on Ethernet's WebServer Example)
 *
 * A simple web server that shows the value of the analog input pins.
 */
 
#include <avr/pgmspace.h>
#include "WiFly.h"
#include "Credentials.h"
#include "htmltext.h"

Server server(80);

static const int rLed = 3;
static const int gLed = 5;
static const int bLed = 6;

static int r=0, g=0, b=0;

template <int SZ>
class RequestBuf {
  char requestbuf[SZ];
  int requestpos;
  char b;
  
  public:
  RequestBuf() { clearBuf(); }
  void clearBuf() { requestpos = 0; requestbuf[requestpos] = 0; }
  void append(int c) {
    if (c == -1 || requestpos >= SZ - 1) return;
    requestbuf[requestpos++] = c;
    requestbuf[requestpos] = 0; 
  }
  const char& operator[](int pos) const { if (pos >= 0 && pos < SZ) return requestbuf[pos]; else return b; }
  char& operator[](int pos) { if (pos >= 0 && pos < SZ) return requestbuf[pos]; else return b; }
  operator char *() { return requestbuf; }
  operator const char *() const { return requestbuf; }
};

RequestBuf<128> requestbuf;

void setup() {
  WiFly.begin();

  if (!WiFly.join(ssid, passphrase)) {
    Serial.print("Failed to associate with "); Serial.println(ssid);
    while (1) {
      // Hang on failure.
    }
  }
  
  WiFly.configure(WIFLY_BAUD, 115200);

  Serial.begin(115200);
  Serial.print("IP: ");
  Serial.println(WiFly.ip());
  
  server.begin();
  
  digitalWrite(rLed, LOW);
  digitalWrite(gLed, LOW);  
  digitalWrite(bLed, LOW);
  
  pinMode(rLed, OUTPUT);
  pinMode(gLed, OUTPUT);
  pinMode(bLed, OUTPUT);
}

void loop() {
  Client client = server.available();
  if (client) {
    Serial.println("got connection");
    requestbuf.clearBuf();
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        requestbuf.append(c);
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == '\n' && current_line_is_blank) {
          handle_request(requestbuf, client);
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(100);
    client.stop();
  }
}

void handle_request(char *request, Client &client) {
  static int botpart_len = 0;
  if (botpart_len == 0) {
    botpart_len = strlen_P(botPart);
  }
  char *firstline = strtok(request, "\r\n");
  char *verb = strtok(firstline, " \t");
  char *req = strtok(NULL, " \t");
  Serial.print("verb "); Serial.println(verb);
  Serial.print("request "); Serial.println(req);
  char *p = strchr(req, '?');
  if (p != NULL) {
    ++p;
    for (p = strtok(p, "&"); p != NULL; p = strtok(NULL, "&")) {
      char *q = strchr(p, '=');
      if (q) ++q;
      if (*p == 'r') {
        r = atoi(q);
      } else if (*p == 'g') {
        g = atoi(q);
      } else if (*p == 'b') {
        b = atoi(q);
      } else if (!strncasecmp(p, "setcolor", strlen("setcolor"))) {
        Serial.println("setcolor");
        if (!strcasecmp(q, "white")) {
          Serial.println("white");
          r = 255; g = 240; b = 230;
        } else if (!strcasecmp(q, "off")) {
          r = 0; g = 0; b = 0;
        } else if (!strcasecmp(q, "blue")) {
          r = 0; g = 0; b = 255;
        } else if (!strcasecmp(q, "red")) {
          b = 0; g = 0; r = 255;
        } else if (!strcasecmp(q, "green")) {
          r = 0; b = 0; g = 255;
        } else if (!strcasecmp(q, "yellow")) {
          b = 20; g = 255; r = 255;
        } else if (!strcasecmp(q, "orange")) {
          r = 255; g = 128; b = 0;
        } else if (!strcasecmp(q, "pink")) {
          r = 255; g = 60; b = 120;
        } else if (!strcasecmp(q, "night")) {
          r = 5; g = 3; b = 1;
        }
      }    
    }
    analogWrite(rLed, r);
    analogWrite(gLed, g);
    analogWrite(bLed, b);
  }
  char buf[256];
  strcpy_P(buf, topPart);
  buf[255] = 0;
  Serial.println(buf);
  client.println(buf);
  sprintf(buf, varPart, r, g, b);
  buf[255] = 0;
  Serial.println(buf);
  client.println(buf);
  int c = 0;
  while (c < botpart_len) {
    strncpy_P(buf, &botPart[c], 255);
    buf[255] = 0;
    Serial.println(buf);
    client.println(buf);
    c += 255;
  }
}

