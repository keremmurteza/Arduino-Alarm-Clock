#include <DS3231.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>
LiquidCrystal_I2C lcd(0x27,16,2);
File myFile;
#define bt_clock  A0
#define bt_up     A1
#define bt_down   A2
#define bt_timer  A3
#define buzzer 8

DS3231  rtc(SDA, SCL);

Time  t;

int hh = 0, mm = 0, ss = 0, dd = 0, bb = 0, set_day;
int yy = 0;
String Day = "  ";

int StartHH  = 0, StartMM  = 0, FinishHH  = 0, FinishMM  = 0, setMode = 0, setAlarm = 0, alarmMode=1;
int fark1h, fark2h, fark3h, fark4h, fark1m, fark2m, fark3m, fark4m, fark1t,fark2t,fark3t,fark4t, yakinh, yakinm, smallest;
int t1h,t2h,t3h,t4h,t1m,t2m,t3m,t4m;
int Start1HH, Start1MM, Finish1HH, Finish1MM;
int Start2HH, Start2MM, Finish2HH, Finish2MM;
int Start3HH, Start3MM, Finish3HH, Finish3MM;
int Start4HH, Start4MM, Finish4HH, Finish4MM;

int timer1, timer2, timer3, timer4;

int stop =0, mode=0, flag=0;

void setup(){

Serial.begin(9600);
while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  SD.begin();
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");
  

rtc.begin();

pinMode(bt_clock, INPUT_PULLUP);
pinMode(bt_up,    INPUT_PULLUP);
pinMode(bt_down,  INPUT_PULLUP);
pinMode(bt_timer, INPUT_PULLUP);
pinMode(buzzer, OUTPUT);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);

  lcd.setCursor (0,0);
  lcd.print(" Real Time Clock ");
  delay (200);
  lcd.clear();

stop=EEPROM.read(50);  
if(stop==0){  
}else{
mode=1;WriteEeprom ();delay (20);
mode=2;WriteEeprom ();delay (20);
mode=3;WriteEeprom ();delay (20);
mode=4;WriteEeprom ();delay (20);
mode=0;
}
EEPROM.write(50,0);


ReadEeprom();

}

