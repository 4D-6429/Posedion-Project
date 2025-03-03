#define R_EN 31
#define L_EN 30
#define R_PWM 2
#define L_PWM 3

void setup() {
  // put your setup code here, to run once:
  pinMode(R_EN, OUTPUT);
  pinMode(L_EN, OUTPUT);
  pinMode(R_PWM, OUTPUT);
  pinMode(L_PWM, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(R_EN, 1);
  digitalWrite(L_EN, 1);
  analogWrite(L_PWM, 255);
}
