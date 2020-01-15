// Radar Position //

int ms_v = 340;
float radar_X = 0;
float radar_Y = 0;
float radar_Z = 0;
float pi = 3.142;

//Gun 1 //

float g1_x = -3000;
float g1_y = 0;
float g1_z = 1;

//Gun 2 //

float g2_x = 2000;
float g2_y = 5000;
float g2_z = 1000;

// Gun 3 //

float g3_x = 2000;
float g3_y = -5000;
float g3_z = 1;

// Required Data //

float dgt1 = 0;//
float d2T = 5200;
int Theta_RTE = 0;//
float dms[] = {0 , 0 , 0}; //
float target_V  = 0; //
String speeds, distance, elevation, deg, minute, seconds;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {

  String recieveData;

  if (Serial.available()) {

    recieveData = Serial.readString();

    distance = getValue(recieveData, '*', 0);
    speeds = getValue(recieveData, '*', 1);
    elevation = getValue(recieveData, '*', 2);
    deg = getValue(recieveData, '*', 3);
    minute = getValue(recieveData, '*', 4);
    seconds = getValue(recieveData, '*', 5);

    dgt1 = distance.toFloat();
    target_V = speeds.toFloat();
    Theta_RTE = elevation.toFloat();
    dms[0] = deg.toFloat();
    dms[1] = minute.toFloat();
    dms[2] = seconds.toFloat();

    float *getData;
    //radar Data Calculation.....

    float angleDeg = dms2degree(dms);
    float z_rt = sin((Theta_RTE * (pi / 180))) * d2T;
    float RT_xy = cos((Theta_RTE * (pi / 180))) * d2T;
    float x_rt = cos((angleDeg * (pi / 180))) * RT_xy;
    float y_rt = sin((angleDeg * (pi / 180))) * RT_xy;

    //  Serial.println(RT_xy);
    //  Serial.println(x_rt);
    //  Serial.println(y_rt);
    //  Serial.println();

    //   velocity calculation
    float gunRange = 5200;
    float tar_disp = dgt1 - gunRange;
    float t = tar_disp / target_V;

    /// Radar Data Calculation

    getData = GunsDataCalculation(g1_x, g1_y, g1_z, x_rt, y_rt, z_rt);

    getData = GunsDataCalculation(g2_x, g2_y, g2_z, x_rt, y_rt, z_rt);

    getData = GunsDataCalculation(g3_x, g3_y, g3_z, x_rt, y_rt, z_rt);

  }

}

float dms2degree(float *ddms)
{
  float dec_degree = 0;
  float mins = (ddms[1] + (ddms[2] / 60)) / 60;
  dec_degree = ddms[0] + mins;
  return dec_degree;
}


float * GunsDataCalculation(float base_x, float base_y, float base_z, float radar2targetX, float radar2targetY, float radar2targetZ)
{
  //retrun data from the calculation
  // format {distance, azimuth ,elevation, time}

  float calculateData[4];
  //Radar to Gun
  float radar2gunX = base_x - radar_X;
  float radar2gunY = base_y - radar_Y;
  float radar2gunZ = base_z - radar_Z;

  // Gun to target
  float target2gunX = radar2targetX - radar2gunX;
  float target2gunY = radar2targetY - radar2gunY;
  float target2gunZ = radar2targetZ - radar2gunZ;

  float gun2targetXY = sqrt((target2gunX * target2gunX) + (target2gunY * target2gunY));
  float thitaAzimuth = acos((target2gunX / gun2targetXY)) * (180 / pi);
  float thitaElevatoin = atan((target2gunZ / gun2targetXY)) * (180 / pi);
  float dist_gun2target = gun2targetXY / cos((thitaElevatoin * (pi / 180)));

  float requireTime = (dist_gun2target / ms_v) + 5;

  calculateData[0] = dist_gun2target;
  calculateData[1] = thitaAzimuth;
  calculateData[2] = thitaElevatoin;
  calculateData[3] = requireTime;

    Serial.println(dist_gun2target);
    Serial.println(thitaAzimuth);
    Serial.println(thitaElevatoin);
    Serial.println(requireTime);
    Serial.println();

  return calculateData;

}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
