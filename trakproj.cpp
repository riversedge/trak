#include "TrakProj.h"
#include <windows.h>
#include <fstream>

using namespace std;

SoundProject::SoundProject() {
 	strcpy(TrackBase, "");
   NumTracks = 0;
   for (int i = 0; i < 8; i++) { Volume[i] = 100; Pan[i] = 0; strcpy(Name[i], "");}
   MainVolume = 100;
   SampleRate = 44;
   BitRate = 16;
}

SoundProject::~SoundProject() {
}

void SoundProject::Reset() {
 	strcpy(TrackBase, "");
   NumTracks = 0;
   for (int i = 0; i < 8; i++) { Volume[i] = 100; Pan[i] = 0; strcpy(Name[i], "");}
   MainVolume = 100;
   SampleRate = 44;
   BitRate = 16;
}

void SoundProject::SetBase(const char *BaseFileName) {
	strcpy(TrackBase, BaseFileName);
}

void SoundProject::SetTrackVolume(int Track, int Vol) {
	Volume[Track - 1] = Vol;
}

void SoundProject::SetTrackPan(int Track, int iPan) {
	Pan[Track - 1] = iPan;
}

void SoundProject::SetNumberTracks(int NumTrack) {
	NumTracks = NumTrack;
}

void SoundProject::SetMainVolume(int Vol) {
	MainVolume = Vol;
}

void SoundProject::SetTrackName(int Track, const char *tName) {
	strcpy(Name[(Track - 1)], tName);
}

char *SoundProject::GetBase() {
	return TrackBase;
}

int SoundProject::GetTrackVolume(int Track) {
	return Volume[Track - 1];
}

int SoundProject::GetTrackPan(int Track) {
	return Pan[Track - 1];
}

int SoundProject::GetNumberTracks() {
	return NumTracks;
}

int SoundProject::GetMainVolume() {
	return MainVolume;
}

char *SoundProject::GetTrackName(int Track) {
	return Name[(Track - 1)];
}

void SoundProject::Load(const char *FilePath) {
   ifstream fp;
   char Line[80];
   char *lParam;
   char *rParam;

   fp.open((const char *)FilePath);
   while (!fp.eof()) {
	   fp.getline(Line, 80);
      if (strstr(Line, "=") != NULL) {
			lParam = rParam = strstr(Line, "=");
		   while (lParam[-1] == ' ') lParam -= sizeof(char);   // Delete ending whitespace
		   lParam[0] = '\0';
		   lParam = Line;
		   rParam += sizeof(char); // Take me after the =
		   while (rParam[0] == ' ') rParam += sizeof(char);	// Delete front whitespace
	      strupr(lParam);
	      if (!strcmp(lParam, "TRACKBASE")) {
	      	strcpy(TrackBase, rParam);
	      }
	      if (!strcmp(lParam, "SAMPLERATE")) {
	      	SampleRate = atoi(rParam);
	      }
	      if (!strcmp(lParam, "BITRATE")) {
	      	BitRate = atoi(rParam);
	      }
	      if (!strcmp(lParam, "MAINVOLUME")) {
	      	MainVolume = atoi(rParam);
	      }
	      if (!strcmp(lParam, "NUMTRACKS")) {
	      	NumTracks = atoi(rParam);
	      }
	      if (!strncmp(lParam, "VOLUME", 6)) {
	      	int i;
	         lParam = &lParam[6];
	         i = atoi(lParam);
	      	Volume[i - 1] = atoi(rParam);
	      }
	      if (!strncmp(lParam, "PAN", 3)) {
	      	int i;
	         lParam = &lParam[3];
	         i = atoi(lParam);
	      	Pan[i - 1] = atoi(rParam);
	      }
 	      if (!strncmp(lParam, "NAME", 4)) {
	      	int i;
	         lParam = &lParam[4];
	         i = atoi(lParam);
	      	strcpy(Name[i - 1], rParam);
	      }
	   }
   }
   fp.close();
}

void SoundProject::Save(const char *FilePath) {
   ofstream fp;

   fp.open(FilePath);
   fp << "TRACKBASE=" << TrackBase << endl;
   fp << "SAMPLERATE=" << SampleRate << endl;
   fp << "BITRATE=" << BitRate << endl;
	fp << "NUMTRACKS=" << NumTracks << endl;
   fp << "MAINVOLUME=" << MainVolume << endl;
   for (int i = 0; i < NumTracks; i++) {
   	fp << "VOLUME" << (i+1) << "=" << Volume[i] << endl;
      fp << "PAN" << (i+1) << "=" << Pan[i] << endl;
      fp << "NAME" << (i+1) << "=" << Name[i] << endl;
   }
   fp.close();
}

void SoundProject::SetSampleRate(int SRate) {
   SampleRate = SRate;
}

void SoundProject::SetBitRate(int BRate) {
   BitRate = BRate;
}

int  SoundProject::GetSampleRate() {
   return SampleRate;
}

int  SoundProject::GetBitRate() {
   return BitRate;
}

