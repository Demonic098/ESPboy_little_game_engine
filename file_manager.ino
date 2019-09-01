static const uint8_t iconBin[] PROGMEM = {
  0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x1c,0xcc,0xcc,0xcc,0xcc,0xcc,
  0xcc,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,
  0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,
  0x11,0x11,0x1c,0x1c,0xc1,0x1c,0x1c,0xc1,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x1c,0x1c,0x11,0x1c,0x1c,
  0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x1c,0xc1,0x1c,0x1c,0x1c,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x1c,
  0x1c,0x1c,0x1c,0x1c,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x1c,0x11,0x11,0x11,
  0x11,0x11,0x1c,0x1c,0xc1,0x1c,0x1c,0x1c,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,0x11,0x11,
  0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0x11,
  0x11,0x11,0x11,0x11,0x1c,0x11,0x11,0x11,0x11,0x11,0x1c,0xcc,0xcc,0xcc,0xcc,0xcc,0xcc,0x11,0x11,0x11,
  0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11
};

uint8_t drawDialog(){
  char txt[] = "Press the leftbutton to delete this save.";
  char b1[] = "Yes";
  char b2[] = "No";
  int16_t i,x,y;
  setColor(1);
  for(i = 30; i < 90; i++)
    drwLine(20, i, 108, i);
  setColor(8);
  drwLine(20, 30, 108, 30);
  drwLine(20, 90, 108, 90);
  drwLine(20, 30, 20, 90);
  drwLine(108, 30, 108, 90);
  i = 0;
  x = 24;
  y = 32;
  for(i = 0; i < 43; i++){
    putchar(txt[i], x, y);
    x += 6;
    if(x > 102){
      x = 24;
      y += 8;
    }
  }
  for(i = 0; i < 4; i++){
    putchar(b1[i], 24 + i * 6, 80);
  }
  for(i = 0; i < 3; i++){
    putchar(b2[i], 80 + i * 6, 80);
  }
  redrawScreen();
  delay(400);
  while(1){
    thiskey = 0;
    while(thiskey == 0){   
      getKey();
      delay(100);
      changeSettings();
      if(fileIsLoad)
        return 0;
    }
    if(thiskey & 4){//left
      return 1;
    }
    else
      return 0;
  }
}

void drawMenuBackground(){
  int16_t i;
  setColor(1);
  for(i = 7; i < 121; i++)
    drwLine(13, i, 115, i);
  setColor(8);
  drwLine(12, 6, 116, 6);
  drwLine(12, 121, 116, 121);
  drwLine(12, 6, 12, 121);
  drwLine(116, 6, 116, 121);
}

void drawSave(uint8_t startPos, uint8_t selectPos){
  uint16_t pos;
  uint8_t c,i,n;
  String s_buffer;
  pos = 0;
  n = 0;
  while(pos < EEPROM_SIZE && (n - startPos < 14)){
    if(n == selectPos)
      setColor(2);
    else
      setColor(0);
    c = EEPROM.read(pos);
    if(c == 0 || c == 0xff)
      return;
    if(n >= startPos){
      i = 0;
      c = EEPROM.read(pos + 1);
      while(i < 12 && c != 0){
        putchar(c, 14 + i * 6, 8 + (n - startPos) * 8);
        i++;
        c = EEPROM.read(pos + 1 + i);
      }
      c = EEPROM.read(pos);
      s_buffer = String(c);
      for(i = 0; i < s_buffer.length(); i++){
        putchar(s_buffer[i], 100 + i * 6, 8 + (n - startPos) * 8);
      }
    }
    n++;
    pos += c;
  }
}

uint8_t getSaveLength(){
  uint16_t pos = 0;
  uint8_t n = 0;
  uint8_t c;
  while(pos < EEPROM_SIZE){
    c = EEPROM.read(pos);
    if(c == 0 || c == 0xff)
      return n;
    pos += c;
    n++;
  }
  return n;
}

void deliteSave(uint8_t num, uint16_t end){
  uint16_t i,pos = 0;
  uint8_t n = 0;
  uint8_t c;
  while(pos < EEPROM_SIZE && n != num){
    c = EEPROM.read(pos);
    pos += c;
    n++;
  }
  c = EEPROM.read(pos);
  for(i = pos + c; i < end; i++){
    EEPROM.write(i - c, EEPROM.read(i));
    EEPROM.write(i, 0);
  }
  EEPROM.commit();
}

void saveManager(){
  uint8_t pos,listLength;
  drawMenuBackground();
  drawSave(0,0);
  redrawScreen();
  pos = 0;
  listLength = getSaveLength();
  delay(400);
  while(1){
    thiskey = 0;
    while(thiskey == 0){   
      getKey();
      delay(100);
      changeSettings();
      if(fileIsLoad)
        return;
    }
    if(thiskey & 128){
      return;
    }
    else if(thiskey & 2){//down
      if(pos < listLength - 1){
        pos++;
        drawMenuBackground();
        drawSave(pos,pos);
        redrawScreen();
      }
    }
    else if(thiskey & 1){//up
      if(pos > 0){
        pos--;
        drawMenuBackground();
        drawSave(pos,pos);
        redrawScreen();
      }
    }
    else if(thiskey & 16){//ok
      if(drawDialog()){
        deliteSave(pos, findEndData());
      }
      drawMenuBackground();
      drawSave(pos,pos);
      redrawScreen();
    }
  }
}

