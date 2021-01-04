
#include <Servo.h>//needed for servo

const int sensorPin = A0;//pin to listen to audio

String trend = "neutral"; //will indicate what the trend of the sound is to control mouth
double baseline;//finds baseline value to compare to

const int sampleNum = 5;//is length of array that are averaged out
const int averageNum = 100;//needs to take the average of the last 100 and the averge of the last 3
double runningTotal[sampleNum];//array of past readings
double runningAverage[15];//array of past averages

Servo myServo;//declares servo

void setup() {
  // put your setup code here, to run once:
  myServo.attach(11);//initalizes the servo

  Serial.begin(9600);//opens serial channel
}

void loop() {
  // put your main code here, to run repeatedly:
  int curAvg;//will be the current average
  
  curAvg = getAverageMicReading();//returns average and cycles arrays
  Serial.println(curAvg);//prints out values to serial plotter

  trend = findTrend();//finds trend in the past values

  moveMouth(trend);//moves the mouth, sperate so that it can be tweeked independently

  //Serial.print("\n");

}

double getAverageMicReading(){//gets new mic reading and updates arrays
  double micVal;//current value of the mic
  double avg;//average of all values in the array
  double total = 0;//stores total of all array values
  
  micVal = analogRead(sensorPin);

  for(int i = 1 ; i < sampleNum ; i++){ //loop shifts array back overwriteing futhest value
    runningTotal[sampleNum - i] = runningTotal[sampleNum - (i + 1)];
  }

  runningTotal[0] = micVal;//adds new value to the start of the array

  for(int j = 0 ; j < sampleNum ; j++){//adds up new array
    total += runningTotal[j];
  }

  avg = total / sampleNum;//divdes total by the lenth of array to get average

  //an array must be made of the average values to identify trends

  for(int t = 1 ; t < 15 ; t++){ //loop shifts array back overwriteing futhest value
    runningAverage[15 - t] = runningAverage[15 - (t + 1)];
  }

  runningAverage[0] = avg;//adds new average the the array 

  return avg;// returns single current average
}

void moveMouth(){//independent function to move the mouth

  //Serial.print(trend);

  if((trend == "neutral") || (trend == "down")){//if the trends is neutral or down close the mouth
    myServo.write(0);//moves servo
    //Serial.print("    Servo to 0");
  }

  if(trend == "up"){//if trend is up open mouth
    myServo.write(70);//moves servo
    //Serial.print("    Servo to 90");
  }
}

String findTrend(){
  String trend = "neutral";//trend is what way the array is heading, as vol inc the mouth opens and vol dec mouth closes
  int trendScore = 0;//trend score starts at neutral, goes negative for a downward trend, positive with upward trend
  int sense = 3;

  for(int i = 0 ; i < 5 ; i++){//takes last five array values and asks thier trend in comparson to previous value
    if(runningAverage[i] > runningAverage[i + 1]){//if last value is greater than the value beofore it
      trendScore++;//the trend score goes up one
    }
    else if(runningAverage[i] < runningAverage[i + 1]){//if last value is less than the value beofore it
      trendScore--;//the trend score goes down one
    }
  }

  //Serial.print(trendScore);


  if(trendScore > 3){//if most points trend up, return up
    trend = "up";//changes trend
    //Serial.print("   Trend up");//prints trend
    myServo.write(70);//moves servo
  }
  else if(trendScore < -3){//if most points trend down, return down
    trend = "down";//changes trend
    //Serial.print("   Trend down");//prints trend
    myServo.write(0);//moves servo
  }
  else{//if there is no clear trend it is neutral
    trend = "neutral";//changes trend
    //Serial.print("   Trend neutral");//prints trend
    myServo.write(0);//moves servo
  }

}
