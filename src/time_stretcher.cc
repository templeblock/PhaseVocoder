#include "time_stretcher.h"

/**
* Phase-locked phase vocoder:
* 1. Gather channels around each peak value into 1 group
* 2. calculate the phase of each peak
* 3. calculate the phase of the other channels with the peak's phasor
*/
void TimeStretcher::SynthesizePhase(vector<float> mag, vector<float> prev_phase, vector<float> phase, vector<float>& synth_ph) {
    vector<int> subband = groupChannel(mag);

    for(int freq=0; freq<FFT_SIZE/2+1; ++freq)
        if (freq==subband[freq])
            phasor[freq] = phase[freq] - prev_phase[prev_subband[freq]];

    for(int freq=0; freq<FFT_SIZE/2+1; ++freq)
    {
        synth_ph[freq] = prev_phase[freq] + phasor[subband[freq]];
        while(synth_ph[freq] >= PI) synth_ph[freq] -= 2.0 * PI;
        while(synth_ph[freq] < -1.0*PI) synth_ph[freq] += 2.0 * PI;
        prev_subband[freq] = subband[freq]; // record previous sub-bands for scaled PL
    }
}

Frame TimeStretcher::SynthesizeFrame(vector<float>& mag, vector<float>& ph){
    Frame f(FFT_SIZE);
    return f;
}

void TimeStretcher::Stretch(float rate, vector<Frame>& input_spec, vector<Frame>& output_spec, bool reset_phase) {
    vector<float> mag, ph;
    float sample_ptr = 0.0; // the pointer to the old spectrum, where the new
                            // magnitude/phase should be synthesized from.

	// initialize the first frame
    mag = input_spec[0].getMagnitude();

	if (reset_phase) {
        ph = input_spec[0].getPhase();
        output_spec.push_back(input_spec[0]);
        prev_subband = groupChannel(mag);
    }
    else {
        SynthesizePhase(mag, input_spec[0].getPhase(), cached_phase, ph);
        output_spec.push_back(SynthesizeFrame(mag, ph));
    }

    for (int i=1; i<rate*input_spec.size(); ++i) {
        sample_ptr += 1.0/rate;
        mag = vectorWeightedSum(input_spec[(int)floor(sample_ptr)].getMagnitude(), input_spec[(int)ceil(sample_ptr)].getMagnitude(), ceil(sample_ptr)-sample_ptr, sample_ptr-floor(sample_ptr));

        SynthesizePhase(mag, input_spec[(int)ceil(sample_ptr)].getPhase(), input_spec[(int)floor(sample_ptr)].getPhase(), ph);
        output_spec.push_back(SynthesizeFrame(mag, ph));
    }

    for(int freq=0;freq<FFT_SIZE/2+1;freq++)
        cached_phase[freq] = ph[freq];
}