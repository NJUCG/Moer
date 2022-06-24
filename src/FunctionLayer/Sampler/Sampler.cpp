#include "Sampler.h"
#include "FunctionLayer/Camera/Camera.h"

CameraSample Sampler::getCameraSample() {
	CameraSample sample;
	sample.xy = sample2D();
	sample.time = sample1D();
	sample.lens = sample2D();
	return sample;
}