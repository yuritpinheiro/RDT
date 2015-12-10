#include <SFML/Audio.hpp>
#include <vector>
#include <string>
#include <cstdio>
#include <queue>
#include <unistd.h>
#include <bigu_files.h>

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
	}

private:
	virtual bool onProcessSamples(const sf::Int16* samples, std::size_t sampleCount){
		// transform
		
		// apply gains
		
		// itransform
		
		// send to buffer
		DataSample sample;
	
		dataSample->samples = samples;
		dataSample->sampleCount = sampleCount;
		//dataSample->push(sample);
		//printf("new sample\n");
		return true;
	}

	//std::queue<DataSample>* dataSample;
	int* ganho_pot_0;
	int* ganho_pot_1;
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
	char* ganho0_char, ganho1_char;
	system(SETUP);
	// iterate over availableDevices and print
	for (int i = 0; i < availableDevices.size(); ++i) {
		printf("%d - %s\n", i, availableDevices[i].c_str());
	}

	// Get from user the selected device
	int opt = 0; // user option

	//std::queue<DataSample> dataSample;
	int ganho_pot_0 = 1;
	int ganho_pot_1 = 1;
	DataSample dataSample;
	Stream str;
	Recorder rec;
	if (!rec.setDevice(availableDevices[opt])){
		printf("Not able to use device");
	}

	// inicializar a comunicação
	rec.init(&dataSample, &ganho_pot_0, &ganho_pot_0);
	printf("Recorder iniciado\n");
	rec.start();
	printf("Gravação iniciada\n");
	usleep(10000);

	str.init(atoi(argv[1]), 44100, &dataSample);
	str.play();

	while(1){
		read_file(ADC_POT_0_VALUE, ganho0_char);
		read_file(ADC_POT_1_VALUE, ganho1_char);
		ganho_pot_0 = atoi(ganho0_char);
		ganho_pot_1 = atoi(ganho1_char);
		usleep(100000000);
		// printf("status %d\n", (int)str.getStatus());
		if (str.getStatus() != Stream::Playing) {
			str.play();
		}
	}

	rec.stop();

	return 0;
}