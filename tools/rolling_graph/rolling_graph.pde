import processing.serial.*;

final int WIDTH = 1200;
final int HEIGHT = 600;
final String serialPort = "/dev/tty.usbmodemFA141";

float[] samples = new float[WIDTH];
int ptr = 0;

Serial myPort;

void settings()
{
  size(WIDTH, HEIGHT);
}

void setup () {
  myPort = new Serial(this, serialPort, 115200);
  
  stroke(0);
}
  
void draw () {
  background(255);
  
  float maxv = max(samples);
  float minv = min(samples);
  
  for (int i = 0 ; i < WIDTH ; ++i) {
    if (i > 0) {
      line(i - 1, map(samples[i-1], minv, maxv, 0, HEIGHT), i, map(samples[i], minv, maxv, 0, HEIGHT));
    }
  }
}
  
void serialEvent (Serial myPort) {
  // get the byte:
  String inLine = myPort.readStringUntil('\n');
  
  if (inLine == null) {
    return;
  }
  
  float sample = float(inLine);
  
  if (Float.isNaN(sample)) {
    return;
  }

  samples[ptr++] = sample;
  
  if (ptr == WIDTH) {
    ptr = 0;
  }
}