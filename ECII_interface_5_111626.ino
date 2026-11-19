#include <string.h>

char comstring[8] = {};
char* endPtr;

const int DIR1 = 5;
const int DIR2 = 12;
const int STEP1 = 6;
const int STEP2 = 13;

int c = 0;
int i = 0;
int step_change;

float azChange;
float azCurrent = 0.0;
float azDelta;
float azNew;
float azTarget;

float elChange;
float elCurrent = 0.0;
float elDelta;
float elNew;
float elTarget;


void azimuth(float azTarget) {
  
  azTarget = min(azTarget, 90.0f);
  azTarget = max(azTarget, -90.0f);
  
  azChange = azTarget - azCurrent;
  step_change = round(azChange/(0.0375));
  azNew = step_change*0.0375 + azCurrent;
  azDelta = azTarget - azNew;

  Serial.print("| current azimuth: ");
  Serial.println(azCurrent);
  Serial.print("|  target azimuth: ");
  Serial.println(azTarget);
  Serial.print("|     new azimuth: ");
  Serial.println(azNew);
  Serial.print("|           delta: ");
  Serial.println(azDelta);
  Serial.print("| number of steps: ");
  Serial.println(step_change);
  Serial.println("|");

  if (step_change > 0) {
    digitalWrite(DIR1, HIGH);
  } else {
    digitalWrite(DIR1, LOW);
  }

  for (int j = 0; j < abs(step_change); j++) {
    digitalWrite(STEP1, HIGH);
    delay(1);
    digitalWrite(STEP1, LOW);
    delay(1);
    //Serial.print("step number: ");
    //Serial.println(j);
  }
  Serial.println("|-- azimuth change complete. ---");
  Serial.println();

  azCurrent = azNew;
}


void elevation(float elTarget) {

  elTarget = min(elTarget, 20.0f);
  elTarget = max(elTarget, 0.0f);
  
  elChange = elTarget - elCurrent;
  step_change = round(elChange/(0.0375));
  elNew = step_change*0.0375 + elCurrent;
  elDelta = elTarget - elNew;

  Serial.print("current elevation: ");
  Serial.println(elCurrent);
  Serial.print(" target elevation: ");
  Serial.println(elTarget);
  Serial.print("    new elevation: ");
  Serial.println(elNew);
  Serial.print("            delta: ");
  Serial.println(elDelta);
  Serial.print("  number of steps: ");
  Serial.println(step_change);
  Serial.println();

  if (step_change > 0) {
    digitalWrite(DIR2, HIGH);
  } else {
    digitalWrite(DIR2, LOW);
  }

  for (int j = 0; j < abs(step_change); j++) {
    digitalWrite(STEP2, HIGH);
    delay(1);
    digitalWrite(STEP2, LOW);
    delay(1);
    //Serial.print("step number: ");
    //Serial.println(j);
  }
  Serial.println("elevation change complete.");
  Serial.println();
  
  elCurrent = elNew;
}


void azi_query() {
  Serial.print("current azimuth: ");
  Serial.println(azCurrent);
  Serial.println();
}


void ele_query() {
  Serial.print("current elevation: ");
  Serial.println(elCurrent);
  Serial.println();
}


void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);
  pinMode(STEP1, OUTPUT);
  pinMode(STEP2, OUTPUT);
}


void loop() {
  
  i = 0;
  
  while (Serial.available()) {
    c = Serial.read();
    
    if (c != '\n' && c != ' ') {
      comstring[i] = c;
      i++;
    } else {
      comstring[i] = '\0';
      break;
    }
  }
  
  if(strncmp(comstring, "AZ", 2) == 0){
    // do the azimuth change
    azTarget = strtof(&comstring[2], &endPtr);
    if (endPtr != &comstring[2]) {
      Serial.print("|-- changing azimuth to ");
      Serial.print(azTarget);
      Serial.println(" ---");
      azimuth(azTarget);
    } else {
      Serial.println("current azimuth request received.");
      azi_query();
    }
    
    comstring[0] = '\0';

  } else if(strncmp(comstring, "EL", 2) == 0) {
    // do the elevation change
    elTarget = strtof(&comstring[2], &endPtr);

    if (endPtr != &comstring[2]) {
      Serial.print("*** changing elevation to ");
      Serial.print(elTarget);
      Serial.println(" ***");
      elevation(elTarget);
    } else {
      Serial.println("current elevation request received.");
      ele_query();
    }
    
    comstring[0] = '\0';
  }

}
