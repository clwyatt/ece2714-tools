#include "wav.hpp"

#include <fstream>
#include <string>
#include <cmath>

#include "signal.hpp"

// debug
#include <iostream>

// see https://ccrma.stanford.edu/courses/422/projects/WaveFormat/
struct WAV
{
  int8_t   ChunkID[4];     //"RIFF"
  int32_t  ChunkSize;      // 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
  int8_t   Format[4];      // "WAVE"

  int8_t   Subchunk1ID[4]; // "fmt "
  int32_t  Subchunk1Size;  // 16
  int16_t  AudioFormat;    // 1
  int16_t  NumChannels;    // Mono = 1
  int32_t  SampleRate;     // samples per second, 44100
  int32_t  ByteRate;       // SampleRate * NumChannels * BitsPerSample/8
  int16_t  BlockAlign;     // NumChannels * BitsPerSample/8
  int16_t  BitsPerSample;  // 8 bits = 8, 16 bits = 16

  int8_t   Subchunk2ID[4]; // "data"
  int32_t  Subchunk2Size;  // NumSamples * NumChannels * BitsPerSample/8
  int16_t* Data;           // the Pulse Code Modulated data
};

void fill_header(WAV & sound, unsigned int rate)
{
  sound.ChunkID[0] = 'R';
  sound.ChunkID[1] = 'I';
  sound.ChunkID[2] = 'F';
  sound.ChunkID[3] = 'F';

  sound.Format[0] = 'W';
  sound.Format[1] = 'A';
  sound.Format[2] = 'V';
  sound.Format[3] = 'E';

  sound.Subchunk1ID[0] = 'f';
  sound.Subchunk1ID[1] = 'm';
  sound.Subchunk1ID[2] = 't';
  sound.Subchunk1ID[3] = ' ';

  sound.Subchunk2ID[0] = 'd';
  sound.Subchunk2ID[1] = 'a';
  sound.Subchunk2ID[2] = 't';
  sound.Subchunk2ID[3] = 'a';

  sound.Subchunk1Size = 16;
  sound.AudioFormat = 1;
  sound.NumChannels = 1;
  sound.SampleRate = rate;
  sound.BitsPerSample = 16;
  sound.ByteRate = sound.SampleRate*sound.NumChannels*sound.BitsPerSample/8;
  sound.BlockAlign = sound.NumChannels*sound.BitsPerSample/8;
}

bool write_wav(RegularSignal & sound, const std::string &filename)
{ 
  std::ofstream outfile(filename.c_str(), std::ios::binary);

  WAV header;
  fill_header(header, sound.getSampleRate());

  header.Subchunk2Size = sound.size()*header.NumChannels*header.BitsPerSample/8;

  header.ChunkSize = 4 + (8 + header.Subchunk1Size) + (8 + header.Subchunk2Size);

  // write header
  outfile.write((char *)&header.ChunkID, sizeof(header.ChunkID));
  outfile.write((char *)&header.ChunkSize, sizeof(header.ChunkSize));
  outfile.write((char *)&header.Format, sizeof(header.Format));
  outfile.write((char *)&header.Subchunk1ID, sizeof(header.Subchunk1ID));
  outfile.write((char *)&header.Subchunk1Size, sizeof(header.Subchunk1Size));
  outfile.write((char *)&header.AudioFormat, sizeof(header.AudioFormat));
  outfile.write((char *)&header.NumChannels, sizeof(header.NumChannels));
  outfile.write((char *)&header.SampleRate, sizeof(header.SampleRate));
  outfile.write((char *)&header.ByteRate, sizeof(header.ByteRate));
  outfile.write((char *)&header.BlockAlign, sizeof(header.BlockAlign));
  outfile.write((char *)&header.BitsPerSample, sizeof(header.BitsPerSample));
  outfile.write((char *)&header.Subchunk2ID, sizeof(header.Subchunk2ID));
  outfile.write((char *)&header.Subchunk2Size, sizeof(header.Subchunk2Size));

  // write the data
  std::cout << "Writing " << header.Subchunk2Size << " sound bytes." << std::endl;

  for(std::size_t i = 0; i < sound.size(); ++i){
    double sample = sound[i];
    int16_t temp = static_cast<int16_t>(lround(sample));
    outfile.write((char *)&temp, 2);
  }
  
  return true;
}

bool read_wav(RegularSignal & sound, const std::string &filename)
{
  std::ifstream infile(filename.c_str(), std::ios::binary);
  if(!infile){
    std::cerr << "Error: could not open file for reading\n";
    return false;
  }

  // read header
  WAV header;
  infile.read((char *)&header.ChunkID, sizeof(header.ChunkID));
  if((header.ChunkID[0] != 'R') || (header.ChunkID[1] != 'I') ||
     (header.ChunkID[2] != 'F') || (header.ChunkID[3] != 'F')){
    std::cerr << "Error: wrong ChunkID\n";
    return false;
  }
    
  infile.read((char *)&header.ChunkSize, sizeof(header.ChunkSize));
    
  infile.read((char *)&header.Format, sizeof(header.Format));
  if((header.Format[0] != 'W') || (header.Format[1] != 'A') ||
     (header.Format[2] != 'V') || (header.Format[3] != 'E')){
    std::cerr << "Error: not WAVE format\n";
    return false;
  }
  
  infile.read((char *)&header.Subchunk1ID, sizeof(header.Subchunk1ID));
  if((header.Subchunk1ID[0] != 'f') || (header.Subchunk1ID[1] != 'm') ||
     (header.Subchunk1ID[2] != 't') || (header.Subchunk1ID[3] != ' ')){
    std::cerr << "Error: invalid Subchunk1ID\n";
    return false;
  }
  
  infile.read((char *)&header.Subchunk1Size, sizeof(header.Subchunk1Size));
  if(header.Subchunk1Size != 16){
        std::cerr << "Error: invalid Subchunk1Size\n";
	return false;
  }
  
  infile.read((char *)&header.AudioFormat, sizeof(header.AudioFormat));
  if(header.AudioFormat != 1){
        std::cerr << "Error: invalid AudioFormat\n";
	return false;
  }
  infile.read((char *)&header.NumChannels, sizeof(header.NumChannels));
  if(header.NumChannels != 1){
    std::cerr << "Error:number of channels not 1\n";
    return false;
  }
    
  infile.read((char *)&header.SampleRate, sizeof(header.SampleRate));
  sound.setSampleRate(header.SampleRate);
    
  infile.read((char *)&header.ByteRate, sizeof(header.ByteRate));
  infile.read((char *)&header.BlockAlign, sizeof(header.BlockAlign));
  infile.read((char *)&header.BitsPerSample, sizeof(header.BitsPerSample));
  if(header.BitsPerSample != 16) return false;
  
  infile.read((char *)&header.Subchunk2ID, sizeof(header.Subchunk2ID));
  if((header.Subchunk2ID[0] != 'd') || (header.Subchunk2ID[1] != 'a') ||
     (header.Subchunk2ID[2] != 't') || (header.Subchunk2ID[3] != 'a')){
    std::cerr << "Error: invalid SubChunk2ID\n";
    return false;
  }
  infile.read((char *)&header.Subchunk2Size, sizeof(header.Subchunk2Size));

  // read the data
  std::cout << "Reading " << header.Subchunk2Size << " sound bytes." << std::endl;

  for(intmax_t i = 0; i < header.Subchunk2Size; i+= 2){
    int16_t temp;
    infile.read((char *)&temp, 2);
    double sample = temp;
    sound.push_back(sample);
  }
  
  return true;
}

