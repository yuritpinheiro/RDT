#include <SFML/Audio.hpp>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <vector>

// custom audio stream that plays a loaded buffer
class MyStream : public sf::SoundStream
{
public:

    void load(std::string device)//const sf::SoundBuffer& buffer)
    {
		if (!recorder.setDevice(device))
		{
    		printf("error: device selection failed\n");
		}
		printf("loaded\n");
		
		recorder.start();
        
        /*// extract the audio samples from the sound buffer to our own container
        m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

        // reset the current playing position 
        m_currentSample = 0;
		*/
        // initialize the base class
        initialize(1, 44100);
		printf("initialized\n");   
    }

private:

    virtual bool onGetData(Chunk& data)
    {
        // number of samples to stream every time the function is called;
        // in a more robust implementation, it should be a fixed
        // amount of time rather than an arbitrary number of samples
        //const int samplesToStream = 1024;

        const sf::SoundBuffer& buffer  = recorder.getBuffer();
		//m_samples.assign(buffer.getSamples(), buffer.getSamples() + buffer.getSampleCount());

        // set the pointer to the next audio samples to be played
        data.samples = buffer.getSamples();// &m_samples[m_currentSample];

        // have we reached the end of the sound?
        //if (m_currentSample + samplesToStream <= m_samples.size())
        //{
            // end not reached: stream the samples and continue
            data.sampleCount = buffer.getSampleCount();//samplesToStream;
            //m_currentSample += buffer.getSampleCount();//samplesToStream;
            printf("get data\n");
            return true;
        //}
        //else
        //{
            // end of stream reached: stream the remaining samples and stop playback
            //data.sampleCount = m_samples.size() - m_currentSample;
            //m_currentSample = m_samples.size();
            //return false;
        //}

    }

    virtual void onSeek(sf::Time timeOffset)
    {
        // compute the corresponding sample index according to the sample rate and channel count
        m_currentSample = static_cast<std::size_t>(timeOffset.asSeconds() * getSampleRate() * getChannelCount());
    }

	sf::SoundBufferRecorder recorder;
    std::vector<sf::Int16> m_samples;
    std::size_t m_currentSample;
};


int main()
{

	if (!sf::SoundBufferRecorder::isAvailable())
	{
    	printf("error: audio capture is not available on this system	\n");
    	return 0;
    
	}

	std::vector<std::string> availableDevices = sf::SoundRecorder::getAvailableDevices();

	// choose a device
	for(int i = 0; i < availableDevices.size(); i++)
		printf("%s\n",availableDevices[i].c_str());
	std::string inputDevice = availableDevices[0];



	//sleep(1);
	//recorder.stop();
	MyStream stream;
	stream.load(availableDevices[0]);

	stream.play();
	printf("played\n");
	while(true){	
		/*
	   	const sf::SoundBuffer& buffer  = recorder.getBuffer();
    	stream.load(buffer);   
    	recorder.start();
    	stream.play();
	    
    	while (stream.getStatus() == MyStream::Playing)
			sf::sleep(sf::seconds(0.1f));
	    
	    recorder.stop();
	    */
	}
    return 0;
}