#ifndef TIMESTRETCHER_FD_H
#define TIMESTRETCHER_FD_H

#include "time_stretcher.h"

using namespace std;

/**
 * Modified version of time stretching
 * - interpolate/extrapolate on spectrogram, thus the number of frames changed
 * - the output synthesis-rate/frame-shift/hop-size remain the same as analysis
*/
class TimeStretcherFD : public TimeStretcher
{
    public:
        TimeStretcherFD(double rate, int n, int s) : TimeStretcher(rate, n, s) {}
        virtual ~TimeStretcherFD(){}
        virtual void UpdatePhase(vector<double> mag, vector<double> prev_phase, vector<double> next_phase, vector<double>& synth_ph);
        virtual void Stretch(vector<Frame*>& input_spec, vector<Frame*>& output_spec, int &synthesis_frame_shift, bool reset_phase);
};

#endif