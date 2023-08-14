#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CTBot.h>


CTBot myBot;

 String ssid = ""; //isi dengan wifi
 String pass = ""; // isi dengan password

 String token = ""; // isi dengan Bot Token telegram
 const int id = 123456789; //isi dengan id telegram

 const int PUMP_PIN = D4;
 const int analogPin = A0;

LiquidCrystal_I2C lcd(0x27, 16, 2);

int brightness = 100;
int statusMode = 0;
int pompaStatus = 0;
int kelembapan;

void setup() {
  
  Serial.begin(115200);
  Serial.println("Memulai Telegram Bot");
  Serial.println("Memulai...");
  Wire.begin();

  pinMode(PUMP_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, HIGH);

  lcd.begin(16, 2);
  lcd.clear();
  
  myBot.wifiConnect(ssid, pass);       
  myBot.setTelegramToken(token);   

  if (myBot.testConnection())  { 
     Serial.println("\nKoneksi Ke Telegram BOT Berhasil!"); 
  }  else {
     Serial.println("\nTidak Terkoneksi Ke Telegram BOT");
 }
}

void sensor (){
    kelembapan = analogRead(analogPin);
}

void loop() {
  
  String statusKelembapan;
  Serial.print("Kelembapan Tanah: ");
  Serial.println(kelembapan);
  lcd.setCursor(0, 0);
  lcd.print(kelembapan);
  
  brightness = map(kelembapan, 0, 1023, 0, 255);
  lcd.setBacklight(brightness);
  
sensor(); 
if (statusMode) {
    if (kelembapan > 500) {
        digitalWrite(PUMP_PIN, LOW);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tanah: Kering     ");
        lcd.setCursor(0, 1);
        lcd.print("pompa berjalan");
    }
    else if (kelembapan <= 400) {
        digitalWrite(PUMP_PIN, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tanah: Normal     ");
        lcd.setCursor(0, 1);
        lcd.print("pompa mati");
    }
    else {
        digitalWrite(PUMP_PIN, HIGH);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tanah: Lembap     ");
        lcd.setCursor(0, 1);
        lcd.print("pompa mati");
    }
}

else {
  
        lcd.setCursor(0, 0);
        lcd.print("Kondisi Tanah      ");
        lcd.setCursor(0, 1);
        
        if (kelembapan >= 500) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Kondisi Tanah");
            lcd.setCursor(0, 1);
            lcd.print("Tanah : Kering");
        }
        else if (kelembapan <= 400) {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Kondisi Tanah");
            lcd.setCursor(0, 1);
            lcd.print("Tanah : Normal");
        }
        else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Kondisi Tanah");
            lcd.setCursor(0, 1);
            lcd.print("Tanah : Lembap");
        }
}
      
 delay(1000);

TBMessage msg;
 if (myBot.getNewMessage(msg)) { 
                                                          
      if (msg.text.equalsIgnoreCase("status")){                                  
      myBot.sendMessage(msg.sender.id, "Status saat ini : " + statusKelembapan);
      Serial.println("pesan terkirim");
          
    } else if (msg.text.equalsIgnoreCase("otomatis")) {
      String otomatis = "MODE OTOMATIS DIAKTIFKAN \n\n";
      otomatis += "*Pompa akan menyala otomatis jika tanah kering\n";
      otomatis += "*Pompa akan mati otomatis jika tanah normal/lembap\n";
      otomatis += "===============================\n";
      otomatis += "status     : Untuk Informasi kelembapan";
      myBot.sendMessage(msg.sender.id,otomatis);
      statusMode = 1;
      
    } else if (msg.text.equalsIgnoreCase("manual")){
      String manual = "MODE MANUAL DIAKTIFKAN\n\n";
      manual += "pompa on   : Untuk Menyalakan Pompa\n";
      manual += "pompa off  : Untuk Mematikan Pompa\n";
      manual += "status     : Untuk Informasi kelembapan\n";
      myBot.sendMessage(msg.sender.id,manual);
      statusMode = 0;
      
    } else if (msg.text.equalsIgnoreCase("pompa on")){
      digitalWrite(PUMP_PIN, LOW); // Menghidupkan Pompa
      myBot.sendMessage(msg.sender.id,"Pompa dinyalakan", "");
      
    } else if (msg.text.equalsIgnoreCase("pompa off")){
      digitalWrite(PUMP_PIN, HIGH);  // Mematikan Pompa
      myBot.sendMessage(msg.sender.id,"Pompa dimatikan", "");
      
    } else if (msg.text.equalsIgnoreCase("/start")) { 
      String welcome = "SELAMAT DATANG \n\n";
      welcome += "PENYIRAM TANAMAN OTOMATIS BERBASIS TELEGRAM \n\n";
      welcome += "SILAHKAN PILIH PERINTAH DI BAWAH INI : \n\n";
      welcome += "otomatis  : Untuk Mode Otomatis\n";
      welcome += "manual    : Untuk Mode Manual\n";
      welcome += "status    : Untuk Informasi kelembapan\n\n";
      welcome += "===============================\n\n";
      welcome += "MODE MANUAL SAAT INI AKTIF \n";
      welcome += "SILAHKAN PILIH PERINTAH DI BAWAH INI : \n\n";
      welcome += "pompa on  : Untuk Menyalakan Pompa\n";
      welcome += "pompa off : Untuk Mematikan Pompa\n";
      myBot.sendMessage(msg.sender.id,welcome);
      
    }else {   
     myBot.sendMessage(msg.sender.id, "ERROR : Perintah Tidak Dikenali");
     Serial.println("\nUser mengirim perintah yang tidak dikenal\n");
    }
 }
}
