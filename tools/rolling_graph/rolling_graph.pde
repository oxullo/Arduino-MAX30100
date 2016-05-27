import processing.serial.*;

final int WIDTH = 1200;
final int HEIGHT = 600;
final int CHANNELS = 2;
final String serialPort = "/dev/tty.usbmodemFD131";
final color[] colors = {color(0, 0, 0), color(255, 0, 0), color(0, 255, 0), color(0, 0, 255)};
final int VALUE_MAX = 500;
final int VALUE_MIN = -500;

float[][] series = new float[CHANNELS][WIDTH];
float heartRate = 0;
int ptr = 0;

Serial myPort;

void settings()
{
  size(WIDTH, HEIGHT);
}

void setup ()
{
  myPort = new Serial(this, serialPort, 115200);
  
  stroke(0);
  fill(0);
  textSize(8);
}

void draw ()
{
  background(255);
  
  for (int s=0 ; s < CHANNELS ; ++s) {
    stroke(colors[s]);
    
    float[] samples = series[s];
    float maxv, minv;
    
    if (VALUE_MAX == -1) {
      maxv = max(samples);
    } else {
      maxv = VALUE_MAX;
      float samplesMax = max(samples);
      line(0, samplesMax, WIDTH, samplesMax);
    }
    
    if (VALUE_MIN == -1) {
      minv = min(samples);
    } else {
      minv = VALUE_MIN;
      float samplesMin = min(samples);
      line(0, samplesMin, WIDTH, samplesMin);
    }
    
    text("ch " + s + " max:" + maxv, 0, 8 + 10 * s);
    if (minv == maxv) {
      maxv = minv + 1;
    }
    
    for (int i = 0 ; i < WIDTH ; ++i) {
      if (i > 0) {
        line(i - 1, HEIGHT - map(samples[i-1], minv, maxv, 0, HEIGHT),
             i, HEIGHT - map(samples[i], minv, maxv, 0, HEIGHT));
      }
    }
  }
  
  text("Rate: " + heartRate, 100, 8);
}
  
void serialEvent (Serial myPort)
{
  String sLine = myPort.readStringUntil('\n');
  
  if (sLine == null) {
    return;
  }

  if (sLine.substring(0, 2).equals("R:")) {
    String[] sValues = split(sLine.substring(2), ' ');
  
    for (int i=0 ; i < sValues.length ; ++i) {
      float sample = float(sValues[i]);
    
      if (Float.isNaN(sample)) {
        continue;
      }
    
      series[i][ptr] = sample;
    }
    
    ptr = (ptr + 1) % WIDTH;
  } else if (sLine.substring(0, 2).equals("H:")) {
    heartRate = float(sLine.substring(2));
  }
}