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
#include "colors.h"

Server server(80);
  char buf[256];
  
static const int rLed = 3;
static const int gLed = 5;
static const int bLed = 6;

byte current[3] = {0};
byte goal[3] = {255, 200, 180};
byte pins[3] = {0};

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
  size_t size() const { return SZ; }
  const char& operator[](int pos) const { if (pos >= 0 && pos < SZ) return requestbuf[pos]; else return b; }
  char& operator[](int pos) { if (pos >= 0 && pos < SZ) return requestbuf[pos]; else return b; }
  operator char *() { return requestbuf; }
  operator const char *() const { return requestbuf; }
};

RequestBuf<128> requestbuf;

void setup() {
  Serial.begin(115200);
  
  Serial.println("starting");
  
  pins[0] = rLed;
  pins[1] = gLed;
  pins[2] = bLed;
  
  WiFly.begin();
  if (!WiFly.join(ssid, passphrase)) {
    Serial.print("Failed to associate with "); Serial.println(ssid);
    while (1) {
      // Hang on failure.
    }
  }
  
  WiFly.configure(WIFLY_BAUD, 115200);

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
      for (int j=0; j < 3; ++j) {
        if (goal[j] != current[j]) {
          int dir = goal[j] - current[j];
          dir /= abs(dir);
          //Serial.print("dir "); Serial.println((short)dir);
          current[j] += dir;
          analogWrite(pins[j], current[j]);
        }
      }
      delay(5);
    }
    // give the web browser time to receive the data
    delay(100);
    client.stop();
  }
  for (int j=0; j < 3; ++j) {
    if (goal[j] != current[j]) {
      int dir = goal[j] - current[j];
      dir /= abs(dir);
      //Serial.print("dir "); Serial.println((short)dir);
      current[j] += dir;
      analogWrite(pins[j], current[j]);
    }
  }
  delay(5);
}

void handle_request(char *request, Client &client) {
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
        goal[0] = atoi(q);
      } else if (*p == 'g') {
        goal[1] = atoi(q);
      } else if (*p == 'b') {
        goal[2] = atoi(q);
      } else if (!strncasecmp(p, "setcolor", strlen("setcolor"))) {
        char cbuf[32], dbuf[32] = {0};
        strncpy(dbuf, q, 31);
        char *k = dbuf;
        while (*k) {
          if (*k == '+') *k = ' ';
          ++k;
        }
        for (const NamedColor *nc = colors; nc->name; ++nc) {
          strcpy_P(cbuf, nc->name);
          if (!strcasecmp(cbuf, dbuf)) {  
            goal[0] = nc->r; goal[1] = nc->g; goal[2] = nc->b;
            break;
          }
        }
        break;
      }    
    }
  }
  output_P(buf, client, topPart);
  buf[255] = 0;
  snprintf(buf, sizeof(buf), varPart, goal[0], goal[1], goal[2]);
  buf[255] = 0;
  Serial.println(buf);
  client.println(buf);
  output_P(buf, client, botPart1);
  for (const NamedColor *nc = colors; nc->name != 0; ++nc) {    
    char cbuf[32];
    strcpy_P(cbuf, nc->name);
    snprintf(buf, sizeof(buf), "<INPUT TYPE=\"submit\" name=\"setcolor\" "
              "style=\"background: #%02x%02x%02x; color: #%02x%02x%02x\" value=\"%s\" />", 
              nc->r, nc->g, nc->b,
              nc->fgr, nc->fgg, nc->fgb,
              cbuf);
    Serial.println(buf);
    client.println(buf);
  }
  output_P(buf, client, botPart2);
}

void printf_P(char *buf, const char *str) {
  int part_len = strlen_P(str);
  Serial.print("len "); Serial.println(part_len);
  int c = 0;
  while (c < part_len) {
    strncpy_P(buf, str+c, 255);
    buf[255] = 0;
    Serial.println(buf);
    c += 255;
  }
}

void output_P(char *buf, Client& client, const char * str) {
  int part_len = strlen_P(str);
  int c = 0;
  Serial.print("len ");
  Serial.println(part_len);
  memset(buf, 0, 256);
  while (c < part_len) {
    strncpy_P(buf, str+c, 255);
    buf[255] = 0;
    Serial.print("c "); Serial.print(c); Serial.print(" buf \"");
    Serial.print(buf);Serial.println("\"");
    client.println(buf);
    c += (255);
  }
}

