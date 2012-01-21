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

static int8_t cycle = -1;
static uint16_t dly = 5;

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
      checkColors();
    }
    // give the web browser time to receive the data
    delay(100);
    client.stop();
  }
  checkColors();
}

void checkColors() {
  uint8_t bgoal = 1;
  for (int j=0; j < 3; ++j) {
    if (goal[j] != current[j]) {
      bgoal = 0;
      int dir = goal[j] - current[j];
      dir /= abs(dir);
      //Serial.print("dir "); Serial.println((short)dir);
      current[j] += dir;
      analogWrite(pins[j], current[j]);
    }
  }
  if (bgoal == 1 && cycle != -1) {
    if (cycle >= 0) {
      Serial.println("cycling");
      NamedColor ncl;
      memcpy_P(&ncl, &colors[cycle++], sizeof(NamedColor));
      goal[0] = ncl.r; goal[1] = ncl.g; goal[2] = ncl.b;
      memcpy_P(&ncl, &colors[cycle], sizeof(NamedColor));
      if (ncl.name == 0) cycle = 0;
    } else {
      Serial.println("randoming");
      delay(50);
      goal[0] = random() & 0xff;
      goal[1] = random() & 0xff;
      goal[2] = random() & 0xff;
    }
  }
  delay(dly);
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
      Serial.print("p "); Serial.println(p);
      if (!strncasecmp(p, "cycle", 5)) {
        if (cycle == -2) cycle = -1; else
        if (cycle == -1) cycle = 0; else cycle = -1;
      }
      else if (!strncasecmp(p, "random", 6)) {
        if (cycle >= -1) {
          cycle = -2;
          dly = 20;
        } else {
          cycle = -1;
          dly = 5;
        }
      }
      else if (!strncasecmp(p, "delay", 5)) {
        dly = atoi(q);
      }
      else if (*p == 'r') {
        goal[0] = atoi(q);
        cycle = -1;
      } else if (*p == 'g') {
        goal[1] = atoi(q);
        cycle = -1;
      } else if (*p == 'b') {
        goal[2] = atoi(q);
        cycle = -1;
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
  int count = 0;
  char cbuf[140];
  strcpy_P(cbuf, cycleButton);
  sprintf(buf, cbuf, ((cycle > -1) ? "Stop Cycling" : "Cycle"), ((cycle == -2) ? "Stop Random" : "Random"));
  Serial.println(buf);
  client.println(buf);
  
  for (const NamedColor *nc = colors; pgm_read_word(nc) != 0; ++nc) {    
    
    NamedColor ncl;
    memcpy_P(&ncl, nc, sizeof(NamedColor));
    strcpy_P(cbuf, ncl.name);
    snprintf(buf, sizeof(buf), "<INPUT TYPE=\"button\" name=\"setcolor\" "
              "style=\"background: #%02x%02x%02x; color: #%02x%02x%02x\" value=\"%s\" "
              "onclick=\"setRGB(%d,%d,%d)\" />", 
              ncl.r, ncl.g, ncl.b,
              ncl.fgr, ncl.fgg, ncl.fgb,
              cbuf, ncl.r, ncl.g, ncl.b);
    Serial.println(buf);
    client.println(buf);
    if (!(++count & 0x7)) {
      client.println("<br />");
    }
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

