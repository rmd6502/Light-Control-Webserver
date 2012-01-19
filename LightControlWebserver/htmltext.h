const char topPart[] PROGMEM = "HTTP/1.1 200 OK\n"
  "Content-Type: text/html\n\n"
  "<HTML><HEAD><TITLE>Arduino!</TITLE>"
  "<STYLE TYPE=\"text/css\">"
  "td { border: solid; }"
  "</STYLE>"
  "<SCRIPT TYPE=\"text/javascript\">\r\n"
  "function setRGB(r,g,b) {\r\n"
  "    document.forms[0].r.value = r;\r\n"
  "    document.forms[0].g.value = g;\r\n"
  "    document.forms[0].b.value = b;\r\n"
  "    document.forms[0].submit();\r\n"
  "}\r\n"
  "</SCRIPT>"
  "</HEAD>"
  "<BODY>"
  "<FORM>"
  "<TABLE style=\"border: solid;\">"
  "<TR>";

const char cycleButton[] PROGMEM = "<INPUT TYPE=\"submit\" name=\"cycle\" value=\"%s\"/><br />";

const char varPart[] = "<TD>Red</TD><TD><INPUT TYPE=\"text\" name=\"r\" value=\"%d\"/></TD>"
  "</TR>"
  "<TR>"
  "<TD>Green</TD><TD><INPUT TYPE=\"text\" name=\"g\" value=\"%d\"/></TD>"
  "</TR>"
  "<TR>"
  "<TD>Blue</TD><TD><INPUT TYPE=\"text\" name=\"b\" value=\"%d\"/></TD>";

const char botPart1[] PROGMEM = "</TR>"
  "</TABLE>"
  "<INPUT TYPE=\"submit\" value=\"Set Colors\"/><br />";

  
const char botPart2[] PROGMEM =
  "</FORM>"
  "</BODY>"
  "</HTML>\r\n";
