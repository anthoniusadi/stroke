// //mqtt
#include <WiFi.h>
#include <PubSubClient.h>
const char *ssid = "IoT_stroke";
const char *password = "iotstroke";

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "latihan1/capaian";
const char *topic2 = "nama";



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
int offset = 200;
bool status_1 = false;
bool status_2 = false;
bool status_3 = false;
bool ready = false;
int progress_bar = 0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
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

// void setup_wifi() {
//   delay(10);
//   // Serial.println();
//   // Serial.print("Connecting to ");
//   // Serial.println(ssid);

//   WiFi.begin(ssid, password);

//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     // Serial.print(".");
//   }

//   // Serial.println("");
//   // Serial.println("WiFi connected");
//   // Serial.println("IP address: ");
//   // Serial.println(WiFi.localIP());
// }

// void callback(char *topic, byte *payload, unsigned int length) {
//   // Serial.print("Message arrived in topic: ");
//   Serial.println(topic);
//   // Serial.print("Message:");
//   for (int i = 0; i < length; i++) {
//     Serial.print((char)payload[i]);
//   }
//   Serial.println();
//   // Serial.println("-----------------------");
// }
// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     // Serial.print("Attempting MQTT connection...");
//     // Attempt to connect
//     if (client.connect("esp32")) {
//       // Serial.println("connected");
//       // Subscribe
//       client.subscribe("latihan1/capaian");
//     } else {
//       // Serial.print("failed, rc=");
//       // Serial.print(client.state());
//       // Serial.println(" try again in 5 seconds");
//       // Wait 5 seconds before retrying
//       delay(5000);
//     }
// }
// }
//char data_from_display;
char msg_out[20];
void loop() {
  // if (!client.connected()) {
  //   reconnect();
  // }

  delay(1);
  int grove = analogRead(sensor_grove);
  int myo = analogRead(sensor_myo);
  int myosensorVal = map(myo, 0, 4096, 0, 2048);
  int grovesensorVal = map(grove, 0, 4096, 0, 2048);
  dataArray[currentIndex] = grovesensorVal;
  int grove_kalibrasi = grovesensorVal - kalibrator;

  Serial.print(grove_kalibrasi);


  while (Serial.available()) {


    char data_from_display = Serial.read();
    if (data_from_display == 'T') {
      for (int i = 0; i < arraySize; i++) {
        value += dataArray[i];
      }
      kalibrator = tare(dataArray, arraySize) - offset;
      initial_value = kalibrator;

      delay(3000);
      ready = true;
    }
  }

  signal_wave = map(grove_kalibrasi, 0, 2048, 0, 800);
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
  progress_bar = map(grove_kalibrasi, 0, 450, 0, 100);
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

  if (progress_bar > 55 && ready==true) {

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
  if (progress_bar > 60 && ready==true) {
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
  if (progress_bar > 65 && ready==true) {
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
// String command = "n0.val=" + String(grove_kalibrasi) + "\xff\xff\xff";
// Serial.print(command);