
int sensor_grove = 26;
int sensor_myo = 34;
int value=0;
int signal_wave=0; 
int kalibrator = 0;
int initial_value = 0;
const int arraySize = 50;  // Ukuran array
int dataArray[arraySize];  // Array untuk menyimpan data
int currentIndex = 0;      // Indeks saat ini untuk menulis data baru
 int offset=200;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensor_grove, INPUT);
  pinMode(sensor_myo, INPUT);

  for (int i = 0; i < arraySize; i++) {
    dataArray[i] = 0;
  }
}

//char data_from_display;
void loop() {
  int grove = analogRead(sensor_grove);
  int myo = analogRead(sensor_myo);
  int myosensorVal = map(myo, 0, 4096, 0, 2048);
  int grovesensorVal = map(grove, 0, 4096, 0, 2048);
  dataArray[currentIndex] = grovesensorVal;
  int grove_kalibrasi = grovesensorVal - kalibrator;

  while (Serial.available()) {
    
    char data_from_display = Serial.read();
    if (data_from_display == 'T') {
      for (int i = 0; i < arraySize; i++) {
         value += dataArray[i];
      }
      kalibrator = tare(dataArray, arraySize)-offset;
      initial_value=kalibrator;
     
      delay(3000);
    }
  }

  signal_wave = map(grove_kalibrasi,0,2048,0,800);
  String Tosend = "add ";
  Tosend += 1;
  Tosend += ",";
  Tosend += 0;
  Tosend += ",";
  Tosend += grove_kalibrasi;

  Serial.print(Tosend);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  String Tosend2 = "add ";
  Tosend2 += 4;
  Tosend2 += ",";
  Tosend2 += 0;
  Tosend2 += ",";
  Tosend2 += signal_wave;
  Serial.print(Tosend2);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);


  Serial.print("n0.val=");
  Serial.print(grove_kalibrasi);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("val.val=");
  Serial.print(grove_kalibrasi);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  currentIndex = (currentIndex + 1) % arraySize;
  // int progress_bar = map(grove_kalibrasi, 0, 1024, 0, 100);
  int progress_bar = map(grove_kalibrasi,0,450,0,100);
  Serial.print("j0.val=");
  Serial.print(progress_bar);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  
}

int tare(int data[], int size) {
  int value = 0;
  int send = 0;
 
  for (int i = 0; i < size; i++) {
    value += data[i];
  }
  send = (value/size);

  return send;
}