void loop(){  
t = rtc.getTime();
Day = rtc.getDOWStr(1);

if (setMode == 0){
hh = t.hour,DEC;
mm = t.min,DEC;
ss = t.sec,DEC;
dd = t.date,DEC;
bb = t.mon,DEC;
yy = t.year,DEC;
}  

  if(setAlarm==0){
  lcd.setCursor(0,0); 
  lcd.print((hh/10)%10);
  lcd.print(hh % 10); 
  lcd.print(":");
  lcd.print((mm/10)%10);
  lcd.print(mm % 10);
  lcd.print(":");
  lcd.print((ss/10)%10);
  lcd.print(ss % 10); 
  lcd.print("  T:"); 
  lcd.print(rtc.getTemp(),0);
  lcd.write(223); 
  lcd.print("C");
  lcd.print("  "); 

  lcd.setCursor(1,1);
  lcd.print(Day);
  lcd.print(" ");
  lcd.print((dd/10)%10);
  lcd.print(dd % 10); 
  lcd.print("/");
  lcd.print((bb/10)%10);
  lcd.print(bb % 10);
  lcd.print("/"); 
  lcd.print((yy/1000)%10);
  lcd.print((yy/100)%10);
  lcd.print((yy/10)%10);
  lcd.print(yy % 10);

   if (timer1==1){
  t1h=Start1HH; t1m=Start1MM;
  if (t1h>=hh){ fark1h=(t1h-hh);}
  else{t1h+=24;
  fark1h=(t1h-hh);}
  if (t1m>=mm){ fark1m=(t1m-mm);}
  else{t1m+=60;
  fark1m=(t1m-mm);}
  fark1t=(fark1h*60)+fark1m;
 }
 else{
  t1h=999;
  fark1h=999;
  fark1t=999;
 }

    if (timer2==1){
  t2h=Start2HH; t2m=Start2MM;
  if (t2h>=hh){ fark2h=(t2h-hh);}
  else{t2h+=24;
  fark2h=(t2h-hh);}
  if (t2m>=mm){ fark2m=(t2m-mm);}
  else{t2m+=60;
  fark2m=(t2m-mm);}
  fark2t=(fark2h*60)+fark2m;
 }
 else{
  t2h=999;
  fark2h=999;
  fark2t=999;
 }

      if (timer3==1){
  t3h=Start3HH; t3m=Start3MM;
  if (t3h>=hh){ fark3h=(t3h-hh);}
  else{t3h+=24;
  fark3h=(t3h-hh);}
  if (t3m>=mm){ fark3m=(t3m-mm);}
  else{t3m+=60;
  fark3m=(t3m-mm);}
  fark3t=(fark3h*60)+fark3m;
 }
 else{
  t3h=999;
  fark3h=999;
  fark3t=999;
 }

    if (timer4==1){
  t4h=Start4HH; t4m=Start4MM;
  if (t4h>=hh){ fark4h=(t4h-hh);}
  else{t4h+=24;
  fark4h=(t4h-hh);
    }
  if (t4m>=mm){ fark4m=(t4m-mm);}
  else{t4m+=60;
  fark4m=(t4m-mm);}
  fark4t=(fark4h*60)+fark4m;
 }
 else{
  t4h=999;
  fark4h=999;
  fark4t=999;
 }

 if (timer1==1 || timer2==1 || timer3==1 || timer4==1){
  
smallest=fark1t;
if (fark2t<smallest) smallest=fark2t;
if (fark3t<smallest) smallest=fark3t;
if (fark4t<smallest) smallest=fark4t;
 if (smallest==fark1t){yakinh=Start1HH; yakinm=Start1MM;}
 if (smallest==fark2t){yakinh=Start2HH; yakinm=Start2MM;}
 if (smallest==fark3t){yakinh=Start3HH; yakinm=Start3MM;}
 if (smallest==fark4t){yakinh=Start4HH; yakinm=Start4MM;}
 if(digitalRead (bt_up) == 0 && setMode==0){
   lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("Next Alarm:");
 if(yakinh<10) lcd.print("0");
 lcd.print(yakinh);
 lcd.print(":");
 if(yakinm<10) lcd.print("0");
 lcd.print(yakinm);
 
  
  }

 }
 else{
 if(digitalRead (bt_up) == 0 && setMode==0){
   lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("No Alarm");  
 lcd.setCursor(0,1);
 lcd.print("Has Setted");
  }
  
  }
  
  }


setupClock();
setTimer();
delay (100);
blinking();


//Timer1 ON
if (timer1==1 && alarmMode==1 && hh==Start1HH && mm==Start1MM)  {digitalWrite(buzzer, HIGH); delay (500); digitalWrite(buzzer, LOW);}

//Timer2 ON
if (timer2==1 && alarmMode==1 && hh==Start2HH && mm==Start2MM)  {digitalWrite(buzzer, HIGH); delay (500); digitalWrite(buzzer, LOW);}

//Timer3 ON
if (timer3==1 && alarmMode==1 && hh==Start3HH && mm==Start3MM)  {digitalWrite(buzzer, HIGH); delay (500); digitalWrite(buzzer, LOW);}

//Timer4 ON
if (timer4==1 && alarmMode==1 && hh==Start4HH && mm==Start4MM)  {digitalWrite(buzzer, HIGH); delay (500); digitalWrite(buzzer, LOW);}

}


void blinking (){
//BLINKING SCREEN
//Set Clock
if (setAlarm <2 && setMode == 1){lcd.setCursor(0,0);  lcd.print("  ");}
if (setAlarm <2 && setMode == 2){lcd.setCursor(3,0);  lcd.print("  ");}
if (setAlarm <2 && setMode == 3){lcd.setCursor(6,0);  lcd.print("  ");}
if (setAlarm <2 && setMode == 4){lcd.setCursor(1,1);  lcd.print("   ");}
if (setAlarm <2 && setMode == 5){lcd.setCursor(5,1);  lcd.print("  ");}
if (setAlarm <2 && setMode == 6){lcd.setCursor(8,1);  lcd.print("  ");}
if (setAlarm <2 && setMode == 7){lcd.setCursor(11,1); lcd.print("    "); }

//Set Timer
if (setMode == 0 && setAlarm == 1 && mode==0){lcd.setCursor(2,1); lcd.print("  "); }
if (setMode == 0 && setAlarm == 2 && mode==0){lcd.setCursor(6,1); lcd.print("  "); }
if (setMode == 0 && setAlarm == 3 && mode==0){lcd.setCursor(10,1); lcd.print("  "); }
if (setMode == 0 && setAlarm == 4 && mode==0){lcd.setCursor(13,1); lcd.print("  "); }

if (setMode == 0 && setAlarm == 1 && mode>0){lcd.setCursor(11,0); lcd.print("  "); }
if (setMode == 0 && setAlarm == 2 && mode>0){lcd.setCursor(14,0); lcd.print("  "); }
if (setMode == 0 && setAlarm == 3 && mode>0){lcd.setCursor(11,1); lcd.print("  "); }
if (setMode == 0 && setAlarm == 4 && mode>0){lcd.setCursor(14,1); lcd.print("  "); }
}

