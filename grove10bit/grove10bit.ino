const int sampleSize = 12;  // Ukuran buffer untuk moving average
float signalBuffer[sampleSize];
int bufferIndex = 0;
#include <WiFi.h>
#include <PubSubClient.h>
const char *ssid = "mqttarduino";
const char *password = "arduinomqtt";
int gain = 2;
// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "latihan1/capaian";
const char *topic2 = "nama";
int set_th = 0;


const char *mqtt_username = "qwerty";
const char *mqtt_password = "public";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

int sensor_grove = 32;
int sensor_myo = 34;
int value = 0;
int signal_wave = 0;
int kalibrator = 0;
int initial_value = 0;
const int arraySize = 50;  // Ukuran array
int dataArray[arraySize];  // Array untuk menyimpan data
int currentIndex = 0;      // Indeks saat ini untuk menulis data baru
int offset = 100;
bool status_1 = false;
bool status_2 = false;
bool status_3 = false;
bool ready = false;
int progress_bar = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  analogReadResolution(12);

  WiFi.mode(WIFI_STA);
  pinMode(sensor_grove, INPUT);
  pinMode(sensor_myo, INPUT);

  for (int i = 0; i < arraySize; i++) {
    dataArray[i] = 0;
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
    String client_id = "esp8266-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
    client.publish(topic2, "pasien_1");
  }
  //setup wifi
  // setup_wifi();
  // client.setServer(mqtt_broker, 1883);
  // client.setCallback(callback);
  // client.subscribe("latihan1/capaian");
}
void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}
char msg_out[20];
int grovesensorVal;
void loop() {
  delay(1);
  int rawSignal = analogRead(sensor_grove);
  float rectifiedSignal = abs(rawSignal - 512);

  // Smoothing (Moving Average)
  signalBuffer[bufferIndex] = rectifiedSignal;
  bufferIndex = (bufferIndex + 1) % sampleSize;

  float smoothedSignal = 0;
  for (int i = 0; i < sampleSize; i++) {
    smoothedSignal += signalBuffer[i];
  }
  smoothedSignal /= sampleSize;
  grovesensorVal = smoothedSignal;
  dataArray[currentIndex] = grovesensorVal;
  int grove_kalibrasi = grovesensorVal - kalibrator;
  while (Serial.available()) {


    char data_from_display = Serial.read();
    if (data_from_display == 'T') {
      for (int i = 0; i < arraySize; i++) {
        value += dataArray[i];
      }
      kalibrator = tare(dataArray, arraySize) - offset;
      initial_value = kalibrator;
      set_th = (grovesensorVal - kalibrator) * gain;
      delay(3000);
      ready = true;
    }
  }
  // signal_wave = map(grove_kalibrasi, 0, 2048, 0, 800);
  signal_wave = grove_kalibrasi;
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


  currentIndex = (currentIndex + 1) % arraySize;
  // int progress_bar = map(grove_kalibrasi, 0, 1024, 0, 100);
  progress_bar = (map(grove_kalibrasi, 0, 800, 0, 100) * gain);
  Serial.print("j0.val=");
  Serial.print(progress_bar);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.print("val.val=");
  Serial.print(progress_bar);
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);

  if (grove_kalibrasi > (set_th + 0.05) && ready == true) {

    if (status_1 == false) {
      client.publish(topic, "1");
      // status_1 = true;
    }
    Serial.print("p0.pic=0");
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    delay(100);
    Serial.print("t2.bco=3982");
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
    delay(100);

    Serial.print("t5.txt=tercapai");
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
  }
  if (grove_kalibrasi > (set_th + 0.1) && ready == true) {
    if (status_2 == false) {
      client.publish(topic, "2");
      // status_2 = true;
    }


    Serial.print("p1.pic=0");
    sendTo();

    Serial.print("t3.bco=3982");
    sendTo();

    Serial.print("t6.txt=tercapai");
    sendTo();
  }
  if (grove_kalibrasi > (set_th + 0.15) && ready == true) {
    if (status_3 == false) {
      client.publish(topic, "3");
      // status_3 = true;
    }


    Serial.print("p2.pic=0");
    sendTo();

    Serial.print("t4.bco=3982");
    sendTo();

    Serial.print("t7.txt=tercapai");

    sendTo();
    //send light
  }
  client.loop();
  delay(10);
}


int tare(int data[], int size) {
  int value = 0;
  int send = 0;

  for (int i = 0; i < size; i++) {
    value += data[i];
  }
  send = (value / size);

  return send;
}
void sendTo() {
  Serial.write(0xff);
  Serial.write(0xff);
  Serial.write(0xff);
}