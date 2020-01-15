int ms_v = 340;
float radar_X = 0;
float radar_Y = 0;
float radar_Z = 0;
float pi = 3.1415926535897932384626433832795;

float g1_x = -3000;
float g1_y = 0;
float g1_z = 1;

float g2_x = 2000;
float g2_y = 5000;
float g2_z = 1000;

float g3_x = 2000;
float g3_y = -5000;
float g3_z = 1;

float d2T = 10000;
int Theta_RTE = 20;
float dms[] = {10 , 30 , 30};
float target_V  = 345;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {

  float *getData;
  //radar Data Calculation.....

  float angleDeg = dms2degree(dms);
  float z_rt = sin((Theta_RTE * (pi / 180)) ) * d2T;
  float RT_xy = cos((Theta_RTE * (pi / 180))) * d2T;
  float x_rt = cos((angleDeg * (pi / 180))) * RT_xy;
  float y_rt = sin((angleDeg * (pi / 180))) * RT_xy;

  //   velocity calculation
  float gunRange = 5200;
  float tar_disp = d2T - gunRange;
  float t = tar_disp / target_V;
  Serial.println(z_rt);

  Serial.println();
  /// Radar Data Calculation

  getData = GunsDataCalculation(g1_x, g1_y, g1_z, x_rt, y_rt, z_rt);
  //  Serial.println("Gun1");
  //  Serial.println(getData[0]);
  //  Serial.println(getData[1]);
  //  Serial.println(getData[2]);
  //  Serial.println(getData[3]);
  getData = GunsDataCalculation(g2_x, g2_y, g2_z, x_rt, y_rt, z_rt);
  //  Serial.println("Gun2");
  //  Serial.println(getData[0]);
  //  Serial.println(getData[1]);
  //  Serial.println(getData[2]);
  //  Serial.println(getData[3]);
  getData = GunsDataCalculation(g3_x, g3_y, g3_z, x_rt, y_rt, z_rt);
  //  Serial.println("Gun3");
  //  Serial.println(getData[0]);
  //  Serial.println(getData[1]);
  //  Serial.println(getData[2]);
  //  Serial.println(getData[3]);

  delay(10000);

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
  float thitaAzimuth = acos(target2gunX * (pi / 180) / gun2targetXY * (pi / 180)) * (180 / pi);
  float thitaElevatoin = atan(target2gunZ * (pi / 180) / gun2targetXY * (pi / 180)) * (180 / pi);
  float dist_gun2target = gun2targetXY / cos(thitaElevatoin * (pi / 180));

  float requireTime = (dist_gun2target / ms_v) + 5;

  calculateData[0] = dist_gun2target;
  calculateData[1] = thitaAzimuth;
  calculateData[2] = thitaElevatoin;
  calculateData[3] = requireTime;

  //    Serial.println(dist_gun2target);
  //    Serial.println(thitaAzimuth);
  //    Serial.println(thitaElevatoin);


  return calculateData;

}
