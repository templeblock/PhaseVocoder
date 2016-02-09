#ifndef __TIMESTRETCHER_H__
#define __TIMESTRETCHER_H__

#include "vocoder_functions.h"
#include "frame.h"

using namespace std;

class TimeStretcher
{
    public:
        TimeStretcher(int n) : FFT_SIZE(n) {
            cached_phase = vector<float>(n/2+1,0);
            phasor = vector<float>(n/2+1,0);
            vocoder_func = new VocoderFunctions();
        }
        virtual ~TimeStretcher() { delete vocoder_func; }

        void UpdatePhase(vector<float> mag, vector<float> prev_phase, vector<float> phase, vector<float>& synth_ph);
        void SynthesizeFrame(vector<float>&, vector<float>&, Frame*);
        void Stretch(float rate, vector<Frame*>& input_spec, vector<Frame*>& output_spec, bool reset_phase);

    protected:
        int FFT_SIZE;
        vector<float> cached_phase; // the last phase spectrum from previous Stretch execution
        vector<float> phasor; // the delta term for the phase of every frequency bin
        VocoderFunctions* vocoder_func;
};

#endif
