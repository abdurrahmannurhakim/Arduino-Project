#include <RTClib.h>
#include <elapsedMillis.h>
elapsedMillis Elapshed, TimeDiscrase;
RTC_DS1307 rtc;
DateTime now;
float pressure, out, endTime, val_zero, av_pressure, sum_pressure;
int sendsecond, sendminute, sendhour, sum_set_time, clone_Timer;
int set_second, set_minute, set_hour, Fsec, Fmin, cntDiscrase;
int startsecond, startminute, starthour, flag_stop, av_count;
int clone_set_second, clone_set_minute, clone_set_hour;
int Timer, zero_set, pressure_set, sendstatus, Pause;
int flag_Serial, flag_time1, flag_time2, cnt_time;
float cv, sv, shv, IpX7Speed, IpX8Speed;
int raw_press, flag, start_calculate;
int flag_sec, flag_minute, flag_hour;
String IN;

void setup() {
  // put your setup code here, to run once:
  Initialing();
  pinMode(12, INPUT);//SENSOR
  val_zero = analogRead(12);
  ledcAttachPin(4, 0); // assign a led pins to a channel
  ledcSetup(0, 4000, 8); // 12 kHz PWM, 8-bit resolution
  pinMode(26, OUTPUT);//ALARM
  pinMode(27, OUTPUT);//SELENOID
}

void loop() {
  raw_press = analogRead(12);
  pressure = pressureCal((float)raw_press);
  set_MAX_Press();
  I2c_timer();
  call_pressure();
  buzzer();
  sendToHMI();
}

void SerialCheck() {
  
    Serial.print(sum_set_time-clone_Timer); Serial.print("t\t");
    Serial.print(sum_set_time); Serial.print("t\t");
    Serial.print(clone_Timer); Serial.print("t\t");
    Serial.print(endTime); Serial.print("L\t");
    Serial.print(av_pressure); Serial.print("P\t");
    Serial.print(pressure); Serial.print("PV\t");
    Serial.print(zero_set); Serial.print("Z\t");
    Serial.print(Timer); Serial.print("T\t");
    Serial.print(startsecond); Serial.print("S\t");
    Serial.print(startminute); Serial.print("M\t");
    Serial.print(starthour); Serial.print("H\t");
    Serial.print(set_second); Serial.print("sS\t");
    Serial.print(set_minute); Serial.print("sM\t");
    Serial.print(set_hour); Serial.print("sH\t");
    Serial.print(sendsecond); Serial.print("sdS\t");
    Serial.print(sendminute); Serial.print("sdM\t");
    Serial.print(sendhour); Serial.print("sdH\n");
  
  //average_all();
  Serial.print(((float)Elapshed) / 1000.00); Serial.print("\t");
  Serial.println(pressure);
}
/*
  void average_all() {
  shv = averageF(sv, cv);
  sv += pressure;
  cv++;
  }
*/
void sendToHMI() {
  Serial2.print("pressure_val=");
  Serial2.print((int)pressure);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  Serial2.print("second=");
  Serial2.print((int)sendsecond);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  Serial2.print("minute=");
  Serial2.print((int)sendminute);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  Serial2.print("hour=");
  Serial2.print((int)sendhour);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2.write(0xff);

  Serial2.print("endTime=");
  Serial2.print((int)endTime);
  Serial2.write(0xff);
  Serial2.write(0xff);
  Serial2
  Serial2.write(0xff);
}

float pressureCal(int input) {
  float out;
  if (endTime == 0) {
    out = (input - val_zero) * 60.134; //calibration factor
  } else {
    out = av_pressure;
  }
  if (out > -500) {
    out = 0;
  } else {}
  return out;
}

void set_MAX_Press() {
  timeCount();
  if (pressure_set == 9800 && flag == 1) { //IP67
    setIPX7Speed();
    digitalWrite(27, HIGH);
    ledcWrite(0, 80 - IpX7Speed);
  } else if (pressure_set == 21000 && flag == 1) { //IP68
    setIPX8Speed();
    digitalWrite(27, HIGH);
    ledcWrite(0, 130 - IpX8Speed);
  } else {
    digitalWrite(27, LOW);
    ledcWrite(0, 0);
  }
}

void timeCount() {
  if (TimeDiscrase >= 100) {
    cntDiscrase++;
    if (cntDiscrase > 1000) {
      cntDiscrase = 0;
    }
    TimeDiscrase = 0;
  }
}

void setIPX7Speed() {
  if (pressure < -11000 && cntDiscrase % 2 == 0) {
    IpX7Speed += 0.1;
  } else if (pressure_set == 0) {
    IpX7Speed = 0;
  } else {}
}

void setIPX8Speed() {
  if (pressure < -21000 && cntDiscrase % 2 == 1) {
    IpX8Speed += 0.1;
  } else if (pressure_set == 0) {
    IpX8Speed = 0;
  } else {}
}

void call_pressure() {
  av_setting();
  /*
    if (start_calculate == 1) {
    if (pressure_set == 9800 && av_pressure > -9560 && flag_stop == 0) {
      endTime = 1;
      flag_stop = 1;
    } else if (pressure_set == 21000 && av_pressure > -19750 && flag_stop == 0) {
      endTime = 1;
      flag_stop = 1;
    } else if (pressure_set == 9800 && av_pressure <= -9560 && flag_stop == 0) {
      endTime = 2;
      flag_stop = 1;
    } else if (pressure_set == 21000 && av_pressure <= -19750 && flag_stop == 0) {
      endTime = 2;
      flag_stop = 1;
    } else {
    }
    }
  */

  if (start_calculate == 1) {
    if (pressure_set == 9800 && av_pressure > -10000 && flag_stop == 0) {
      endTime = 1;
      flag_stop = 1;
    } else if (pressure_set == 21000 && av_pressure > -20000 && flag_stop == 0) {
      endTime = 1;
      flag_stop = 1;
    } else if (pressure_set == 9800 && av_pressure <= -10000 && flag_stop == 0) {
      endTime = 2;
      flag_stop = 1;
    } else if (pressure_set == 21000 && av_pressure <= -20000 && flag_stop == 0) {
      endTime = 2;
      flag_stop = 1;
    } else {
    }
  }
}

float averageF(float IN_SUM, float IN_devide)
{
  float IN_value;
  IN_value = IN_SUM / IN_devide;
  return IN_value;
}

void av_setting() {
  time_range();
  if (sum_set_time - clone_Timer <= 5) {
    av_pressure = averageF(sum_pressure, av_count);
    sum_pressure += pressure;
    av_count++;
  } else {
  }
}

void time_range() {
  sum_set_time = clone_set_second + 60 * clone_set_minute + 3600 * clone_set_hour;
}

void buzzer() {
  if (endTime == 1) {
    high_note_long();
  } else if (endTime == 2) {
    high_note_flip();
  } else {
    low();
  }
}

void low() {
  digitalWrite(26, LOW);
}

void high_note_flip() {
  if (Elapshed >= 500) {
    cnt_time++;
    Elapshed = 0;
  } else {}

  if (cnt_time % 2 == 0) {
    digitalWrite(26, HIGH);
  } else if (cnt_time % 2 == 1) {
    digitalWrite(26, LOW);
  } else {}
}
void high_note_long() {
  digitalWrite(26, HIGH);
}
