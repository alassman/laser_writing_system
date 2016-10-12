int readline(int readch, char *buffer, int len);
int input_pos = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  static char buffer[50];
  if (readline(Serial.read(), buffer, 50) > 0) {
    Serial.print("You entered: >");
    Serial.print(buffer);
    Serial.println("<");
  }
}

//returns index of one past final character
int readline(int readch, char *buffer, int len)
{
  int rpos;

  if (readch > 0) {
    switch (readch) {
      case '\n': // Ignore new-lines
                Serial.println("<");

        break;
      case '\r': // Return on CR
        Serial.println("<");
        rpos = input_pos;
        input_pos = 0;  // Reset position index ready for next time
        return rpos;
      default:
        if (input_pos < len-1) {
          buffer[input_pos++] = readch;
          buffer[input_pos] = 0;
        }
    }
  }
  // No end of line has been found, so return -1.
  return -1;
}