void setupClock (void) {
   
    if (setMode == 8){
    lcd.setCursor (0,0);
    lcd.print ("Set Time Finish ");
    lcd.setCursor (0,1);
    lcd.print ("Set Date Finish ");
    delay (1000);
    rtc.setTime (hh, mm, ss);
    rtc.setDate (dd, bb, yy);  
    lcd.clear();
    setMode = 0;
    }
    
 if (setAlarm >0){alarmMode=0;}
    
 if(digitalRead (bt_clock) == 0 && flag==0) {flag=1;
 if(setAlarm>0){
  WriteEeprom(); WriteSD(); setAlarm=1; mode =5;}
 else{setMode = setMode+1;}
 }
  
 if(digitalRead (bt_timer) == 0 && flag==0){flag=1;
 if(setMode>0){setMode=8;}
  else{
  setAlarm = setAlarm+1;
  if(setAlarm>4){setAlarm=1; 
  WriteEeprom (); WriteSD();
  mode=mode+1;
  ReadEeprom();
  }
  } 
  lcd.clear();
  } 

    if(setAlarm == 1 && mode==5){
    lcd.clear();
    lcd.setCursor (0,0);
    lcd.print ("Set Timer Finish");
    lcd.setCursor (0,1);
    delay (2000); 
    lcd.clear();
    setAlarm=0;
    mode =0;
    alarmMode=1;
    }

if(digitalRead (bt_clock) == 1 && digitalRead (bt_timer) == 1){flag=0;}
  
 if(digitalRead (bt_up) == 0){                          
            if (setAlarm<2 && setMode==1)hh=hh+1; 
            if (setAlarm<2 && setMode==2)mm=mm+1;
            if (setAlarm<2 && setMode==3)ss=ss+1;
            if (setAlarm<2 && setMode==4)set_day=set_day+1;
            if (setAlarm<2 && setMode==5)dd=dd+1;
            if (setAlarm<2 && setMode==6)bb=bb+1;
            if (setAlarm<2 && setMode==7)yy=yy+1;
            //Timer
            if (mode==0 && setMode==0 && setAlarm==1)timer1=1;
            if (mode==0 && setMode==0 && setAlarm==2)timer2=1;
            if (mode==0 && setMode==0 && setAlarm==3)timer3=1;
            if (mode==0 && setMode==0 && setAlarm==4)timer4=1;
             
            if (mode>0 && setMode==0 && setAlarm==1)StartHH=StartHH+1;
            if (mode>0 && setMode==0 && setAlarm==2)StartMM=StartMM+1;


if(hh>23)hh=0;
if(mm>59)mm=0;
if(ss>59)ss=0;
if(set_day>7)set_day=0;
if(dd>31)dd=0;
if(bb>12)bb=0;
if(yy>2030)yy=2000;
if(StartHH>23)StartHH=0;
if(StartMM>59)StartMM=0;
if(FinishHH>23)FinishHH=0;
if(FinishMM>59)FinishMM=0;
rtc.setDOW(set_day);
 }        

if(digitalRead (bt_down) == 0){                                      
            if (setAlarm<2 && setMode==1)hh=hh-1; 
            if (setAlarm<2 && setMode==2)mm=mm-1;
            if (setAlarm<2 && setMode==3)ss=ss-1;
            if (setAlarm<2 && setMode==4)set_day=set_day-1;
            if (setAlarm<2 && setMode==5)dd=dd-1;
            if (setAlarm<2 && setMode==6)bb=bb-1;
            if (setAlarm<2 && setMode==7)yy=yy-1;
            //Timer
            if (mode==0 && setMode==0 && setAlarm==1)timer1=0;
            if (mode==0 && setMode==0 && setAlarm==2)timer2=0;
            if (mode==0 && setMode==0 && setAlarm==3)timer3=0;
            if (mode==0 && setMode==0 && setAlarm==4)timer4=0;
            
            if (mode>0 && setMode==0 && setAlarm==1)StartHH=StartHH-1;
            if (mode>0 && setMode==0 && setAlarm==2)StartMM=StartMM-1;


if(hh<0)hh=23;
if(mm<0)mm=59;
if(ss<0)ss=59;
if(set_day<0)set_day=7;
if(dd<0)dd=31;
if(bb<0)bb=12;
if(yy<0)yy=2030;
if(StartHH<0)StartHH=23;
if(StartMM<0)StartMM=59;
if(FinishHH<0)FinishHH=23;
if(FinishMM<0)FinishMM=59;
rtc.setDOW(set_day);
 }
 
}

