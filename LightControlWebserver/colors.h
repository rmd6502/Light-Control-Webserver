struct NamedColor {
  const char * PROGMEM name;
  int r,g,b;
};

const char* PROGMEM red = "red";
const char* PROGMEM dim_red = "dim red";
const char* PROGMEM orange = "orange";
const char* PROGMEM yellow = "yellow";
const char* PROGMEM green = "green";
const char* PROGMEM blue = "blue";
const char* PROGMEM violet = "violet";
const char* PROGMEM white = "white";
const char* PROGMEM off = "off";
const char* PROGMEM night = "night";

const NamedColor colors[] = {
  { red, 255, 0, 0 },
  { dim_red, 100, 0, 0 },
  { orange, 255, 128, 0 },
  { yellow, 255, 255, 0 },
  { green, 0, 255, 0 },
  { blue, 0, 0, 255 },  
  { violet, 255, 0, 255 },
  { white, 255, 240, 230 },
  { off, 0, 0, 0 },
  { night, 3, 2, 1 },
  { 0, 0, 0, 0}
};
