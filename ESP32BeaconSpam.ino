#include <WiFi.h>
#include "esp_wifi.h"


esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);

String alfa = "1234567890qwertyuiopasdfghjkklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM_";
//String prefix = "ESP8266_Fake_";
char* prefix = "G";
byte channel;

// Beacon Packet buffer
uint8_t my_packet[128] = { 0x80, 0x00, 0x00, 0x00, 
                /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
                /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
                /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
                /*22*/  0xc0, 0x6c, 
                /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, 
                /*32*/  0x64, 0x00, 
                /*34*/  0x01, 0x04, 
                /* SSID */
                /*36*/  0x00, 0x12, 0x72, 0x72, 0x72, 0x72, 0x72, 0x5f, 0x52, 0x65, 0x64, 0x54, 0x65, 0x61, 0x6d, 0x5f, 0x46, 0x61, 0x6b, 0x65,
                        0x01, 0x08, 0x82, 0x84,
                        0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, 0x03, 0x01, 
                /*56*/  0x04};                       

// barebones packet
uint8_t packet[128] = { 0x80, 0x00, 0x00, 0x00, //Frame Control, Duration
                /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address 
                /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address - overwritten later
                /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID - overwritten to the same as the source address
                /*22*/  0xc0, 0x6c, //Seq-ctl
                /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp - the number of microseconds the AP has been active
                /*32*/  0x64, 0x00, //Beacon interval
                /*34*/  0x01, 0x04, //Capability info
                /* SSID */
                /*36*/  0x00
                };


void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  Serial.println("Sending the stuff...");
  WiFi.mode(WIFI_AP_STA);
 
  //Set channel
  //channel = random(1,14); 
  //wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  //esp_wifi_init(&cfg);
  //esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_promiscuous(true);
  //esp_wifi_set_max_tx_power(78);
  esp_wifi_set_max_tx_power(82);
  
  //Select external antenna
  //pinMode(15,OUTPUT);
  pinMode(21,OUTPUT);
  digitalWrite(21, LOW);//ceramic/printed antenna on (default)
  //digitalWrite(15, LOW);//ceramic/printed antenna on (default)
}

// Function to send beacons with fixed ESSID length
void broadcastSSID3() {
  channel = random(1,12); 
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  delay(1);  

  // Randomize SRC MAC
  packet[10] = packet[16] = random(256);
  packet[11] = packet[17] = random(256);
  packet[12] = packet[18] = random(256);
  packet[13] = packet[19] = random(256);
  packet[14] = packet[20] = random(256);
  packet[15] = packet[21] = random(256);

  packet[37] = 6;
  
  
  // Randomize SSID (Fixed size 6. Lazy right?)
  packet[38] = alfa[random(65)];
  packet[39] = alfa[random(65)];
  packet[40] = alfa[random(65)];
  packet[41] = alfa[random(65)];
  packet[42] = alfa[random(65)];
  packet[43] = alfa[random(65)];
  
  packet[56] = channel;

  uint8_t postSSID[13] = {0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
                      0x03, 0x01, 0x04 /*DSSS (Current Channel)*/ };



  // Add everything that goes after the SSID
  for(int i = 0; i < 12; i++) 
    packet[38 + 6 + i] = postSSID[i];

  esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
  esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
  esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
}


// Function to send beacons with fixed ESSID length
void broadcastSSID2() {
  channel = random(1,12); 
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  delay(1);  

  // Randomize SRC MAC
  my_packet[10] = my_packet[16] = random(256);
  my_packet[11] = my_packet[17] = random(256);
  my_packet[12] = my_packet[18] = random(256);
  my_packet[13] = my_packet[19] = random(256);
  my_packet[14] = my_packet[20] = random(256);
  my_packet[15] = my_packet[21] = random(256);

  my_packet[37] = 6;
  
  
  // Randomize SSID (Fixed size 6. Lazy right?)
  my_packet[38] = alfa[random(65)];
  my_packet[39] = alfa[random(65)];
  my_packet[40] = alfa[random(65)];
  my_packet[41] = alfa[random(65)];
  my_packet[42] = alfa[random(65)];
  //packet[43] = alfa[random(65)];
  
  my_packet[56] = channel;

  uint8_t postSSID[13] = {0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
                      0x03, 0x01, 0x04 /*DSSS (Current Channel)*/ };



  // Add everything that goes after the SSID
  for(int i = 0; i < 12; i++) 
    my_packet[38 + 6 + i] = postSSID[i];

  esp_wifi_80211_tx(WIFI_IF_AP, my_packet, sizeof(my_packet), false);
  esp_wifi_80211_tx(WIFI_IF_AP, my_packet, sizeof(my_packet), false);
  esp_wifi_80211_tx(WIFI_IF_AP, my_packet, sizeof(my_packet), false);
}

// Function to send beacons with random ESSID length
void broadcastSSID() {
  int result;
  channel = random(1,12); 
  esp_wifi_set_channel(channel, WIFI_SECOND_CHAN_NONE);
  delay(1);  

  // Randomize SRC MAC
  packet[10] = packet[16] = random(256);
  packet[11] = packet[17] = random(256);
  packet[12] = packet[18] = random(256);
  packet[13] = packet[19] = random(256);
  packet[14] = packet[20] = random(256);
  packet[15] = packet[21] = random(256);

  
  /////////////////////////////
  int essid_len = random(6, 10);
  //int essid_len = 6;

  // random prefix to beacon essid
  uint8_t rand_reg[essid_len] = {};
  for (int i = 0; i < essid_len; i++)
    rand_reg[i] = alfa[random(65)];

  int ssidLen = strlen(prefix);
  int rand_len = sizeof(rand_reg);
  int fullLen = ssidLen + rand_len;
  packet[37] = fullLen;

  // Insert random prefix
  for (int i = 0; i < rand_len; i++)
    packet[38+i] = rand_reg[i];

  // Insert my tag
  for(int i = 0; i < ssidLen; i++)
    packet[38 + rand_len + i] = prefix[i];

  /////////////////////////////
  
  packet[50 + fullLen] = channel;

  uint8_t postSSID[13] = {0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
                      0x03, 0x01, 0x04 /*DSSS (Current Channel)*/ };



  // Add everything that goes after the SSID
  for(int i = 0; i < 12; i++) 
    packet[38 + fullLen + i] = postSSID[i];
  

  result = esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
  result = esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
  result = esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
  result = esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
  result = esp_wifi_80211_tx(WIFI_IF_AP, packet, sizeof(packet), false);
}



void loop() {
  //broadcastSSID(); // Random length SSID (slowest)
  //broadcastSSID2(); // With red team beacon (2nd fastest)
  broadcastSSID3(); // 6 character SSID (fastest)
  delay(1);
}
