#ifndef SOUNDPROJECT_H
#define SOUNDPROJECT_H

class SoundProject {
private:
	char TrackBase[50];
   int  NumTracks;
   int  Volume[8];			// Set to 8 because only supports 8 tracks right now
   int  Pan[8];   			// Set to 8 because only supports 8 tracks right now
   char Name[8][30];
   int  MainVolume;
   int  SampleRate;
   int  BitRate;

public:
	SoundProject();
   ~SoundProject();

   void SetBase(const char *BaseFileName);
   void SetTrackVolume(int Track, int Vol);
   void SetTrackPan(int Track, int iPan);
   void SetNumberTracks(int NumTrack);
   void SetMainVolume(int Vol);
   void SetTrackName(int Track, const char *tName);
   void SetSampleRate(int SRate);
   void SetBitRate(int BRate);

   char *GetBase();
   int  GetTrackVolume(int Track);
   int  GetTrackPan(int Track);
   int  GetNumberTracks();
   int  GetMainVolume();
   int  GetSampleRate();
   int  GetBitRate();
   char *GetTrackName(int Track);

   void Reset();

   void Save(const char *FilePath);
   void Load(const char *FilePath);
};

#endif
