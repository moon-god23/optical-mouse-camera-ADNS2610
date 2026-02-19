// ADNS-2610 Raw Data Stream
const int SDIO = 2;
const int sCK = 3;

void setup() {
  Serial.begin(115200);
  pinMode(sCK, OUTPUT);
  pinMode(SDIO, OUTPUT);

  // Soft Reset: Write 0x80 to Configuration Register (0x00) 
  writeRegister(0x00, 0x80); 
  delay(100); 
  
  // Forced Awake: Set bit 0 of Configuration Register to 1 
  writeRegister(0x00, 0x01); 
}

void loop() {

  // Reset pixel pointer by writing to Pixel_Data register (0x08) 
  writeRegister(0x08, 0x00); 

  for (int i = 0; i < 324; i++) {
    byte raw = readRegister(0x08);
    
    // Wait for Data_Valid (Bit 6) to be 1 
    if (!(raw & 0x40)) { 
      i--; 
      continue;
    }

    // Extract 6-bit grayscale data [cite: 1009, 1018]
    byte val = raw & 0x3F; 
    
    Serial.print(val);
    
    // Formatting: comma between pixels, new line every 18 pixels 
    if (i % 18 == 17) {
      Serial.println(); 
    } else {
      Serial.print(",");
    }
  }
  byte squal = readRegister(0x04);
  Serial.print("SQUAL: "); 
  Serial.println(squal);
  
  Serial.println("---FRAME_END---");
  delay(1000); 
}

void writeRegister(byte addr, byte data) {
  pinMode(SDIO, OUTPUT);
  addr |= 0x80; // MSB=1 for Write 
  for (int i = 7; i >= 0; i--) {
    digitalWrite(sCK, LOW);
    digitalWrite(SDIO, (addr >> i) & 1);
    digitalWrite(sCK, HIGH); 
  }
  for (int i = 7; i >= 0; i--) {
    digitalWrite(sCK, LOW);
    digitalWrite(SDIO, (data >> i) & 1);
    digitalWrite(sCK, HIGH);
  }
}

byte readRegister(byte addr) {
  byte data = 0;
  pinMode(SDIO, OUTPUT);
  addr &= 0x7F; // MSB=0 for Read [cite: 590]
  for (int i = 7; i >= 0; i--) {
    digitalWrite(sCK, LOW);
    digitalWrite(SDIO, (addr >> i) & 1);
    digitalWrite(sCK, HIGH);
  }
  
  pinMode(SDIO, INPUT);
  delayMicroseconds(120); // t_HOLD: Must be >100us 
  
  for (int i = 7; i >= 0; i--) {
    digitalWrite(sCK, LOW);
    digitalWrite(sCK, HIGH);
    data |= (digitalRead(SDIO) << i); // Read on rising edge 
  return data;
}