// Based on https://www.researchgate.net/publication/3728672_A_simple_real-time_QRS_detection_algorithm
// http://www.atmel.com/images/doc2527.pdf
// http://de.mathworks.com/matlabcentral/fileexchange/45840-complete-pan-tompkins-implementation-ecg-qrs-detector/content/pan_tompkin.m
// http://cnx.org/contents/YR1BUs9_@1/QRS-Detection-Using-Pan-Tompki

#ifndef QRSDETECTOR_H
#define QRSDETECTOR_H

#include <stdint.h>

#define QRSDETECTOR_INIT_HOLDOFF                2000    // in ms, how long to wait before counting
#define QRSDETECTOR_MASKING_HOLDOFF             200     // in ms, non-retriggerable window after beat detection
#define QRSDETECTOR_BPFILTER_ALPHA              0.5     // EMA factor for the beat period value
#define QRSDETECTOR_MIN_THRESHOLD               30      // minimum threshold (filtered) value
#define QRSDETECTOR_MAX_THRESHOLD               500     // maximumg threshold (filtered) value
#define QRSDETECTOR_THRESHOLD_FALLOFF_TARGET    0.3     // thr chasing factor of the max value when beat
#define QRSDETECTOR_THRESHOLD_DECAY_FACTOR      0.99    // thr chasing factor when no beat
#define QRSDETECTOR_INVALID_READOUT_DELAY       2000    // in ms, no-beat time to cause a reset
#define QRSDETECTOR_SAMPLES_PERIOD              10      // in ms, 1/Fs


typedef enum QRSDetectorState {
    QRSDETECTOR_STATE_INIT,
    QRSDETECTOR_STATE_WAITING,
    QRSDETECTOR_STATE_FOLLOWING_SLOPE,
    QRSDETECTOR_STATE_MASKING
} QRSDetectorState;

// http://www.schwietering.com/jayduino/filtuino/
// Band pass bessel filter order=2 alpha1=0.05 alpha2=0.15
// Fc=100Hz -> Fl=5Hz, Fh=15Hz
class  FilterBeBp2
{
	public:
		FilterBeBp2()
		{
			for(int i=0; i <= 4; i++)
				v[i]=0.0;
		}
	private:
		float v[5];
	public:
		float step(float x) //class II
		{
			v[0] = v[1];
			v[1] = v[2];
			v[2] = v[3];
			v[3] = v[4];
			v[4] = (9.189847175584472949e-2 * x)
				 + (-0.24114073878907088533 * v[0])
				 + (1.15795881596677863179 * v[1])
				 + (-2.41311507478477782485 * v[2])
				 + (2.44900730391579601886 * v[3]);
			return
				 (v[0] + v[4])
				- 2 * v[2];
		}
};


class QRSDetector
{
public:
    QRSDetector();
    float addSample(float sample);
    float getHeartRate();
    float getCurrentThreshold();

private:
    void checkForBeat(float value);
    void decreaseThreshold();

    FilterBeBp2 filter;
    QRSDetectorState state;
    float threshold;
    float beatPeriod;
    float lastMaxValue;
    uint32_t tsLastPulse;
};

#endif
