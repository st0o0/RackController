#include <defines.h>

FanController Fan_Left(Fan_Left_SENS_PIN, SENSOR_THRESHOLD1, Fan_Left_PWM_PIN);
FanController Fan_Right(Fan_Right_SENS_PIN, SENSOR_THRESHOLD2, Fan_Right_PWM_PIN);

// LCD:
LiquidCrystal lcd(9, 8, 7, 6, 5, 4);

bool Switch = true;

byte Heart[8] = {
    0b00000,
    0b01010,
    0b11111,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
    0b00000};

byte fan_curve(float temp)
{
    int spd = 100 * (temp - FAN_OFF_TEMP) / (FAN_MAX_TEMP - FAN_OFF_TEMP);
    return byte(min(max(0, spd), 100));
}

float read_temp(int pin)
{
    return ((analogRead(pin) * 5000) / 1000) / 10;
}

void set_fanspeed(byte spd)
{
    Fan_Left.setDutyCycle(spd);
    Fan_Right.setDutyCycle(spd);
}

void setup()
{
    pinMode(SWITCH_PIN, INPUT);
    Serial.begin(9600);
    Fan_Right.begin();
    Fan_Left.begin();
    lcd.begin(16, 2);
    lcd.createChar(0, Heart);
    lcd.setCursor(0, 0);
    lcd.print("FanController");
    lcd.setCursor(0, 1);
    lcd.print("has started");
    lcd.setCursor(15, 0);
    lcd.write((byte)0);
    lcd.setCursor(15, 1);
    lcd.write((byte)0);
    delay(UPDATE_INTERVAL);
    Fan_Left.setDutyCycle(0);
    Fan_Right.setDutyCycle(0);
}

void loop()
{
    byte spd;
    float Temp0 = read_temp(TEMP0_PIN);
    float Temp1 = read_temp(TEMP1_PIN);
    float Temp = min(100, max(0, (Temp0 - Temp1)));
    char cTemp0[8];
    char cTemp1[8];
    char cTemp[8];
    dtostrf(Temp0, 0, 1, cTemp0);
    dtostrf(Temp1, 0, 1, cTemp1);
    dtostrf(Temp, 0, 1, cTemp);
    if (digitalRead(SWITCH_PIN) == 1)
    {
        spd = map(analogRead(POTI_PIN), 0, 1023, 0, 100);
    }
    else
    {
        spd = fan_curve(Temp);
    }
    set_fanspeed(spd);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("TMP:");
    lcd.print(cTemp);
    lcd.setCursor(0, 2);
    lcd.print("SPD:");
    lcd.print(spd);
    lcd.print("%");
    lcd.setCursor(8, 1);
    lcd.write((byte)0);
    lcd.setCursor(8, 0);
    lcd.write((byte)0);
    if (Switch)
    {
        unsigned int rpms_right = Fan_Right.getSpeed();
        unsigned int rpms_left = Fan_Left.getSpeed();
        lcd.setCursor(9, 0);
        lcd.print("FL:");
        lcd.print(rpms_left);
        lcd.setCursor(9, 1);
        lcd.print("FR:");
        lcd.print(rpms_right);
        if (digitalRead(SWITCH_PIN) == 0)
        {
            Switch = false;
        }
    }
    else
    {
        lcd.setCursor(9, 0);
        lcd.print("T0:");
        lcd.print(cTemp0);
        lcd.setCursor(9, 1);
        lcd.print("T1:");
        lcd.print(cTemp1);
        Switch = true;
    }
    delay(UPDATE_INTERVAL);
}