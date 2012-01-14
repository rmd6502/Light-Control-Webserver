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

char varPart[] = "<TD>Red</TD><TD><INPUT TYPE=\"text\" name=\"r\" value=\"%d\"/></TD>"
  "</TR>"
  "<TR>"
  "<TD>Green</TD><TD><INPUT TYPE=\"text\" name=\"g\" value=\"%d\"/></TD>"
  "</TR>"
  "<TR>"
  "<TD>Blue</TD><TD><INPUT TYPE=\"text\" name=\"b\" value=\"%d\"/></TD>";

const char botPart1[] PROGMEM = "</TR>"
  "</TABLE>"
  "<INPUT TYPE=\"submit\" value=\"Set Colors\"/><br />"
  "<INPUT TYPE=\"submit\" name=\"setcolor\" value=\"white\"/>"
  "<INPUT TYPE=\"submit\" name=\"setcolor\" style=\"background: black; color: white; \" value=\"off\"/>";
  
const char botPart2[] PROGMEM = 
  "<INPUT TYPE=\"submit\" name=\"setcolor\" style=\"background: blue; color: white; \" value=\"blue\"/>"
  "<INPUT TYPE=\"submit\" name=\"setcolor\" style=\"background: red; color: white; \" value=\"red\"/>";
  
const char botPart3[] PROGMEM =   
  "<INPUT TYPE=\"submit\" name=\"setcolor\" style=\"background: yellow; color: black; \" value=\"yellow\"/>"
  "<INPUT TYPE=\"submit\" name=\"setcolor\" style=\"background: green; color: yellow; \" value=\"green\"/>";
  
const char botPart4[] PROGMEM =
  "<INPUT TYPE=\"submit\" name=\"setcolor\" style=\"background: #101010; color: white; \" value=\"night\"/>"
  "</FORM>"
    "</BODY>"
    "</HTML>\r\n";
