const char topPart[] PROGMEM = "HTTP/1.1 200 OK\n"
  "Content-Type: text/html\n\n"
  "<HTML><HEAD><TITLE>Arduino!</TITLE>"
  "<STYLE TYPE=\"text/css\">"
  "td { border: solid; }"
  "</STYLE>"
  "</HEAD>"
  "<BODY>"
  "<FORM>"
  "<TABLE style=\"border: solid;\">"
  "<TR>";

const char varPart[] = "<TD>Red</TD><TD><INPUT TYPE=\"text\" name=\"r\" value=\"%d\"/></TD>"
  "</TR>"
  "<TR>"
  "<TD>Green</TD><TD><INPUT TYPE=\"text\" name=\"g\" value=\"%d\"/></TD>"
  "</TR>"
  "<TR>"
  "<TD>Blue</TD><TD><INPUT TYPE=\"text\" name=\"b\" value=\"%d\"/></TD>";

const char botPart1[] PROGMEM = "</TR>"
  "</TABLE>"
  "<INPUT TYPE=\"submit\" value=\"Set Colors\"/><br />"
  "<INPUT TYPE=\"submit\" name=\"setcolor\" value=\"white\"/>";
  
const char botPart2[] PROGMEM =
  "</FORM>"
  "</BODY>"
  "</HTML>\r\n";
