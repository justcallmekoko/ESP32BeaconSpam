#include <WiFi.h>
#include "esp_wifi.h"


esp_err_t esp_wifi_80211_tx(wifi_interface_t ifx, const void *buffer, int len, bool en_sys_seq);

uint8_t channel = 0;

int r;

String alfa = "1234567890qwertyuiopasdfghjkklzxcvbnm QWERTYUIOPASDFGHJKLZXCVBNM_";
char* prefix = "M";
//byte channel;

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
uint8_t packet[128] = { 0x80, 0x00, //Frame Control 
                        0x00, 0x00, //Duration
                /*4*/   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, //Destination address 
                /*10*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //Source address - overwritten later
                /*16*/  0x01, 0x02, 0x03, 0x04, 0x05, 0x06, //BSSID - overwritten to the same as the source address
                /*22*/  0xc0, 0x6c, //Seq-ctl
                //Frame body starts here
                /*24*/  0x83, 0x51, 0xf7, 0x8f, 0x0f, 0x00, 0x00, 0x00, //timestamp - the number of microseconds the AP has been active
                /*32*/  0xFF, 0x00, //Beacon interval
                /*34*/  0x01, 0x04, //Capability info
                /* SSID */
                /*36*/  0x00
                };

void broadcastSSID(){

  // Min len of SSID - 1 for suf
  // Max len of SSID - 1 for suf
  int essid_len = random(7, 16);

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
  {
    packet[38+i] = rand_reg[i];
  }

  // Insert my tag
  
  for(int i = 0; i < ssidLen; i++) {
    packet[38 + rand_len + i] = prefix[i];
  }
  
  
  uint8_t postSSID[13] = {0x01, 0x08, 0x82, 0x84, 0x8b, 0x96, 0x24, 0x30, 0x48, 0x6c, //supported rate
                      0x03, 0x01, 0x04 /*DSSS (Current Channel)*/ };

  for(int i = 0; i < 12; i++) {
    packet[38 + fullLen + i] = postSSID[i];
  }

  //packet[50 + fullLen] = channel;

  packet[10] = packet[16] = random(256);
  packet[11] = packet[17] = random(256);
  packet[12] = packet[18] = random(256);
  packet[13] = packet[19] = random(256);
  packet[14] = packet[20] = random(256);
  packet[15] = packet[21] = random(256);

  int packetSize = 51 + fullLen;
  
  // Broadcast AP on each channel
  /*
  for(int c = 1;c<15;c++){
    packet[50 + fullLen] = c;
    esp_wifi_set_channel(c,WIFI_SECOND_CHAN_NONE);
    delay(1);  
    esp_wifi_80211_tx(WIFI_IF_AP, packet, packetSize, false);
    esp_wifi_80211_tx(WIFI_IF_AP, packet, packetSize, false);
    esp_wifi_80211_tx(WIFI_IF_AP, packet, packetSize, false);
    
    delay(1);  
  }//14 channels
  */

  // Broadcast AP on one random channel
  int c = random(1, 15);
  packet[50 + fullLen] = c;
  esp_wifi_set_channel(c,WIFI_SECOND_CHAN_NONE);
  delay(1);  
  esp_wifi_80211_tx(WIFI_IF_AP, packet, packetSize, false);
  esp_wifi_80211_tx(WIFI_IF_AP, packet, packetSize, false);
  esp_wifi_80211_tx(WIFI_IF_AP, packet, packetSize, false);
    
  delay(8);
  //digitalWrite(21, LOW);
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(100);
  Serial.println("Sending the stuff...");
  WiFi.mode(WIFI_AP_STA);
 
  //Set channel
  channel = random(1,14); 
  esp_wifi_set_promiscuous(true);
  esp_wifi_set_max_tx_power(78);
  
  //Select external antenna
  //pinMode(15,OUTPUT);
  pinMode(21,OUTPUT);
  digitalWrite(21, LOW);//ceramic/printed antenna on (default)
  //digitalWrite(15, LOW);//ceramic/printed antenna on (default)

}

void loop() {
  r = random(0,9999);
  //if(r < 3226){//20Tx, 80Rx
  //if(r < 4615){ //30Tx,70Rx
  //if(r < 5714){ //40Tx,60Rx
  //if(r < 6667){ //50Tx,50Rx
  //if(r < 7500){ //60Tx,40Rx
  //if(r < 8235){ //70Tx,30Rx
  if(r < 8889) //80Tx,20Rx
  {
    delay(10);   
    broadcastSSID(); 
  }
}
