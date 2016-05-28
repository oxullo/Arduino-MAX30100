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
    float seriesMax = max(samples);
    
    if (VALUE_MAX == -1) {
      maxv = max(samples);
    } else {
      maxv = VALUE_MAX;
    }
    
    if (VALUE_MIN == -1) {
      minv = min(samples);
    } else {
      minv = VALUE_MIN;
    }
    
    text("ch " + s + " max:" + maxv, 0, 8 + 10 * s);
    if (minv == maxv) {
      maxv = minv + 1;
    }
    
    boolean maxDisplayed = false;
    for (int i = 0 ; i < WIDTH ; ++i) {
      if (i > 0) {
        float ipy = HEIGHT - map(samples[i-1], minv, maxv, 0, HEIGHT);
        float iy = HEIGHT - map(samples[i], minv, maxv, 0, HEIGHT);
        
        if (abs(samples[i] - seriesMax) < 0.001 && !maxDisplayed) {
          text("v=" + samples[i], i, iy);
          maxDisplayed = true;
        }
          
        line(i - 1, ipy, i, iy);
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