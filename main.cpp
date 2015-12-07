#include <SFML/Audio.hpp>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

class MyStream : public sf::SoundStream {
public:

    void load(const sf::SoundBuffer& buffer)
    {
        // extract the audio samples from the sound buffer to our own container
        m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

        // reset the current playing position
        m_currentSample = 0;

        // initialize the base class
        initialize(buffer.getChannelCount(), buffer.getSampleRate());
    }

private:

    virtual bool onGetData(Chunk& data)
    {
        // number of samples to stream every time the function is called;
        // in a more robust implementation, it should be a fixed
        // amount of time rather than an arbitrary number of samples
        const int samplesToStream = 50000;

        // set the pointer to the next audio samples to be played
        data.samples = &m_samples[m_currentSample];

        // have we reached the end of the sound?
        if (m_currentSample + samplesToStream <= m_samples.size())
        {
            // end not reached: stream the samples and continue
            data.sampleCount = samplesToStream;
            m_currentSample += samplesToStream;
            return true;
        }
        else
        {
            // end of stream reached: stream the remaining samples and stop playback
            data.sampleCount = m_samples.size() - m_currentSample;
            m_currentSample = m_samples.size();
            return false;
        }
    }

    virtual void onSeek(sf::Time timeOffset)
    {

    }

    std::vector<sf::Int16> m_samples;
    std::size_t m_currentSample;
};

int main(int argc, char const *argv[]) {

	// std::vector<std::string> availableDevices = sf::SoundBufferRecorder::getAvailableDevices();
	//
	// for (int i = 0; i < availableDevices.size(); i++) {
	// 	printf("%s\n", availableDevices[i].c_str());
	// }

	// Testar interfaces de entradas de audio
	if (!sf::SoundBufferRecorder::isAvailable()) {
		printf("Audio interface not available.\n");
	}

	// Criar o buffer de entrada
	sf::SoundBufferRecorder recorder;
	printf("Começo gravação\n");
	recorder.start();

	// delay
	usleep(10000000);

	recorder.stop();
	printf("Fim gravação\n");

	const sf::SoundBuffer& buffer = recorder.getBuffer();

	sf::Sound sound(buffer);
	printf("Começo play\n");
	sound.play();

	usleep(10000000);

	printf("Fim play\n");
	return 0;
}
