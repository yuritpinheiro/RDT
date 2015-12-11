#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <cstdio>
#include <queue>
#include <unistd.h>
#include "bigu_files.h"
#include <fftw3.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct DataSample {
	std::size_t sampleCount;
	const sf::Int16* samples;
};



class Recorder : public sf::SoundRecorder {
public:	
	void init(DataSample* head, int* ganho_pot_0, int* ganho_pot_1) {//std::queue<DataSample>* head){
		dataSample = head;
		setProcessingInterval(sf::milliseconds(50));
		this->ganho_pot_1 = ganho_pot_1;
		this->ganho_pot_0 = ganho_pot_0;
		in = (fftw3_complex*) fftw_malloc(sizeof() * N);
		out = (fftw3_complex*) fftw_malloc(sizeof() * N);
		p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
		p2 = fftw_plan_dft_1d(N, out, in, FFTW_BACKWARD, FFTW_ESTIMATE);
	}

private:
	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount){
		// transform
		for (int i = 0; i < N; i++) {
			in[i][0] = samples[i];
			in[i][1] = 0;
		}
		fftw_execute(p);

		// apply gains
		

		// itransform
		fftw_execute(p2);
		for (int i = 0; i < N; i++) {
			samples[i] = in[i][0];
		}

		// send to buffer
		DataSample sample;
	
		dataSample->samples = samples;
		dataSample->sampleCount = sampleCount;
		//dataSample->push(sample);
		//printf("new sample\n");
		return true;
	}

	//std::queue<DataSample>* dataSample;

	fftw3_complex *in, *out;
	fftw3_plan p, p2;
	int N = 400;
	double* ganho_pot_0;
	double* ganho_pot_1;
	DataSample* dataSample;
};

class Stream : public sf::SoundStream {
public:
	void init(unsigned int channelCount, unsigned int sampleRate, DataSample* head) {//std::queue<DataSample>* head){
		dataSample = head;
		initialize(channelCount, sampleRate);
	}

private:
	virtual bool onGetData(Chunk& data){
		//if (dataSample->size() > 0) {
			data.samples = dataSample->samples;
			data.sampleCount = dataSample->sampleCount;
			//printf("sC: %d\n", (int)dataSample->size());
			//dataSample->pop();
			return true;
		//} else {
			//return false;
		//}
	}

	virtual void onSeek(sf::Time tiemOffset){}

	//std::queue<DataSample>* dataSample;
	DataSample* dataSample;
};

int main(int argc, char const *argv[])
{
	std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();
	char* ganho0_char = new char[4];
	char* ganho1_char = new char[4];
	// system(SETUP);
	// iterate over availableDevices and print
	for (int i = 0; i < availableDevices.size(); ++i) {
		printf("%d - %s\n", i, availableDevices[i].c_str());
	}

	// Get from user the selected device
	int opt = atoi(argv[1]); // user option

	//std::queue<DataSample> dataSample;
	double ganho_pot_0 = 1;
	double ganho_pot_1 = 1;
	DataSample dataSample;
	printf("str criado\n");
	Recorder rec;
	printf("rec criado\n");
	if (!rec.setDevice(availableDevices[opt])){
		printf("Not able to use device");
	} else {
		printf("Device setted\n");
	}

	// inicializar a comunicação
	rec.init(&dataSample, &ganho_pot_0, &ganho_pot_1);
	printf("Recorder iniciado\n");
	rec.start();
	printf("Gravação iniciada\n");
	usleep(100000);

	Stream str;
	str.init(atoi(argv[2]), 44100, &dataSample);
	str.play();

	while(1){
		// read_file(ADC_POT_0_VALUE, ganho0_char);
		// read_file(ADC_POT_1_VALUE, ganho1_char);
		// ganho_pot_0 = atof(ganho0_char);
		// ganho_pot_1 = atof(ganho1_char);
		usleep(1000000);
		// printf("status %d\n", (int)str.getStatus());
		if (str.getStatus() != Stream::Playing) {
			str.play();
		}
	}

	rec.stop();

	return 0;
}
