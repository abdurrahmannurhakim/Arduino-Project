
//============================================================================
// Name        : I2c_ESP32.ino
// Author      : Abdurrahman Nurhakim
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Vacum Test for IP7 or IP8 Test with Nextion HMI
//============================================================================

void Initialing() {
  Serial.begin(9600);
  Serial2.begin(115200);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void I2c_timer() {
  serialRead();
  time_control();
  set_timer();
  category_timer();
  SerialCheck();
}


void set_timer() {
  if (starthour == 1 && sendhour < set_hour)
  {
    start_calculate = 0;
    sendsecond = Timer;
    if (sendsecond > 59)
    {
      sendminute++;
      sendsecond = 0;
      Timer = 0;
      if (sendminute > 59)
      {
        sendhour++;
        sendminute = 0;
        Timer = 0;
      }
    }
  } else if (starthour == 1 && sendhour >= set_hour)
  {
    start_calculate = 0;
    starthour = 0;
    Timer = 0;
    startminute = 1;
    sendhour = set_hour;
    set_hour = 0;
  } else if (startminute == 1 && sendminute < set_minute)
  {
    start_calculate = 0;
    sendsecond = Timer;
    if (sendsecond > 59)
    {
      sendminute++;
      sendsecond = 0;
      Timer = 0;
    }
  } else if (startminute == 1 && sendminute >= set_minute) {
    start_calculate = 0;
    startminute = 0;
    Timer = 0;
    startsecond = 1;
    sendminute = set_minute;
    set_minute = 0;
  } else if (startsecond == 1 && sendsecond < set_second)
  {
    start_calculate = 0;
    sendsecond = Timer;
  } else if (startsecond == 1 && sendsecond >= set_second)
  {
    start_calculate = 1;
    sendsecond = set_second;
    set_second = 0;
  } else {}
}

void category_timer() {
  if (set_hour == 0 && set_minute == 0 && set_second == 0 && Fmin == 0 && Fsec == 0)
  {
    starthour = 0;
    startminute = 0;
    startsecond = 0;
    flag = 0;
  } else if (set_hour != 0)
  {
    starthour = 1;
    startminute = 0;
    startsecond = 0;
    Fmin = 1;
    Fsec = 0;
  } else if (set_hour == 0 && set_minute != 0)
  {
    starthour = 0;
    startminute = 1;
    startsecond = 0;
    Fsec = 1;
  } else if (set_hour == 0 && set_minute == 0 && Fmin == 1)
  {
    starthour = 0;
    startminute = 1;
    startsecond = 0;
    Fsec = 1;
  } else if (set_hour == 0 && set_minute == 0 && set_second != 0)
  {
    starthour = 0;
    startminute = 0;
    startsecond = 1;
    Fmin = 0;
  } else if (set_hour == 0 && set_minute == 0 && set_second == 0 && Fsec == 1)
  {
    starthour = 0;
    startminute = 0;
    startsecond = 1;
    Fmin = 0;
  } else {}
}

void cloneTimeSet() {
  if (clone_Timer <= sum_set_time) {
    clone_Timer++;
  } else if (clone_Timer > sum_set_time) {
    clone_Timer = sum_set_time;
  } else {}
}

void time_control() {
  now = rtc.now();
  if (Pause == 0) {
    if ((int)now.second() % 2 == 0 && flag_time1 == 1) {
      flag_time1 = 0;
      cloneTimeSet();
      Timer++;
    } else if ((int)now.second() % 2 == 1 && flag_time1 == 0) {
      flag_time1 = 1;
    } else if ((int)now.second() % 2 == 1 && flag_time2 == 1) {
      flag_time2 = 0;
      cloneTimeSet();
      Timer++;
    } else if ((int)now.second() % 2 == 0 && flag_time2 == 0) {
      flag_time2 = 1;
    } else {}
  } else {}

  if (zero_set == 1) {
    low();
    sum_pressure = 0;
    av_count = 0;
    av_pressure = 0;
    IpX7Speed = 0;
    IpX8Speed = 0;
    endTime = 0;
    cnt_time = 0;
    flag_stop = 0;
    Fmin = 0;
    Fsec = 0;
    Timer = 0;
    clone_Timer = 0;
    sendsecond = 0;
    sendminute = 0;
    sendhour = 0;
    start_calculate = 0;
    endTime = 0;
    Elapshed = 0;
    zero_set = 0;
    sum_set_time = 0;
  } else {}
}

void serialRead() {
  while (Serial2.available()) {
    char inChar = (char)Serial2
    .read();
    if (inChar == '#') {
      flag_Serial = 1;
    } else {}
    if (flag_Serial == 1) {
      if (inChar == 'A') {
        zero_set = IN.toInt();
        flag_Serial = 0;
        IN = "";
      } else if (inChar == '#') {
        IN = "";
      } else if (inChar == 'S') {
        set_second = IN.toInt();
        clone_set_second = set_second;
        flag_Serial = 0;
        IN = "";
      } else if (inChar == 'M') {
        set_minute = IN.toInt();
        clone_set_minute = set_minute;
        flag_Serial = 0;
        IN = "";
      } else if (inChar == 'H') {
        set_hour = IN.toInt();
        clone_set_hour = set_hour;
        flag_Serial = 0;
        IN = "";
      } else if (inChar == 'P') {
        pressure_set = IN.toInt();
        flag_Serial = 0;
        IN = "";
      } else if (inChar == 'I') {
        Pause = IN.toInt();
        flag_Serial = 0;
        IN = "";
      } else if (inChar == 'F') {
        flag = IN.toInt();
        flag_Serial = 0;
        IN = "";
      } else {
        IN.concat(inChar);
      }
    } else {}
  }
}
