int sensor_grove = 26;
int sensor_myo = 34;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(sensor_grove, INPUT);
  pinMode(sensor_myo, INPUT);
}
int kalibrator=0;

void loop() {
  if (Serial.available()>0){
    kalibrator = tare();
  }
  int myo = analogRead(sensor_myo);
  int myosensorVal = map(myo, 0, 4096, 0, 1024);
  int grove = analogRead(sensor_grove)+kalibrator;
  int grovesensorVal = map(grove, 0, 4096, 0, 1024);



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

  Serial.print("z0.val=");  //Send the object tag
  int analogVal =map(grovesensorVal, 0, 1024, 0, 200);
  Serial.print(analogVal);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  
}

int tare(){

return value;
}
