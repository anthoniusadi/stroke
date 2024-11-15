int sensor_grove = 26;
int sensor_myo = 34;

int kalibrator = 0;
const int arraySize = 50;  // Ukuran array
int dataArray[arraySize];  // Array untuk menyimpan data
int currentIndex = 0;      // Indeks saat ini untuk menulis data baru

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





  int myo = analogRead(sensor_myo);
  int myosensorVal = map(myo, 0, 4096, 0, 1024);
  int grove = analogRead(sensor_grove) + kalibrator;
  int grovesensorVal = map(grove, 0, 4096, 0, 1024);


  dataArray[currentIndex] = grove;

  while (Serial.available()) {
    char data_from_display = Serial.read();
    if (data_from_display == 'T') {
      kalibrator = tare(dataArray, arraySize);
      Serial.println(kalibrator);
      delay(5000);
    }
  }

  String Tosend = "add ";
  Tosend += 1;
  Tosend += ",";
  Tosend += 0;
  Tosend += ",";
  Tosend += grovesensorVal;
  Serial.print(Tosend);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  String Tosendsensor = "add ";
  Tosendsensor += 2;
  Tosendsensor += ",";
  Tosendsensor += 0;
  Tosendsensor += ",";
  Tosendsensor += myosensorVal;
  Serial.print(Tosendsensor);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  Serial.print("n0.val=");
  Serial.print(grove);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.print("n1.val=");
  Serial.print(myo);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  currentIndex = (currentIndex + 1) % arraySize;
  // Serial.print("z0.val=");  //Send the object tag
  // int analogVal = map(grovesensorVal, 0, 1024, 0, 200);
  // Serial.print(analogVal);
  // Serial.write(0xff);
  // Serial.write(0xff);
  // Serial.write(0xff);
}

int tare(int data[], int size) {
  int value;
  for (int i = 0; i < size; i++) {
    value += data[i];
  }
  value = value / size;
  return value;
}