void setTimer (){
//Timer
if (setMode == 0 && setAlarm >0 && mode>0){
  lcd.setCursor (0,0);
  lcd.print("Timer");
  lcd.print(mode);
  lcd.print(" On :");
  lcd.setCursor (11,0);
  lcd.print((StartHH/10)%10);
  lcd.print(StartHH % 10);
  lcd.print(":");
  lcd.print((StartMM/10)%10);
  lcd.print(StartMM % 10);


 }

if (setMode == 0 && setAlarm >0 && mode==0){
  lcd.setCursor (0,0);
  lcd.print(" T1  T2  T3  T4 ");
  lcd.setCursor (0,1);
    if(timer1==1){lcd.print("  A");}
  else{lcd.print("  D");}
   
    if(timer2==1){lcd.print("   A");}
  else{lcd.print("   D");}
  
    if(timer3==1){lcd.print("   A");}
  else{lcd.print("   D");}
  
    if(timer4==1){lcd.print("   A");}
  else{lcd.print("   D");}
}
}

void ReadEeprom() {
Start1HH=EEPROM.read(11);Start1MM=EEPROM.read(12);   
Start2HH=EEPROM.read(21);Start2MM=EEPROM.read(22); 
Start3HH=EEPROM.read(31);Start3MM=EEPROM.read(32); 
Start4HH=EEPROM.read(41);Start4MM=EEPROM.read(42); 

if(mode==1){StartHH=Start1HH, StartMM=Start1MM;}
if(mode==2){StartHH=Start2HH, StartMM=Start2MM;}
if(mode==3){StartHH=Start3HH, StartMM=Start3MM; }
if(mode==4){StartHH=Start4HH, StartMM=Start4MM;}

timer1=EEPROM.read(1);
timer2=EEPROM.read(2);
timer3=EEPROM.read(3);
timer4=EEPROM.read(4);
}

void WriteEeprom() {
if(mode==1){EEPROM.write(11,StartHH);EEPROM.write(12,StartMM);}
if(mode==2){EEPROM.write(21,StartHH);EEPROM.write(22,StartMM);}
if(mode==3){EEPROM.write(31,StartHH);EEPROM.write(32,StartMM);}
if(mode==4){EEPROM.write(41,StartHH);EEPROM.write(42,StartMM);}

EEPROM.write(1,timer1);
EEPROM.write(2,timer2);
EEPROM.write(3,timer3);
EEPROM.write(4,timer4);
}

void WriteSD(){
 myFile = SD.open("test.txt", FILE_WRITE);
 
   
   if(timer1==1){
if (myFile){
  myFile.println("Alarm 1:");
  myFile.print("t1h");
  }
   else{Serial.println("dosya açılamadı");}
   }
  else{Serial.println("Alarm 1 Kapalı");}



   if(timer2==1){
if (myFile){
  myFile.println("Alarm 2:");
  myFile.print(t2h);
  myFile.print(t2m);
 }
  else{Serial.println("dosya açılamadı");}
   }
  else{Serial.println("Alarm 2 Kapalı");}

     if(timer3==1){
if (myFile){
  myFile.println("Alarm 3:");
  myFile.print(int(t3h));
  myFile.print(t3m);
  }
   else{Serial.println("dosya açılamadı");}
   }
  else{Serial.println("Alarm 3 Kapalı");}

     if(timer4==1){
if (myFile){
  myFile.println("Alarm 4:");
  myFile.print(t4h);
  myFile.print(t4m);
  
  }
  else{Serial.println("dosya açılamadı");}
   }
  else{Serial.println("Alarm 4 Kapalı");}
  
  Serial.write(myFile.read());
  myFile.close();
}