void fileList(String path) {
  fs::Dir dir = SPIFFS.openDir(path);
  char s[32];
  char thisF[32];
  int16_t lst = 1;
  int16_t pos = 0;
  int16_t startpos = 0;
  int16_t fileCount = 0;
  int16_t skip = 0;
  uint8_t i,b;
  for(i = 0; i < 192; i++)
    mem[i + 1024 + 192] = pgm_read_byte_near(iconBin + i);
  setImageSize(1);
  while (dir.next()) {
    fs::File entry = dir.openFile("r");
    entry.close();
    fileCount++;
  }
  Serial.print(F("find "));
  Serial.print(fileCount);
  Serial.println(F(" files"));
  while(1){
    clearScr(0);
    skip = startpos;
    lst = 1;
    dir = SPIFFS.openDir(path);
    setColor(8);
    for(i = 1; i < 17; i++)
      drwLine(2, (pos - startpos) * 17 + i, 124,  (pos - startpos) * 17 + i);
    setColor(1);
    while (dir.next() && lst < 8) {
      fs::File entry = dir.openFile("r");
      if(skip > 0){
        skip--;
      }
      else{
        strcpy(s, entry.name());
        if(lst + startpos - 1 == pos)
          strcpy(thisF, entry.name());
        putString(s, lst * 17 - 16);
        i = 0;
        while(i < 28 && s[i] != '.')
          i++;
        i++;
        if(s[i] == 'l' && s[i + 1] == 'g' && s[i + 2] == 'e'){
          entry.seek(3, SeekSet);
          if(entry.available())
            b = (uint8_t)entry.read();
          if(b & 0x2){
            entry.seek(5, SeekSet);
            for(i = 0; i < 192; i++)
              if(entry.available())
                mem[i + 1024] = (uint8_t)entry.read(); 
            drawImg(1024, 0, lst * 17 - 16, 24, 16);
          }
          else
            drawImg(1024 + 192, 0, lst * 17 - 16, 24, 16);
        }
        else if(s[i] == 'b' && s[i + 1] == 'i' && s[i + 2] == 'n')
          drawImg(1024 + 192, 0, lst * 17 - 16, 24, 16);
        lst++;
      }
      entry.close();   
    }
    if(lst + startpos - 1 < pos){
      if(fileCount > pos)
        startpos++;
      else
        pos--;
    }
    else if(startpos > pos){
      startpos = pos;
    }
    redrawScreen();  
    getKey();
    delay(200);
    while(thiskey == 0){   
      getKey();
      delay(100);
      changeSettings();
      if(fileIsLoad)
        return;
    }
    if(thiskey & 16){//ok
      cpuInit();
      i = 0;
      while(i < 28 && thisF[i] != '.')
        i++;
      i++;
      setLoadedFileName(thisF);
      if(thisF[i] == 'b' && thisF[i + 1] == 'i' && thisF[i + 2] == 'n')
        loadBinFromSPIFS(thisF);
      else if(thisF[i] == 'l' && thisF[i + 1] == 'g' && thisF[i + 2] == 'e')
        loadLgeFromSPIFS(thisF);
      return;
    }
    else if(thiskey & 2){//down
      if(pos < fileCount - 1)
        pos++;
      if(pos - startpos > 5)
        startpos++;
    }
    else if(thiskey & 1){//up
      if(pos > 0)
        pos--;
      if(pos - startpos < 0)
        startpos--;
    }
    if(thiskey & 128){//select
      saveManager();
      delay(400);
    }
    if(thiskey & 4){//left
      cpuInit();
      return;
    }
  }
}

void loadBinFromSPIFS(char fileName[]){
  int i;
  fs::File f = SPIFFS.open(fileName, "r");
  if(f.size() < RAM_SIZE)
    for(i = 0; i < f.size(); i++){
      mem[i] = (uint8_t)f.read();
    }
  Serial.print(F("loaded "));
  Serial.print(i);
  Serial.println(F(" byte"));
  Serial.print(F("free heap "));
  Serial.println(system_get_free_heap_size());
  f.close();  //Close file
}

void loadLgeFromSPIFS(char fileName[]){
  int n,j = 0;
  uint8_t b,l;
  int16_t length, position, point;
  fs::File f = SPIFFS.open(fileName, "r");
  if((char)f.read() == 'l' && (char)f.read() == 'g' && (char)f.read() == 'e'){
    l = (uint8_t)f.read();
  }
  else
    return;
  n = (uint8_t)f.read();
  Serial.print(F("offset "));
  Serial.print(n);
  f.seek(n, SeekSet);
  n = 0;
  while(f.available()){
    b = (uint8_t)f.read();
    if((b & 128) == 0){
      length = ((b & 127) << 8) + (uint8_t)f.read();
      for( j = 0; j < length; j ++){
        if(n < RAM_SIZE)
          mem[n] = (uint8_t)f.read();
        n++;
      }
    }
    else{
      length = (b & 127) >> 1;
      position = (((b & 1) << 8) + (uint8_t)f.read());
      point = n - position;
      for( j = 0; j < length; j ++){
        if(n < RAM_SIZE && point + j < RAM_SIZE)
          mem[n] = mem[point + j];
        n++;
      }
    }
  }
  Serial.print(F("loaded "));
  Serial.print(n);
  Serial.println(F(" byte"));
  Serial.print(F("free heap "));
  Serial.println(system_get_free_heap_size());
  f.close();  //Close file
}
